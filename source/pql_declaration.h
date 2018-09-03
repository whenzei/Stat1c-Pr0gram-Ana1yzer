#ifndef PQL_DECLARATION_H
#define PQL_DECLARATION_H

#include <string>
using std::string;

enum PqlDeclarationKeyword {
	kProcedure = 0,
	kVariable,
	kAssign
};

/*
This class stores the infomation of a single declaration in PQL.
*/
class PqlDeclaration {
 private:
  PqlDeclarationKeyword keyword_; // the keyword of the declaration
  string var_name_; // the variable name of the declaration

public:
  /* Constructor */
  PqlDeclaration(PqlDeclarationKeyword, string);

  /* Getters */
  PqlDeclarationKeyword GetKeyword();
  string GetVarName();
};

#endif