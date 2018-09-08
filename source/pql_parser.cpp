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
  while ((next = queryText_.find(';', last)) != string::npos) {
    statements.push_back(queryText_.substr(last, next - last));
    last = next + 1;
  }
  statements.push_back(queryText_.substr(last));

  for (vector<string>::const_iterator i = statements.begin();
    i != statements.end(); ++i) {
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

  vector<string> tokens;

  std::stringstream stream(statement);
  string token;
  while (std::getline(stream, token, ' ')) {
    if (token != "") tokens.push_back(token);
  }

  if (tokens.size() == 0) {
    errorMessage_ = "Statements can not be empty.";
    return false;
  } else {
    if (tokens[0] == "Select") {
      if (isLast) {
        if (!ParseSelect(tokens)) return false;
      } else {
        errorMessage_ = "Select clause must be the last statement.";
        return false;
      }
    }
    else {
      if (!ParseDeclaration(tokens)) return false;
    }
  }

  return true;
}

bool PqlParser::ParseSelect(vector<string> tokens) {
  // TODO: check for grammar

  // set variable name
  query_->SetVarName(tokens[1]);

  return true;
}

bool PqlParser::ParseDeclaration(vector<string> tokens) {
  string entity_raw = tokens[0];
  PqlDeclarationEntity entity;

  if (entity_raw == "stmt") {
    entity = kStmt;
  } 
  else if (entity_raw == "read") {
    entity = kRead;
  } 
  else if (entity_raw == "print") {
    entity = kPrint;
  } 
  else if (entity_raw == "call") {
    entity = kCall;
  } 
  else if (entity_raw == "while") {
    entity = kWhile;
  }
  else if (entity_raw == "if") {
    entity = kIf;
  }
  else if (entity_raw == "assign") {
    entity = kAssign;
  }
  else if (entity_raw == "variable") {
    entity = kVariable;
  }
  else if (entity_raw == "constant") {
    entity = kConstant;
  }
  else if (entity_raw == "prog_line") {
    entity = kProgline;
  }
  else if (entity_raw == "procedure") {
    entity = kProcedure;
  } else {
    errorMessage_ = "Invalid declaration entity.";
    return false;
  }

  for (int i = 1; i < tokens.size(); i++) {
    string synonym = tokens[i];
    
    // check for comma
    if (i != tokens.size() - 1) {
      if (synonym.back() != ',') {
        errorMessage_ = "Missing comma in declaration synonyms.";
        return false;
      } else {
        synonym = synonym.substr(0, synonym.size() - 1);
      }
    }

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