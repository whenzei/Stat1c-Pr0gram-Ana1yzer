#ifndef QUERY_H
#define QUERY_H

#include "declaration.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Query {
 private:
  vector<Declaration> declarations_;
  string var_name_;

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
