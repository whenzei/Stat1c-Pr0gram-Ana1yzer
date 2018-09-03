#ifndef DECLARATION_H
#define DECLARATION_H

#include <string>
using std::string;

enum DeclarationKeyword {
	kProcedure = 0,
	kVariable,
	kAssign
};

/*
This class stores the infomation of a single declaration in PQL.
*/
class Declaration {
 private:
  DeclarationKeyword keyword_; // the keyword of the declaration
  string var_name_; // the variable name of the declaration

public:
  /* Constructor */
  Declaration(DeclarationKeyword, string);

  /* Getters */
  DeclarationKeyword GetKeyword();
  string GetVarName();
};

#endif