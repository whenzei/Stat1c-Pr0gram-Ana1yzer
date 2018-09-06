#include "pql_query.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

PqlQuery::PqlQuery() {}

void PqlQuery::AddDeclaration(PqlDeclarationEntity entity, string var_name) {
  declarations_.insert(std::make_pair(var_name, entity));
}

void PqlQuery::SetVarName(string var_name) { this->var_name_ = var_name; }

map<string, PqlDeclarationEntity> PqlQuery::GetDeclarations() { return declarations_; }

string PqlQuery::GetVarName() { return var_name_; }
