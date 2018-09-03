#ifndef PQL_QUERY_H
#define PQL_QUERY_H

#include "pql_declaration.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

/*
This class parses and stores the PQL query, including the declarations and 'Select' statement.
*/
class PqlQuery {
 private:
  vector<PqlDeclaration> declarations_; // the list of declarations
  string var_name_; // the variable name of the 'Select' statement

  /* Parsing */
  void Parse(string);
  void ParseStatement(string);

  public:
  /* Constructor */
  PqlQuery(string);

  /* Getters */
  vector<PqlDeclaration> GetDeclarations();
  string GetVarName();
};

#endif  // !QUERY_H
