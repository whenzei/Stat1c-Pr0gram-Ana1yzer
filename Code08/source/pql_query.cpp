#include "pql_query.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

PqlQuery::PqlQuery() {}

PqlQuery::~PqlQuery() {
  for(int i = 0; i < clauses_.size(); i++) {
    delete clauses_[i];
    clauses_[i] = nullptr;
  }
}

bool PqlQuery::AddDeclaration(PqlDeclarationEntity entity, string var_name) {
  return declarations_.insert(std::make_pair(var_name, entity)).second;
}

void PqlQuery::AddSelection(string selection, PqlDeclarationEntity type) {
  selections_.push_back(std::make_pair(selection, type));
  if (selection[0] == '0') optimizer_.AddSelection(selection.substr(1));
  else optimizer_.AddSelection(selection);
}

void PqlQuery::AddClause(PqlClause* clause) { 
  clauses_.push_back(clause);
  if (clause->GetClauseType() == PqlClauseType::kPattern) {
    PqlPattern* pattern = (PqlPattern*)clause;
    PqlDeclarationEntity type;
    if (pattern->GetType().second == PqlPatternType::kAssign) type = PqlDeclarationEntity::kAssign;
    else if (pattern->GetType().second == PqlPatternType::kWhile) type = PqlDeclarationEntity::kWhile;
    else if (pattern->GetType().second == PqlPatternType::kIf) type = PqlDeclarationEntity::kIf;
    optimizer_.AddUnion(clause, pattern->GetType().first, type, pattern->GetFirstParameter().first, pattern->GetFirstParameter().second);
  } else if (clause->GetClauseType() == PqlClauseType::kSuchthat) {
    Parameters param = ((PqlSuchthat*)clause)->GetParameters();
    string param1 = param.first.first;
    string param2 = param.second.first;
    if (param1[0] == '0') param1 = param1.substr(1);
    if (param2[0] == '0') param2 = param2.substr(1);
    optimizer_.AddUnion(clause, param1, param.first.second, param2, param.second.second);
  } else if (clause->GetClauseType() == PqlClauseType::kWith) {
    Parameters param = ((PqlWith*)clause)->GetParameters();
    string param1 = param.first.first;
    string param2 = param.second.first;
    if (param1[0] == '0') param1 = param1.substr(1);
    if (param2[0] == '0') param2 = param2.substr(1);
    optimizer_.AddUnion(clause, param1, param.first.second, param2, param.second.second);
  }
}

void PqlQuery::SetVarName(string var_name) { this->var_name_ = var_name; }

void PqlQuery::AddSuchthat(PqlSuchthat suchthat) {
  suchthats_.push_back(suchthat);
}

void PqlQuery::AddPattern(PqlPattern pattern) { patterns_.push_back(pattern); }

Declarations PqlQuery::GetDeclarations() { return declarations_; }

vector<Synonym> PqlQuery::GetSelections() { return selections_; }

string PqlQuery::GetVarName() { return var_name_; }

vector<PqlSuchthat> PqlQuery::GetSuchThats() { return suchthats_; }

vector<PqlPattern> PqlQuery::GetPatterns() { return patterns_; }

vector<PqlClause*> PqlQuery::GetClauses() { return clauses_; }

vector<PqlGroup> PqlQuery::GetGroups() { return groups_; }

PqlDeclarationEntity PqlQuery::DeclarationStringToType(string input) {
  if (input == "stmt") {
    return PqlDeclarationEntity::kStmt;
  } else if (input == "read") {
    return PqlDeclarationEntity::kRead;
  } else if (input == "print") {
    return PqlDeclarationEntity::kPrint;
  } else if (input == "call") {
    return PqlDeclarationEntity::kCall;
  } else if (input == "while") {
    return PqlDeclarationEntity::kWhile;
  } else if (input == "if") {
    return PqlDeclarationEntity::kIf;
  } else if (input == "assign") {
    return PqlDeclarationEntity::kAssign;
  } else if (input == "variable") {
    return PqlDeclarationEntity::kVariable;
  } else if (input == "constant") {
    return PqlDeclarationEntity::kConstant;
  } else if (input == "prog_line") {
    return PqlDeclarationEntity::kProgline;
  } else if (input == "procedure") {
    return PqlDeclarationEntity::kProcedure;
  } else {
    return PqlDeclarationEntity::kNone;
  }
}

void PqlQuery::Optimize() { groups_ = optimizer_.Optimize(); }