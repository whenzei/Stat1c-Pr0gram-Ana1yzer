#include "query.h"
#include "declaration.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

Query::Query(string content) {
  if (content != "")
    Parse(content);
  else
    cout << "Query cannot be empty!" << endl;
}

vector<Declaration> Query::GetDeclarations() { return declarations; }

string Query::GetVarName() { return var_name; }

void Query::Parse(string content) {
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
    ParseStatement(*i);
  }
}

void Query::ParseStatement(string statement) {
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

  if (tokens.size() == 0 || tokens.size() > 2) {
    cout << "Invalid query syntax." << endl;
    return;
  }

  string keyword = tokens[0];
  if (keyword == "Select") {
    var_name = tokens[1];
  } else if (keyword == "assign") {
    Declaration declaration(kAssign, tokens[1]);
    declarations.push_back(declaration);
  } else {
    cout << "Invalid query keyword." << endl;
  }
}