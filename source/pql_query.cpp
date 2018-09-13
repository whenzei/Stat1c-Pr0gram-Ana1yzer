#include "pql_query.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

PqlQuery::PqlQuery() {}

bool PqlQuery::AddDeclaration(PqlDeclarationEntity entity, string var_name) {
  return declarations_.insert(std::make_pair(var_name, entity)).second;
}

void PqlQuery::SetVarName(string var_name) { this->var_name_ = var_name; }

void PqlQuery::AddSuchthat(PqlSuchthat suchthat) {
  suchthats_.push_back(suchthat);
}

unordered_map<string, PqlDeclarationEntity> PqlQuery::GetDeclarations() { return declarations_; }

string PqlQuery::GetVarName() { return var_name_; }

vector<PqlSuchthat> PqlQuery::GetSuchThats() { return suchthats_; }