#include "pql_parser.h"
#include "expression_helper.h"
#include "pql_validator.h"

#include <iostream>
#include <sstream>

const bool DEBUG_FLAG = false;

PqlParser::PqlParser(string queryText, PqlQuery* query) {
  queryText_ = queryText;
  query_ = query;
}

bool PqlParser::Parse() {
  if (queryText_ == "") {
    errorMessage_ = "Query can not be empty.";
    return false;
  }

  vector<string> statements;

  // 1. Split up individual statements
  size_t last = 0;
  size_t next = 0;
  string text;
  while ((next = queryText_.find(';', last)) != string::npos) {
    text = queryText_.substr(last, next - last);
    text = text.substr(text.find_first_not_of(' '));
    statements.push_back(text);
    last = next + 1;
  }
  text = queryText_.substr(last);
  if (text != "") {
    text = text.substr(text.find_first_not_of(' '));
    statements.push_back(text);
  }

  // 2. Process each statement
  for (vector<string>::const_iterator i = statements.begin();
       i != statements.end(); ++i) {
    if (!ParseStatement(*i, i + 1 == statements.end())) return false;
  }

  return true;
}

bool PqlParser::ParseStatement(string statement, bool isLast) {
  if (statement == "") {
    if (isLast) {
      errorMessage_ = "Missing select statement.";
    } else {
      errorMessage_ = "Declaration can not be empty.";
    }
    return false;
  }

  // 1. Check if it's a select statement and process it
  if (statement.substr(0, statement.find(' ')) == "Select") {
    if (isLast) {
      TokenizerFunc tokenizer_functions[] = {
          &Tokenizer::SkipWhitespace,     &Tokenizer::TokenizeParenthesis,
          &Tokenizer::TokenizeComma,      &Tokenizer::TokenizeQuotation,
          &Tokenizer::TokenizeUnderscore, &Tokenizer::TokenizeOperators,
          &Tokenizer::TokenizeNames,      &Tokenizer::TokenizeDigits};
      TokenList tokens = Tokenizer::Tokenize(statement, tokenizer_functions);

      if (DEBUG_FLAG) {
        for (auto token : tokens) {
          std::cout << Tokenizer::Debug(token) << " ";
        }

        std::cout << std::endl;
      }

      if (!ParseSelect(tokens)) return false;
    } else {
      errorMessage_ = "Select clause must be the last statement.";
      return false;
    }
  }
  // 2. Check if it's a declaration and process it
  else {
    TokenizerFunc tokenizer_functions[] = {&Tokenizer::SkipWhitespace,
                                           &Tokenizer::TokenizeWords,
                                           &Tokenizer::TokenizeComma};
    TokenList tokens = Tokenizer::Tokenize(statement, tokenizer_functions);

    if (!ParseDeclaration(tokens)) return false;
  }

  return true;
}

bool PqlParser::ParseSelect(TokenList tokens) {
  // 1. Set select variable name
  if (PqlValidator::ValidateIdent(tokens[1].value)) {
    query_->SetVarName(tokens[1].value);
  } else {
    errorMessage_ = "Select synonym must be in IDENT format.";
    return false;
  }

  // 2. Check if there are such that/pattern/with clauses
  if (tokens.size() > 2) {
    int current_index = 2;
    string previous_type;
    while (current_index < tokens.size() &&
           tokens[current_index].type != Tokenizer::TokenType::kEOF) {
      if (tokens[current_index].value == "such") {
        if (tokens[current_index + 1].value != "that") {
          errorMessage_ = "Missing 'that' in such that clause.";
          return false;
        }
      } else if (tokens[current_index].value == "that") {
        // 2.1. Handle such that clause
        current_index++;
        if (!ParseSuchthat(tokens, &current_index)) return false;
        previous_type = "suchthat";
      } else if (tokens[current_index].value == "pattern") {
        // 2.2. TODO: Handle pattern clause
        current_index++;
        if (!ParsePattern(tokens, &current_index)) return false;
        previous_type = "pattern";
      } else if (tokens[current_index].value == "with") {
        // 2.3. TODO: Handle with clause
        current_index++;
        // if (!ParseWith(tokens, &current_index)) return false;
        previous_type = "with";
      } else if (tokens[current_index].value == "and" && previous_type != "") {
        // 2.4. Handle 'and'
        if (previous_type == "suchthat") {
          current_index++;
          if (!ParseSuchthat(tokens, &current_index)) return false;
          previous_type = "suchthat";
        } else if (previous_type == "pattern") {
          current_index++;
          if (!ParsePattern(tokens, &current_index)) return false;
          previous_type = "pattern";
        } else if (previous_type == "with") {
          current_index++;
          // if (!ParseWith(tokens, &current_index)) return false;
          previous_type = "with";
        }
      } else {
        errorMessage_ = "Unknown clause in select statement.";
        return false;
      }
      current_index++;
    }
  }

  return true;
}

bool PqlParser::ParseSuchthat(TokenList tokens, int* current_index) {
  Token current = tokens[*current_index];
  PqlSuchthatType suchthat_type;
  string first;
  PqlDeclarationEntity first_type;
  string second;
  PqlDeclarationEntity second_type;

  // 1 Identify such that type
  string raw_type = current.value;
  current = tokens[++*current_index];
  if (current.type == Tokenizer::TokenType::kOperator && current.value == "*") {
    raw_type += "*";
    current = tokens[++*current_index];
  }
  suchthat_type = PqlSuchthat::StringToType(raw_type);
  if (suchthat_type == PqlSuchthatType::kNone) {
    errorMessage_ = "Unknown such that type.";
    return false;
  }

  // 2. Check opening parentheses
  if (current.type == Tokenizer::TokenType::kOpenParen) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing open parentheses in such that clause.";
    return false;
  }

  // 3. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing comma in such that parameters.";
    return false;
  }

  // 5. Check second parameter
  if (!ParseParameter(tokens, current_index, &second, &second_type))
    return false;

  // 6. Check closing parentheses
  current = tokens[*current_index];
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    errorMessage_ = "Missing closing parentheses in such that clause.";
    return false;
  }

  // 7. Check if parameter is defined
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
    } else {
      errorMessage_ = "Parameter in such that clause not declared.";
      return false;
    }
  }
  if (second_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(second) != declarations.end()) {
      second_type = declarations.at(second);
    } else {
      errorMessage_ = "Parameter in such that clause not declared.";
      return false;
    }
  }

  // 8. Check specific types for Modifies and Uses
  if (suchthat_type == PqlSuchthatType::kModifies) {
    if (first_type == PqlDeclarationEntity::kProcedure ||
        first_type == PqlDeclarationEntity::kIdent) {
      suchthat_type = PqlSuchthatType::kModifiesP;
    } else {
      suchthat_type = PqlSuchthatType::kModifiesS;
    }
  } else if (suchthat_type == PqlSuchthatType::kUses) {
    if (first_type == PqlDeclarationEntity::kProcedure ||
        first_type == PqlDeclarationEntity::kIdent) {
      suchthat_type = PqlSuchthatType::kUsesP;
    } else {
      suchthat_type = PqlSuchthatType::kUsesS;
    }
  }

  // 9. Check if parameters are acceptable for the type
  SuchthatParameters acceptable_parameters = suchthat_table.at(suchthat_type);
  // 9.1 Check first parameter
  if (acceptable_parameters.first.find(first_type) ==
      acceptable_parameters.first.end()) {
    errorMessage_ =
        "First parameter in such that clause is invalid for the type.";
    return false;
  }

  // 9.2 Check second parameter
  if (acceptable_parameters.second.find(second_type) ==
      acceptable_parameters.second.end()) {
    errorMessage_ =
        "Second parameter in such that clause is invalid for the type.";
    return false;
  }

  // 9. Create such that object
  query_->AddSuchthat(
      PqlSuchthat(suchthat_type, first, first_type, second, second_type));

  return true;
}

bool PqlParser::ParsePattern(TokenList tokens, int* current_index) {
  Token current = tokens[*current_index];

  Declarations declarations = query_->GetDeclarations();
  if (declarations.find(current.value) != declarations.end()) {
    switch (declarations.at(current.value)) {
      case PqlDeclarationEntity::kAssign:
        *current_index += 1;
        if (!ParsePatternAssign(tokens, current_index, current.value))
          return false;
        break;
      case PqlDeclarationEntity::kWhile:
        *current_index += 1;
        if (!ParsePatternWhile(tokens, current_index, current.value))
          return false;
        break;
      case PqlDeclarationEntity::kIf:
        *current_index += 1;
        if (!ParsePatternIf(tokens, current_index, current.value)) return false;
        break;
      default:
        errorMessage_ =
            "Pattern clause not of type 'assign', 'while', or 'if'.";
        return false;
    }
  } else {
    errorMessage_ = "Pattern synonym not declared.";
    return false;
  }

  return true;
}

bool PqlParser::ParsePatternAssign(TokenList tokens, int* current_index,
                                   string type_name) {
  Token current = tokens[*current_index];
  string first;
  PqlDeclarationEntity first_type;

  // 1. Check open parenthesis
  if (current.type == Tokenizer::TokenType::kOpenParen) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing open parentheses in assign pattern clause.";
    return false;
  }

  // 2. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 3. Check if parameter type matches assign pattern
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
    } else {
      errorMessage_ =
          "First parameter of assign pattern clause is not declared.";
      return false;
    }
  }

  if (pattern_assign.find(first_type) == pattern_assign.end()) {
    errorMessage_ = "First parameter of assign pattern clause is invalid.";
    return false;
  }

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing comma in assign pattern parameters.";
    return false;
  }

  // 5. Check for underscore
  bool underscore = false;
  if (current.type == Tokenizer::TokenType::kUnderscore) {
    underscore = true;
    current = tokens[++*current_index];
  }

  TokenList expression;
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    // 6. Parse expression

    if (current.type == Tokenizer::TokenType::kQuotation) {
      current = tokens[++*current_index];
    } else {
      errorMessage_ = "Missing quotation in pattern expression parameters.";
      return false;
    }

    int paren_count = 0;
    while (current.type != Tokenizer::TokenType::kQuotation) {
      expression.push_back(current);
      if (current.type == Tokenizer::TokenType::kOpenParen) paren_count++;
      if (current.type == Tokenizer::TokenType::kCloseParen) paren_count--;
      current = tokens[++*current_index];
    }
    if (!PqlValidator::ValidateExpression(expression)) {
      errorMessage_ =
          "Expression in second parameter of assign pattern clause is invalid.";
      return false;
    }

    if (current.type == Tokenizer::TokenType::kQuotation) {
      current = tokens[++*current_index];
    } else {
      errorMessage_ = "Missing quotation in pattern expression parameters.";
      return false;
    }

    // 6. Check for underscore
    if (underscore) {
      if (current.type == Tokenizer::TokenType::kUnderscore) {
        current = tokens[++*current_index];
      } else {
        errorMessage_ =
            "Missing underscore in second parameter of assign pattern clause.";
        return false;
      }
    }
  }

  // 7. Check close parenthesis
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    errorMessage_ = "Missing close parenthesis in assign pattern clause.";
    return false;
  }

  // 8. Create assign pattern object
  PqlPattern pattern =
      PqlPattern(type_name, PqlPatternType::kAssign, first, first_type);
  PqlPatternExpressionType expression_type;
  if (underscore && expression.empty()) {
    expression_type = PqlPatternExpressionType::kUnderscore;
  } else if (underscore && !expression.empty()) {
    expression_type = PqlPatternExpressionType::kUnderscoreExpressionUnderscore;
  } else {
    expression_type = PqlPatternExpressionType::kExpression;
  }
  pattern.SetAssignExpression(expression_type,
                              ExpressionHelper::ToPostfix(expression));
  query_->AddPattern(pattern);

  return true;
}

bool PqlParser::ParsePatternWhile(TokenList tokens, int* current_index,
                                  string type_name) {
  Token current = tokens[*current_index];
  string first;
  PqlDeclarationEntity first_type;

  // 1. Check open parenthesis
  if (current.type == Tokenizer::TokenType::kOpenParen) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing open parentheses in while pattern clause.";
    return false;
  }

  // 2. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 3. Check if parameter type matches while pattern
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
    } else {
      errorMessage_ =
          "First parameter of while pattern clause is not declared.";
      return false;
    }
  }

  if (pattern_while.find(first_type) == pattern_while.end()) {
    errorMessage_ = "First parameter of while pattern clause is invalid.";
    return false;
  }

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing comma in while pattern parameters.";
    return false;
  }

  // 5. Check underscore
  if (current.type == Tokenizer::TokenType::kUnderscore) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Second parameter of while pattern clause must be '_'.";
    return false;
  }

  // 6. Check close parenthesis
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    errorMessage_ = "Missing close parenthesis in while pattern clause.";
    return false;
  }

  // 7. Create pattern object
  query_->AddPattern(
      PqlPattern(type_name, PqlPatternType::kWhile, first, first_type));

  return true;
}

bool PqlParser::ParsePatternIf(TokenList tokens, int* current_index,
                               string type_name) {
  Token current = tokens[*current_index];
  string first;
  PqlDeclarationEntity first_type;

  // 1. Check open parenthesis
  if (current.type == Tokenizer::TokenType::kOpenParen) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing open parentheses in if pattern clause.";
    return false;
  }

  // 2. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 3. Check if parameter type matches if pattern
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
    } else {
      errorMessage_ = "First parameter of if pattern clause is not declared.";
      return false;
    }
  }

  if (pattern_if.find(first_type) == pattern_if.end()) {
    errorMessage_ = "First parameter of if pattern clause is invalid.";
    return false;
  }

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing comma in if pattern parameters.";
    return false;
  }

  // 5. Check underscore
  if (current.type == Tokenizer::TokenType::kUnderscore) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Second parameter of if pattern clause must be '_'.";
    return false;
  }

  // 6. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Missing comma in if pattern parameters.";
    return false;
  }

  // 7. Check underscore
  if (current.type == Tokenizer::TokenType::kUnderscore) {
    current = tokens[++*current_index];
  } else {
    errorMessage_ = "Third parameter of if pattern clause must be '_'.";
    return false;
  }

  // 8. Check close parenthesis
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    errorMessage_ = "Missing close parenthesis in if pattern clause.";
    return false;
  }

  // 9. Create pattern object
  query_->AddPattern(
      PqlPattern(type_name, PqlPatternType::kIf, first, first_type));

  return true;
}

bool PqlParser::ParseParameter(TokenList tokens, int* current_index,
                               string* value, PqlDeclarationEntity* type) {
  Token current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kQuotation) {
    // 1. Handle quotations
    current = tokens[++*current_index];

    if (PqlValidator::ValidateIdent(current.value)) {
      *value = current.value;
      *type = PqlDeclarationEntity::kIdent;
      current = tokens[++*current_index];
    } else {
      errorMessage_ =
          "Such that parameter in quotation is not in ident format.";
      return false;
    }

    // consume closing quotation
    if (current.type == Tokenizer::TokenType::kQuotation) {
    } else {
      errorMessage_ = "Missing quotation in such that parameters.";
      return false;
    }
  } else if (current.type == Tokenizer::TokenType::kUnderscore) {
    // 2. Handle underscore
    *value = "_";
    *type = PqlDeclarationEntity::kUnderscore;
  } else if (PqlValidator::ValidateInteger(current.value)) {
    // 3. Handle integer
    *value = current.value;
    *type = PqlDeclarationEntity::kInteger;
  } else if (PqlValidator::ValidateIdent(current.value)) {
    // 4. Handle ident
    *value = current.value;
    *type = PqlDeclarationEntity::kSynonym;
  } else {
    errorMessage_ = "Invalid parameter in such that clause.";
    return false;
  }

  ++*current_index;
  return true;
}

bool PqlParser::ParseDeclaration(TokenList tokens) {
  // 1. Get declaration entity type
  PqlDeclarationEntity entity =
      PqlQuery::DeclarationStringToType(tokens[0].value);
  if (entity == PqlDeclarationEntity::kNone) {
    errorMessage_ = "Invalid declaration entity.";
    return false;
  }

  // 2. Get variable names
  for (int i = 1; i < tokens.size(); i++) {
    if (tokens[i].type == Tokenizer::TokenType::kEOF) break;
    if (tokens[i].type == Tokenizer::TokenType::kComma) {
      if (i + 1 >= tokens.size()) {
        errorMessage_ = "Synonym expected after comma in declaration.";
        return false;
      }
      continue;
    }

    string synonym = tokens[i].value;

    // check for grammar
    if (PqlValidator::ValidateIdent(synonym)) {
      // add to declarations
      if (!query_->AddDeclaration(entity, synonym)) {
        errorMessage_ = "Duplicated declaration synonym.";
        return false;
      }
    } else {
      errorMessage_ = "Declaration synonym has to be in IDENT format.";
      return false;
    }
  }

  return true;
}

string PqlParser::GetErrorMessage() { return errorMessage_; }