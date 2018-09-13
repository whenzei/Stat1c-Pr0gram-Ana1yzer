#include "pql_parser.h"
#include "pql_validator.h"

#include <sstream>
#include <iostream>

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
  

  for (vector<string>::const_iterator i = statements.begin(); i != statements.end(); ++i) {
    if (!ParseStatement(*i, i+1 == statements.end())) return false;
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

  if (statement.substr(0, statement.find(' ')) == "Select") {
    if (isLast) {
      TokenizerFunc tokenizer_functions[] = {
        &Tokenizer::SkipWhitespace,
        &Tokenizer::TokenizeParenthesis, &Tokenizer::TokenizeComma, &Tokenizer::TokenizeQuotation,
        &Tokenizer::TokenizeUnderscore, &Tokenizer::TokenizeWords };
      TokenList tokens = Tokenizer::Tokenize(statement, tokenizer_functions);

      for (int i = 0; i < tokens.size(); i++) {
        std::cout << Tokenizer::Debug(tokens[i]) << std::endl;
      }

      if (!ParseSelect(tokens)) return false;
    }
    else {
      errorMessage_ = "Select clause must be the last statement.";
      return false;
    }
  }
  else {
    TokenizerFunc tokenizer_functions[] = {
          &Tokenizer::SkipWhitespace, &Tokenizer::TokenizeWords, &Tokenizer::TokenizeComma };
    TokenList tokens = Tokenizer::Tokenize(statement, tokenizer_functions);
    
    for (int i = 0; i < tokens.size(); i++) {
      std::cout << Tokenizer::Debug(tokens[i]) << std::endl;
    }

    if (!ParseDeclaration(tokens)) return false;
  }

  return true;
}

bool PqlParser::ParseSelect(TokenList tokens) {
  // set variable name
  if (PqlValidator::ValidateIdent(tokens[1].value)) {
    query_->SetVarName(tokens[1].value);
  }
  else {
    errorMessage_ = "Select synonym must be in IDENT format.";
    return false;
  }
  
  if (tokens.size() > 2) { // there are other clauses
    int current_index = 2;
    string previous_type;
    while (current_index < tokens.size() && tokens[current_index].type != Tokenizer::TokenType::kEOF) {
      if (tokens[current_index].value == "such") {
        if (tokens[current_index+1].value != "that") {
          errorMessage_ = "Missing 'that' in such that clause.";
          return false;
        }
      }
      else if (tokens[current_index].value == "that") {
        // 1. Handle such that clause
        current_index++;
        if(!ParseSuchthat(tokens, &current_index)) return false;
        previous_type = "suchthat";
      }
      else if (tokens[current_index].value == "pattern") {
        // 2. TODO: Handle pattern clause
        current_index++;
        //if (!ParsePattern(tokens, &current_index)) return false;
        previous_type = "pattern";
      } 
      else if (tokens[current_index].value == "with") {
        // 3. TODO: Handle with clause
        current_index++;
        //if (!ParseWith(tokens, &current_index)) return false;
        previous_type = "with";
      } 
      else if (tokens[current_index].value == "and" && previous_type != "") {
        // 4. Handle 'and'
        if(previous_type == "suchthat") {
          current_index++;
          if (!ParseSuchthat(tokens, &current_index)) return false;
          previous_type = "suchthat";
        }
        else if (previous_type == "pattern") {
          current_index++;
          //if (!ParsePattern(tokens, &current_index)) return false;
          previous_type = "pattern";
        }
        else if (previous_type == "with") {
          current_index++;
          //if (!ParseWith(tokens, &current_index)) return false;
          previous_type = "with";
        }
      }
      else {
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

  // identify such that type
  if (current.value == "Modifies") {
    suchthat_type = PqlSuchthatType::kModifies;
  }
  else if (current.value == "Uses") {
    suchthat_type = PqlSuchthatType::kUses;
  }
  else if (current.value == "Calls") {
    suchthat_type = PqlSuchthatType::kCalls;
  }
  else if (current.value == "Calls*") {
    suchthat_type = PqlSuchthatType::kCallsT;
  }
  else if (current.value == "Parent") {
    suchthat_type = PqlSuchthatType::kParent;
  }
  else if (current.value == "Parent*") {
    suchthat_type = PqlSuchthatType::kParentT;
  }
  else if (current.value == "Follows") {
    suchthat_type = PqlSuchthatType::kFollows;
  }
  else if (current.value == "Follows*") {
    suchthat_type = PqlSuchthatType::kFollowsT;
  }
  else if (current.value == "Next") {
    suchthat_type = PqlSuchthatType::kNext;
  }
  else if (current.value == "Next*") {
    suchthat_type = PqlSuchthatType::kNextT;
  }
  else if (current.value == "Affects") {
    suchthat_type = PqlSuchthatType::kAffects;
  }
  else if (current.value == "Affects*") {
    suchthat_type = PqlSuchthatType::kAffectsT;
  }
  else {
    errorMessage_ = "Unknown such that type.";
    return false;
  }

  current = tokens[++*current_index];
  // 1. Opening parentheses
  if (current.type == Tokenizer::TokenType::kOpenParen) {
    current = tokens[++*current_index];
  }
  else {
    errorMessage_ = "Missing open parentheses in such that clause.";
    return false;
  }

  // 2. First parameter
  if (!ParseSuchthatParameter(tokens, current_index, &first, &first_type)) return false;

  // 3. Comma
  current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kComma) {
    current = tokens[++*current_index];
  }
  else {
    errorMessage_ = "Missing comma in such that parameters.";
    return false;
  }

  // 4. Second parameter
  if (!ParseSuchthatParameter(tokens, current_index, &second, &second_type)) return false;

  // 5. Closing parentheses
  current = tokens[*current_index];
  if (!current.type == Tokenizer::TokenType::kCloseParen) {
    errorMessage_ = "Missing closing parentheses in such that clause.";
    return false;
  }

  // 6. Check parameter validity
  unordered_map<string, PqlDeclarationEntity> declarations = query_->GetDeclarations();
  if (first_type == PqlDeclarationEntity::kSynonym) {
    if(declarations.find(first) != declarations.end()) {
      first_type = declarations.at(first);
    } else {
      errorMessage_ = "Parameter in such that clause not declared.";
      return false;
    }
  }
  if (second_type == PqlDeclarationEntity::kSynonym) {
    if (declarations.find(second) != declarations.end()) {
      second_type = declarations.at(second);
    }
    else {
      errorMessage_ = "Parameter in such that clause not declared.";
      return false;
    }
  }

  // 7. Check if parameter matches such that type
  if (suchthat_type == PqlSuchthatType::kModifies) {
    if (first_type == PqlDeclarationEntity::kProcedure || first_type == PqlDeclarationEntity::kIdent) {
      suchthat_type = PqlSuchthatType::kModifiesP;
    }
    else {
      suchthat_type = PqlSuchthatType::kModifiesS;
    }
  }
  else if (suchthat_type == PqlSuchthatType::kUses) {
    if (first_type == PqlDeclarationEntity::kProcedure || first_type == PqlDeclarationEntity::kIdent) {
      suchthat_type = PqlSuchthatType::kUsesP;
    }
    else {
      suchthat_type = PqlSuchthatType::kUsesS;
    }
  }

  pair<unordered_set<PqlDeclarationEntity>, unordered_set<PqlDeclarationEntity>> acceptable_parameters = suchthat_table.at(suchthat_type);
  // 7.1 Check first parameter
  if(acceptable_parameters.first.find(first_type) == acceptable_parameters.first.end()) {
    errorMessage_ = "First parameter in such that clause is invalid for the type.";
    return false;
  }

  // 7.2 Check second parameter
  if (acceptable_parameters.second.find(second_type) == acceptable_parameters.second.end()) {
    errorMessage_ = "Second parameter in such that clause is invalid for the type.";
    return false;
  }

  // 8. Create such that object
  query_->AddSuchthat(PqlSuchthat(suchthat_type, first, first_type, second, second_type));
}

bool PqlParser::ParseSuchthatParameter(TokenList tokens, int* current_index, string* value, PqlDeclarationEntity* type) {
  Token current = tokens[*current_index];
  if (current.type == Tokenizer::TokenType::kQuotation) {
    // 2.1 Handle quotations
    current = tokens[++*current_index];

    if (PqlValidator::ValidateIdent(current.value)) {
      *value = current.value;
      *type = PqlDeclarationEntity::kIdent;
      current = tokens[++*current_index];
    }
    else {
      errorMessage_ = "Such that parameter in quotation is not in ident format.";
      return false;
    }

    // consume closing quotation
    if (current.type == Tokenizer::TokenType::kQuotation) {
    }
    else {
      errorMessage_ = "Missing quotation in such that parameters.";
      return false;
    }
  }
  else if (current.type == Tokenizer::TokenType::kUnderscore) {
    // 2.2 Handle underscore
    *value = "_";
    *type = PqlDeclarationEntity::kUnderscore;
  }
  else if (PqlValidator::ValidateInteger(current.value)) {
    // 2.3 Handle integer
    *value = current.value;
    *type = PqlDeclarationEntity::kInteger;
  }
  else if (PqlValidator::ValidateIdent(current.value)) {
    // 2.4 Handle ident
    *value = current.value;
    *type = PqlDeclarationEntity::kSynonym;
  }
  else {
    errorMessage_ = "Invalid parameter in such that clause.";
    return false;
  }

  ++*current_index;
  return true;
}

bool PqlParser::ParseDeclaration(TokenList tokens) {
  string entity_raw = tokens[0].value;
  PqlDeclarationEntity entity;

  if (entity_raw == "stmt") {
    entity = PqlDeclarationEntity::kStmt;
  } 
  else if (entity_raw == "read") {
    entity = PqlDeclarationEntity::kRead;
  } 
  else if (entity_raw == "print") {
    entity = PqlDeclarationEntity::kPrint;
  } 
  else if (entity_raw == "call") {
    entity = PqlDeclarationEntity::kCall;
  } 
  else if (entity_raw == "while") {
    entity = PqlDeclarationEntity::kWhile;
  }
  else if (entity_raw == "if") {
    entity = PqlDeclarationEntity::kIf;
  }
  else if (entity_raw == "assign") {
    entity = PqlDeclarationEntity::kAssign;
  }
  else if (entity_raw == "variable") {
    entity = PqlDeclarationEntity::kVariable;
  }
  else if (entity_raw == "constant") {
    entity = PqlDeclarationEntity::kConstant;
  }
  else if (entity_raw == "prog_line") {
    entity = PqlDeclarationEntity::kProgline;
  }
  else if (entity_raw == "procedure") {
    entity = PqlDeclarationEntity::kProcedure;
  } else {
    errorMessage_ = "Invalid declaration entity.";
    return false;
  }

  for (int i = 1; i < tokens.size(); i++) {
    if (tokens[i].type == Tokenizer::TokenType::kEOF) break;
    if (tokens[i].type == Tokenizer::TokenType::kComma) {
      if (i+1 >= tokens.size()) {
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

string PqlParser::GetErrorMessage() {
  return errorMessage_;
}