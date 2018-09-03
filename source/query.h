#ifndef QUERY_H
#define QUERY_H

#include "declaration.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

/*
This class parses and stores the PQL query, including the declarations and 'Select' statement.
*/
class Query {
 private:
  vector<Declaration> declarations_; // the list of declarations
  string var_name_; // the variable name of the 'Select' statement

  /* Parsing */
  void Parse(string);
  void ParseStatement(string);

  public:
  /* Constructor */
  Query(string);

  /* Getters */
  vector<Declaration> GetDeclarations();
  string GetVarName();
};

#endif  // !QUERY_H
