#pragma once

#ifndef PQL_GROUP_H
#define PQL_GROUP_H

#include "pql_clause.h"

#include <vector>
#include <unordered_map>
using std::vector;
using std::unordered_map;

class PqlGroup {
private:
  /* clauses in the group */
  vector<PqlClause*> clauses_;
  
  /* priority of the group */
  int priority_;

  /* true if any clauses in group uses a synonym that is selected */
  bool uses_selection_;

  unordered_map<string, vector<int>> synonym_map_;

public:
  PqlGroup();
  void SortClauses();
  void AddClause(PqlClause*);
  vector<PqlClause*> GetClauses();
  int GetPriority();
  void SetPriority(int);
  void UsesSelectionTrue();
  bool GetUsesSelection();
};

#endif