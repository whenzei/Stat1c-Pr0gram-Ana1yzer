#include "declaration.h"

Declaration::Declaration(DeclarationKeyword keyword, string var_name) {
  this->keyword = keyword;
  this->var_name = var_name;
}

DeclarationKeyword Declaration::GetKeyword() { return keyword; }

string Declaration::GetVarName() { return var_name; }