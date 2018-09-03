#include "pql_declaration.h"

PqlDeclaration::PqlDeclaration(PqlDeclarationKeyword keyword, string var_name) {
  this->keyword_ = keyword;
  this->var_name_ = var_name;
}

PqlDeclarationKeyword PqlDeclaration::GetKeyword() { return keyword_; }

string PqlDeclaration::GetVarName() { return var_name_; }