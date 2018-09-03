#include "declaration.h"

Declaration::Declaration(DeclarationKeyword keyword, string var_name) {
  this->keyword_ = keyword;
  this->var_name_ = var_name;
}

DeclarationKeyword Declaration::GetKeyword() { return keyword_; }

string Declaration::GetVarName() { return var_name_; }