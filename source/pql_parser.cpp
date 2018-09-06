#include "pql_parser.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

void PqlParser::Parse(string content, PqlQuery* query) {
  if (content == "") {
    cout << "Query cannot be empty." << endl;
    return;
  }

  vector<string> statements;

  size_t last = 0;
  size_t next = 0;
  while ((next = content.find(';', last)) != string::npos) {
    statements.push_back(content.substr(last, next - last));
    last = next + 1;
  }

  if (statements.size() == 0) {
    cout << "Invalid query syntax. Possibly missing semicolon." << endl;
    return;
  }

  for (vector<string>::const_iterator i = statements.begin();
    i != statements.end(); ++i) {
    ParseStatement(*i, query, i+1 == statements.end());
  }
}

void PqlParser::ParseStatement(string statement, PqlQuery* query, bool isLast) {
  if (statement == "") {
    cout << "Invalid query syntax." << endl;
    return;
  }

  vector<string> tokens;

  std::stringstream stream(statement);
  string token;
  while (std::getline(stream, token, ' ')) {
    if (token != "") tokens.push_back(token);
  }

  if (tokens.size() == 0) {
    cout << "Invalid query syntax." << endl;
    return;
  } else {
    if (tokens[0] == "Select") {
      if (isLast) {
        ParseSelect(tokens, query);
      } else {
        cout << "Select clause must be the last statement." << endl;
        return;
      }
    }
    else {
      ParseDeclaration(tokens, query);
    }
  }
}

void PqlParser::ParseSelect(vector<string> tokens, PqlQuery* query) {
  // TODO: check for grammar

  // set variable name
  query->SetVarName(tokens[1]);
}

void PqlParser::ParseDeclaration(vector<string> tokens, PqlQuery* query) {
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
  }

  for (int i = 1; i < tokens.size(); i++) {
    // TODO: check for grammar

    // add to declarations
    query->AddDeclaration(kAssign, tokens[i]);
  }
  
}