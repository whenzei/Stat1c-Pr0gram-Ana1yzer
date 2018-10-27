#include "pql_parser.h"
#include "expression_helper.h"
#include "pql_validator.h"
#include "pql_clause.h"
#include "util.h"

const bool DEBUG_FLAG = false;

PqlParser::PqlParser(string query_text, PqlQuery* query) {
  query_text_ = query_text;
  query_ = query;
}

bool PqlParser::Parse() {
  if (query_text_ == "") {
    error_message_ = "Query can not be empty.";
    return false;
  }

  // 1. Split up individual statements by semicolon
  vector<string> statements = Util::Split(query_text_, ';');

  // should have at least 2 statements - a SYN declaration stmt and a clause stmt
  // early return if less than 2
  if (statements.size() < 2) {
    error_message_ = "Incomplete or invalid query.";
    return false;
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
      error_message_ = "Missing select statement.";
    }
    else {
      error_message_ = "Declaration can not be empty.";
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
          &Tokenizer::TokenizeNames,      &Tokenizer::TokenizeDigits,
          &Tokenizer::TokenizePeriod,     &Tokenizer::TokenizeHash,
          &Tokenizer::TokenizeRelationals };
      TokenList tokens = Tokenizer::Tokenize(statement, tokenizer_functions);

      if (DEBUG_FLAG) {
        for (auto token : tokens) {
          std::cout << Tokenizer::Debug(token) << " ";
        }

        std::cout << std::endl;
      }

      if (!ParseSelect(tokens)) return false;
    }
    else {
      error_message_ = "Select clause must be the last statement.";
      return false;
    }
  }
  // 2. Check if it's a declaration and process it
  else {
    TokenizerFunc tokenizer_functions[] = { &Tokenizer::SkipWhitespace,
                                           &Tokenizer::TokenizeWords,
                                           &Tokenizer::TokenizeComma };
    TokenList tokens = Tokenizer::Tokenize(statement, tokenizer_functions);

    if (!ParseDeclaration(tokens)) return false;
  }

  return true;
}

bool PqlParser::ParseDeclaration(TokenList tokens) {
  // 1. Get declaration entity type
  PqlDeclarationEntity entity =
    PqlQuery::DeclarationStringToType(tokens[0].value);
  if (entity == PqlDeclarationEntity::kNone) {
    error_message_ = "Invalid declaration entity.";
    return false;
  }

  // 2. Get variable names
  for (int i = 1; i < tokens.size(); i++) {
    if (tokens[i].type == Tokenizer::TokenType::kEOF) break;
    if (tokens[i].type == Tokenizer::TokenType::kComma) {
      if (i + 1 >= tokens.size()) {
        error_message_ = "Synonym expected after comma in declaration.";
        return false;
      }
      continue;
    }

    string synonym = tokens[i].value;

    // check for grammar
    if (PqlValidator::ValidateIdent(synonym)) {
      // add to declarations
      if (!query_->AddDeclaration(entity, synonym)) {
        error_message_ = "Duplicated declaration synonym.";
        return false;
      }
    }
    else {
      error_message_ = "Declaration synonym has to be in IDENT format.";
      return false;
    }
  }

  return true;
}

bool PqlParser::ParseSelect(TokenList tokens) {
  // 1. Handle selection
  int current_index = 1;
  Declarations declarations = query_->GetDeclarations();

  if (tokens[current_index].type == Tokenizer::TokenType::kName) {
    // 1.1. Handle BOOLEAN
    if (tokens[current_index].value == "BOOLEAN") {
      // do nothing, empty vector denotes boolean selection
    }
    // 1.2. Handle single synonym
    else if (declarations.find(tokens[current_index].value) != declarations.end()) {
      /* LEGACY: TO BE DELETED */
      query_->SetVarName(tokens[current_index].value);
      /* LEGACY: TO BE DELETED */

      // check for attribute
      PqlDeclarationEntity type = declarations.at(tokens[current_index].value);
      string selection = tokens[current_index].value;
      if (tokens[current_index+1].type == Tokenizer::TokenType::kPeriod) {
        if (type == PqlDeclarationEntity::kProgline) {
          error_message_ = "Progline can not have attribute name in selection clause.";
          return false;
        }
        current_index += 2;
        if(!ParseAttribute(tokens, &current_index, &type)) return false;
        current_index--; // step back because ParseAttribute step forward internally
      }
      // append extra 'p' for call.procName
      if (type == PqlDeclarationEntity::kCallName) {
        selection = "0" + selection;
      }
      query_->AddSelection(selection, type);
    }
    else {
      error_message_ = "Select synonym is not declared.";
      return false;
    }
  }
  // 1.3. Handle tuples
  else if (tokens[current_index].type == Tokenizer::TokenType::kRelational && tokens[current_index].value == "<") {
    ++current_index;
    bool has_next = true;
    while(tokens[current_index].type != Tokenizer::TokenType::kRelational && tokens[current_index].value != ">") {
      if (has_next) {
        // expect a synonym
        if (declarations.find(tokens[current_index].value) != declarations.end()) {
          // check for attribute
          PqlDeclarationEntity type = declarations.at(tokens[current_index].value);
          string selection = tokens[current_index].value;
          if (tokens[current_index + 1].type == Tokenizer::TokenType::kPeriod) {
            if (type == PqlDeclarationEntity::kProgline) {
              error_message_ = "Progline can not have attribute name in selection clause.";
              return false;
            }
            current_index += 2;
            if (!ParseAttribute(tokens, &current_index, &type)) return false;
            current_index--; // step back because ParseAttribute step forward internally
          }
          // append '0' infront for call.procName
          if (type == PqlDeclarationEntity::kCallName) {
            selection = "0" + selection;
          }
          query_->AddSelection(selection, type);
        }
        else {
          error_message_ = "Select synonym is not declared.";
          return false;
        }
        has_next = false;
      }
      else {
        // expect a comma
        if (tokens[current_index].type != Tokenizer::TokenType::kComma) {
          error_message_ = "Missing comma in select tuple.";
          return false;
        }
        has_next = true;
      }
      ++current_index;
    }

    // should not has next if tuple is closed
    if (has_next) {
      error_message_ = "Extra comma in select tuple.";
      return false;
    }
  }
  else {
    error_message_ = "Invalid selection";
    return false;
  }

  ++current_index;
  // 2. Handle such that/pattern/with clauses
  string previous_type;
  while (current_index < tokens.size() &&
    tokens[current_index].type != Tokenizer::TokenType::kEOF) {
    if (tokens[current_index].value == "such") {
      if (tokens[current_index + 1].value != "that") {
        error_message_ = "Missing 'that' in such that clause.";
        return false;
      }
    }
    else if (tokens[current_index].value == "that") {
      // 2.1. Handle such that clause
      current_index++;
      if (!ParseSuchthat(tokens, &current_index)) return false;
      previous_type = "suchthat";
    }
    else if (tokens[current_index].value == "pattern") {
      // 2.2. Handle pattern clause
      current_index++;
      if (!ParsePattern(tokens, &current_index)) return false;
      previous_type = "pattern";
    }
    else if (tokens[current_index].value == "with") {
      // 2.3. Handle with clause
      current_index++;
      if (!ParseWith(tokens, &current_index)) return false;
      previous_type = "with";
    }
    else if (tokens[current_index].value == "and" && previous_type != "") {
      // 2.4. Handle 'and'
      if (previous_type == "suchthat") {
        current_index++;
        if (!ParseSuchthat(tokens, &current_index)) return false;
        previous_type = "suchthat";
      }
      else if (previous_type == "pattern") {
        current_index++;
        if (!ParsePattern(tokens, &current_index)) return false;
        previous_type = "pattern";
      }
      else if (previous_type == "with") {
        current_index++;
        if (!ParseWith(tokens, &current_index)) return false;
        previous_type = "with";
      }
    }
    else {
      error_message_ = "Unknown clause in select statement.";
      return false;
    }
    current_index++;
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
    error_message_ = "Unknown such that type.";
    return false;
  }

  // 2. Check opening parentheses
  if (current.type == Tokenizer::TokenType::kOpenParen) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Missing open parentheses in such that clause.";
    return false;
  }

  // 3. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Missing comma in such that parameters.";
    return false;
  }

  // 5. Check second parameter
  if (!ParseParameter(tokens, current_index, &second, &second_type))
    return false;

  // 6. Check closing parentheses
  current = tokens[*current_index];
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    error_message_ = "Missing closing parentheses in such that clause.";
    return false;
  }

  // 7. Check if parameter is defined
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
    }
    else {
      error_message_ = "Parameter in such that clause not declared.";
      return false;
    }
  }
  if (second_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(second) != declarations.end()) {
      second_type = declarations.at(second);
    }
    else {
      error_message_ = "Parameter in such that clause not declared.";
      return false;
    }
  }

  // 8. Check specific types for Modifies and Uses
  if (suchthat_type == PqlSuchthatType::kModifies) {
    if (first_type == PqlDeclarationEntity::kProcedure ||
      first_type == PqlDeclarationEntity::kIdent) {
      suchthat_type = PqlSuchthatType::kModifiesP;
    }
    else {
      suchthat_type = PqlSuchthatType::kModifiesS;
    }
  }
  else if (suchthat_type == PqlSuchthatType::kUses) {
    if (first_type == PqlDeclarationEntity::kProcedure ||
      first_type == PqlDeclarationEntity::kIdent) {
      suchthat_type = PqlSuchthatType::kUsesP;
    }
    else {
      suchthat_type = PqlSuchthatType::kUsesS;
    }
  }

  // 9. Check if parameters are acceptable for the type
  SuchthatParameters acceptable_parameters = suchthat_table.at(suchthat_type);
  // 9.1 Check first parameter
  if (acceptable_parameters.first.find(first_type) ==
    acceptable_parameters.first.end()) {
    error_message_ =
      "First parameter in such that clause is invalid for the type.";
    return false;
  }

  // 9.2 Check second parameter
  if (acceptable_parameters.second.find(second_type) ==
    acceptable_parameters.second.end()) {
    error_message_ =
      "Second parameter in such that clause is invalid for the type.";
    return false;
  }

  // 9. Create such that object
  query_->AddClause(new PqlSuchthat(suchthat_type, first, first_type, second, second_type));

  // 10. Generate group
  GenerateGroup(first, first_type, second, second_type);

  /* LEGACY: TO BE DELETED */
  query_->AddSuchthat(PqlSuchthat(suchthat_type, first, first_type, second, second_type));
  /* LEGACY: TO BE DELETED */

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
      error_message_ =
        "Pattern clause not of type 'assign', 'while', or 'if'.";
      return false;
    }
  }
  else {
    error_message_ = "Pattern synonym not declared.";
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
  }
  else {
    error_message_ = "Missing open parentheses in assign pattern clause.";
    return false;
  }

  // 2. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 3. Check if parameter type matches assign pattern
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
    }
    else {
      error_message_ =
        "First parameter of assign pattern clause is not declared.";
      return false;
    }
  }

  if (pattern_assign.find(first_type) == pattern_assign.end()) {
    error_message_ = "First parameter of assign pattern clause is invalid.";
    return false;
  }

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Missing comma in assign pattern parameters.";
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
    }
    else {
      error_message_ = "Missing quotation in pattern expression parameters.";
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
      error_message_ =
        "Expression in second parameter of assign pattern clause is invalid.";
      return false;
    }

    if (current.type == Tokenizer::TokenType::kQuotation) {
      current = tokens[++*current_index];
    }
    else {
      error_message_ = "Missing quotation in pattern expression parameters.";
      return false;
    }

    // 6. Check for underscore
    if (underscore) {
      if (current.type == Tokenizer::TokenType::kUnderscore) {
        current = tokens[++*current_index];
      }
      else {
        error_message_ =
          "Missing underscore in second parameter of assign pattern clause.";
        return false;
      }
    }
  }

  // 7. Check close parenthesis
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    error_message_ = "Missing close parenthesis in assign pattern clause.";
    return false;
  }

  // 8. Create assign pattern object
  PqlPattern* pattern =
    new PqlPattern(type_name, PqlPatternType::kAssign, first, first_type);
  PqlPatternExpressionType expression_type;
  if (underscore && expression.empty()) {
    expression_type = PqlPatternExpressionType::kUnderscore;
  }
  else if (underscore && !expression.empty()) {
    expression_type = PqlPatternExpressionType::kUnderscoreExpressionUnderscore;
  }
  else {
    expression_type = PqlPatternExpressionType::kExpression;
  }
  pattern->SetAssignExpression(expression_type, ExpressionHelper::ToPostfix(expression));
  query_->AddClause(pattern);

  // 9. Generate group
  GenerateGroup(first, first_type);

  /* LEGACY: TO BE DELETED */
  query_->AddPattern(*pattern);
  /* LEGACY: TO BE DELETED */

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
  }
  else {
    error_message_ = "Missing open parentheses in while pattern clause.";
    return false;
  }

  // 2. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 3. Check if parameter type matches while pattern
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
      if(first_type != PqlDeclarationEntity::kVariable) {
        error_message_ = "First parameter of while pattern clause is invalid.";
        return false;
      }
    }
    else {
      error_message_ = "First parameter of while pattern clause is not declared.";
      return false;
    }
  }

  if (pattern_while.find(first_type) == pattern_while.end()) {
    error_message_ = "First parameter of while pattern clause is invalid.";
    return false;
  }

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Missing comma in while pattern parameters.";
    return false;
  }

  // 5. Check underscore
  if (current.type == Tokenizer::TokenType::kUnderscore) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Second parameter of while pattern clause must be '_'.";
    return false;
  }

  // 6. Check close parenthesis
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    error_message_ = "Missing close parenthesis in while pattern clause.";
    return false;
  }

  // 7. Create pattern object
  query_->AddClause(new PqlPattern(type_name, PqlPatternType::kWhile, first, first_type));

  // 8. Generate group
  GenerateGroup(first, first_type);

  /* LEGACY: TO BE DELETED */
  query_->AddPattern(PqlPattern(type_name, PqlPatternType::kWhile, first, first_type));
  /* LEGACY: TO BE DELETED */

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
  }
  else {
    error_message_ = "Missing open parentheses in if pattern clause.";
    return false;
  }

  // 2. Check first parameter
  if (!ParseParameter(tokens, current_index, &first, &first_type)) return false;

  // 3. Check if parameter type matches if pattern
  Declarations declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
      if (first_type != PqlDeclarationEntity::kVariable) {
        error_message_ = "First parameter of while pattern clause is invalid.";
        return false;
      }
    }
    else {
      error_message_ = "First parameter of if pattern clause is not declared.";
      return false;
    }
  }

  if (pattern_if.find(first_type) == pattern_if.end()) {
    error_message_ = "First parameter of if pattern clause is invalid.";
    return false;
  }

  // 4. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Missing comma in if pattern parameters.";
    return false;
  }

  // 5. Check underscore
  if (current.type == Tokenizer::TokenType::kUnderscore) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Second parameter of if pattern clause must be '_'.";
    return false;
  }

  // 6. Check comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Missing comma in if pattern parameters.";
    return false;
  }

  // 7. Check underscore
  if (current.type == Tokenizer::TokenType::kUnderscore) {
    current = tokens[++*current_index];
  }
  else {
    error_message_ = "Third parameter of if pattern clause must be '_'.";
    return false;
  }

  // 8. Check close parenthesis
  if (current.type != Tokenizer::TokenType::kCloseParen) {
    error_message_ = "Missing close parenthesis in if pattern clause.";
    return false;
  }

  // 9. Create pattern object
  query_->AddClause(new PqlPattern(type_name, PqlPatternType::kIf, first, first_type));

  // 10. Generate group
  GenerateGroup(first, first_type);

  /* LEGACY: TO BE DELETED */
  query_->AddPattern(PqlPattern(type_name, PqlPatternType::kIf, first, first_type));
  /* LEGACY: TO BE DELETED */

  return true;
}

bool PqlParser::ParseWith(TokenList tokens, int* current_index) {
  Token current = tokens[*current_index];
  string left;
  PqlDeclarationEntity left_type;
  string right;
  PqlDeclarationEntity right_type;

  // 1. Get left
  ParseParameter(tokens, current_index, &left, &left_type, "with ");
  if (left_type == PqlDeclarationEntity::kUnderscore) {
    error_message_ = "Invalid left parameter of with clause.";
    return false;
  }

  // 2. Check if left synonym is valid
  Declarations declarations = query_->GetDeclarations();
  if (left_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(left) != declarations.end()) {
      left_type = declarations.at(left);
    }
    else {
      error_message_ = "Left parameter of with clause not declared.";
      return false;
    }
  }

  // 3. Handle period
  current = tokens[*current_index];
  if (left_type == PqlDeclarationEntity::kIdent || left_type == PqlDeclarationEntity::kInteger || left_type == PqlDeclarationEntity::kProgline) {
    if (current.type == Tokenizer::TokenType::kPeriod) {
      error_message_ = "IDENT, INTEGER and progline can not have attribute name.";
      return false;
    }
  }
  else {
    if (current.type == Tokenizer::TokenType::kPeriod) {
      current = tokens[++*current_index];
      // 3.1. Handle attribute
      if(!ParseAttribute(tokens, current_index, &left_type)) return false;

      // append extra 'p' for call.procName
      if (left_type == PqlDeclarationEntity::kCallName) {
        left = "0" + left;
      }
    }
    else {
      error_message_ = "Missing attribute name in with clause.";
      return false;
    }
  }

  // 4. Handle equal
  current = tokens[*current_index];
  if (current.type != Tokenizer::TokenType::kRelational || current.value != "=") {
    error_message_ = "Missing '=' in with clause.";
    return false;
  }

  // 5. Get right
  current = tokens[++*current_index];
  ParseParameter(tokens, current_index, &right, &right_type, "with ");
  if (right_type == PqlDeclarationEntity::kUnderscore) {
    error_message_ = "Invalid right parameter of with clause.";
    return false;
  }

  // 6. Check if right synonym is valid
  if (right_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(right) != declarations.end()) {
      right_type = declarations.at(right);
    }
    else {
      error_message_ = "Right parameter of with clause not declared.";
      return false;
    }
  }

  // 7. Handle period
  current = tokens[*current_index];
  if (right_type == PqlDeclarationEntity::kIdent || right_type == PqlDeclarationEntity::kInteger || right_type == PqlDeclarationEntity::kProgline) {
    if (current.type == Tokenizer::TokenType::kPeriod) {
      error_message_ = "IDENT, INTEGER and progline can not have attribute name.";
      return false;
    }
  }
  else {
    if (current.type == Tokenizer::TokenType::kPeriod) {
      current = tokens[++*current_index];
      // 7.1. Handle attribute
      if (!ParseAttribute(tokens, current_index, &right_type)) return false;

      // append extra 'p' for call.procName
      if (right_type == PqlDeclarationEntity::kCallName) {
        right = "0" + right;
      }
    }
    else {
      error_message_ = "Missing attribute name in with clause.";
      return false;
    }
  }

  // 8. Check that left and right is compatible
  EntitySet set = with_table.at(left_type);
  if (set.find(right_type) == set.end()) {
    error_message_ = "Left parameter incompatible with right parameter in wtih clause.";
    return false;
  }

  // 9. If left and right is the same then ignore the clause
  if (left == right && left_type == right_type) return true;

  // 10. Create with clause
  query_->AddClause(new PqlWith(left, left_type, right, right_type));

  // 11. Generate group
  GenerateGroup(left, left_type, right, right_type);

  --*current_index; // move back 1 step because ParseParameter and ParseAttribute took a step forward at the end
  return true;
}

bool PqlParser::ParseParameter(TokenList tokens, int* current_index,
  string* value, PqlDeclarationEntity* type, string error_keyword) {
  Token current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kQuotation) {
    // 1. Handle quotations
    current = tokens[++*current_index];

    if (PqlValidator::ValidateIdent(current.value)) {
      *value = current.value;
      *type = PqlDeclarationEntity::kIdent;
      current = tokens[++*current_index];
    }
    else {
      error_message_ = error_keyword +
        "clause parameter in quotation is not in ident format.";
      return false;
    }

    // consume closing quotation
    if (current.type == Tokenizer::TokenType::kQuotation) {
    }
    else {
      error_message_ = "Missing quotation in " + error_keyword + "clause parameters.";
      return false;
    }
  }
  else if (current.type == Tokenizer::TokenType::kUnderscore) {
    // 2. Handle underscore
    *value = "_";
    *type = PqlDeclarationEntity::kUnderscore;
  }
  else if (PqlValidator::ValidateInteger(current.value)) {
    // 3. Handle integer
    *value = current.value;
    *type = PqlDeclarationEntity::kInteger;
  }
  else if (PqlValidator::ValidateIdent(current.value)) {
    // 4. Handle ident
    *value = current.value;
    *type = PqlDeclarationEntity::kSynonym;
  }
  else {
    error_message_ = "Invalid " + error_keyword + "clause parameter.";
    return false;
  }

  ++*current_index;
  return true;
}

bool PqlParser::ParseAttribute(TokenList tokens, int* current_index, PqlDeclarationEntity* type) {
  Token current = tokens[*current_index];
  if (current.type != Tokenizer::TokenType::kName) {
    error_message_ = "Invalid attribute name of with clause.";
    return false;
  }

  // 1. Get the attribute name
  string attr = current.value;
  if (attr == "stmt") {
    current = tokens[++*current_index];
    if (current.type != Tokenizer::TokenType::kHash) {
      error_message_ = "Invalid attribute name of with clause.";
      return false;
    }
  }

  // 2. Validate attribute
  if(!PqlValidator::ValidateAttribute(*type, attr)) {
    error_message_ = "Invalid attribute in with clause.";
    return false;
  }

  // 3. Handle procedure name
  if (*type == PqlDeclarationEntity::kCall && attr == "procName") {
      *type = PqlDeclarationEntity::kCallName;
  }
  
  ++*current_index;
  return true;
}

string PqlParser::GetErrorMessage() { return error_message_; }

void PqlParser::GenerateGroup(string first, PqlDeclarationEntity first_type, string second, PqlDeclarationEntity second_type) {
  // CASE 1: Both are synoynms
  if (PqlValidator::IsSynonym(first_type) && PqlValidator::IsSynonym(second_type)) {
    // Find if synonyms have existing groupings
    int first_group = -1;
    int second_group = -1;
    if (synonym_group_.find(first) != synonym_group_.end()) {
      first_group = synonym_group_.at(first);
    }
    if (synonym_group_.find(second) != synonym_group_.end()) {
      second_group = synonym_group_.at(second);
    }

    // CASE 1a: Both no groups
    if (first_group == -1 && second_group == -1) {
      int group = group_ref_.size();
      synonym_group_.insert({ first, group });
      synonym_group_.insert({ second, group });
      group_ref_.insert({ group, -1 });
    }
    // CASE 1b: Both have groups
    else if (first_group != -1 && second_group != -1) {
      // Check if both are in the same group
      if (first_group != second_group) {
        // CASE 1c.1: Both have no group ref
        if (group_ref_.at(first_group) == -1 && group_ref_.at(second_group) == -1) {
          group_ref_[second_group] = first_group;
        }
        // CASE 1c.3: Both have group ref
        else if (group_ref_.at(first_group) != -1 && group_ref_.at(second_group) != -1) {
          int root = second_group;
          while (group_ref_.at(root) != -1) root = group_ref_.at(root);
          group_ref_[root] = first_group;
        }
        // CASE 1c.2: Only 1 have group ref
        else {
          if (group_ref_.at(first_group) != -1) group_ref_[second_group] = first_group;
          else group_ref_[first_group] = second_group;
        }
      }
    }
    // CASE 1c: 1 of them has group
    else {
      if (first_group != -1) synonym_group_.insert({ second, first_group });
      else synonym_group_.insert({ first, second_group });
    }
  }
  // CASE 2: Only 1 synonym
  else if (PqlValidator::IsSynonym(first_type) || PqlValidator::IsSynonym(second_type)) {
    string synonym;
    if (PqlValidator::IsSynonym(first_type)) synonym = first;
    else synonym = second;

    // CASE 2a: No group yet
    if (synonym_group_.find(synonym) == synonym_group_.end()) {
    int group = group_ref_.size();
      synonym_group_.insert({ synonym, group });
      group_ref_.insert({ group, -1 });
    }
    // CASE 2b: Group exist, just ignore
  }
  // CASE 3: No synonyms, just ignore 
}