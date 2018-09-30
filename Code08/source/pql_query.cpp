#include "pql_query.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

PqlQuery::PqlQuery() {}

bool PqlQuery::AddDeclaration(PqlDeclarationEntity entity, string var_name) {
  return declarations_.insert(std::make_pair(var_name, entity)).second;
}

void PqlQuery::AddSelection(string selection) {
  selections_.push_back(selection);
}

void PqlQuery::AddClause(PqlClause* clause) {
  clauses_.push_back(clause);
}

void PqlQuery::SetVarName(string var_name) { this->var_name_ = var_name; }

void PqlQuery::AddSuchthat(PqlSuchthat suchthat) {
  suchthats_.push_back(suchthat);
}

void PqlQuery::AddPattern(PqlPattern pattern) {
  patterns_.push_back(pattern);
}

Declarations PqlQuery::GetDeclarations() { return declarations_; }

vector<string> PqlQuery::GetSelections() { return selections_; }

string PqlQuery::GetVarName() { return var_name_; }

vector<PqlSuchthat> PqlQuery::GetSuchThats() { return suchthats_; }

vector<PqlPattern> PqlQuery::GetPatterns() { return patterns_; }

vector<PqlClause*> PqlQuery::GetClauses() { return clauses_; }

PqlDeclarationEntity PqlQuery::DeclarationStringToType(string input) {
  if (input == "stmt") {
    return PqlDeclarationEntity::kStmt;
  }
  else if (input == "read") {
    return PqlDeclarationEntity::kRead;
  }
  else if (input == "print") {
    return PqlDeclarationEntity::kPrint;
  }
  else if (input == "call") {
    return PqlDeclarationEntity::kCall;
  }
  else if (input == "while") {
    return PqlDeclarationEntity::kWhile;
  }
  else if (input == "if") {
    return PqlDeclarationEntity::kIf;
  }
  else if (input == "assign") {
    return PqlDeclarationEntity::kAssign;
  }
  else if (input == "variable") {
    return PqlDeclarationEntity::kVariable;
  }
  else if (input == "constant") {
    return PqlDeclarationEntity::kConstant;
  }
  else if (input == "prog_line") {
    return PqlDeclarationEntity::kProgline;
  }
  else if (input == "procedure") {
    return PqlDeclarationEntity::kProcedure;
  }
  else {
    return PqlDeclarationEntity::kNone;
  }
}