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
  DeclarationKeyword keyword;
  string var_name;

public:
  /* Constructor */
  Declaration(DeclarationKeyword, string);

  /* Getters */
  DeclarationKeyword GetKeyword();
  string GetVarName();
};

#endif