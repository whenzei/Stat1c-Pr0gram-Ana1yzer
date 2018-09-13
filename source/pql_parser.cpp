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
        &Tokenizer::TokenizeUnderscore, &Tokenizer::TokenizeDigits, &Tokenizer::TokenizeWords };
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
    Token current = tokens[2];
    while (current.type != Tokenizer::TokenType::kEOF) {
      // check for such that
    }
  }

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