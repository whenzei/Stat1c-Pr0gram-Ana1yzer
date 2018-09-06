#ifndef PQL_QUERY_H
#define PQL_QUERY_H

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

enum PqlDeclarationEntity {
  kStmt = 0,
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssign,
  kVariable,
  kConstant,
  kProgline,
  kProcedure
};

/*
This class parses and stores the PQL query, including the declarations and
'Select' statement.
*/
class PqlQuery {
 private:
  map<string, PqlDeclarationEntity> declarations_; // the list of declarations
  string var_name_;  // the variable name of the 'Select' statement

 public:
  /* Constructor */
  PqlQuery();

  /* Setters */
  void AddDeclaration(PqlDeclarationEntity, string);
  void SetVarName(string);

  /* Getters */
  map<string, PqlDeclarationEntity> GetDeclarations();
  string GetVarName();
};

#endif  // !QUERY_H
