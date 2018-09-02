#include "query.h"

Query::Query() { var_name = ""; }

void Query::AddDeclaration(Declaration declaration) {
  declarations.push_back(declaration);
}

void Query::SetVarName(string var_name) { this->var_name = var_name; }

vector<Declaration> Query::GetDeclarations() { return declarations; }

string Query::GetVarName() { return var_name; }