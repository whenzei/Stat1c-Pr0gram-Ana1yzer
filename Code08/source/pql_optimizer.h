#pragma once

#ifndef PQL_OPTIMIZER_H
#define PQL_OPTIMIZER_H

#include "pql_global.h"
#include "pql_group.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::string;

class PqlOptimizer {
private:
  // Union find of the clauses
  unordered_map<string, int> union_;
  unordered_map<int, int> find_;

  // Collection of clauses
  vector<PqlClause*> clauses_;

  // Collection of selected synonyms
  unordered_set<string> selections_;

  // @return true if a clause uses a synonym that is selected
  bool ClauseUsesSelection(PqlClause*);

public:
  PqlOptimizer();
  vector<PqlGroup> Optimize();
  void AddUnion(PqlClause*, string, PqlDeclarationEntity, string = "", PqlDeclarationEntity = PqlDeclarationEntity::kNone);
  void AddSelection(string);
};

#endif