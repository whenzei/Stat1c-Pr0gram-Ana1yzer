#pragma once

#ifndef PQL_GROUP_H
#define PQL_GROUP_H

#include "pql_clause.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
using std::vector;
using std::unordered_map;
using std::unordered_set;

/* Contains a list of clauses that are related */
class PqlGroup {
private:
  // Clauses in the group
  vector<PqlClause*> clauses_;
  
  // Priority of the group
  int priority_;

  // True if any clauses in this group uses a synonym that is selected by the query
  bool uses_selection_;

  // A map of synonym to the clause index that uses it
  unordered_map<string, unordered_set<int>> synonym_map_;

public:
  // Constructor
  PqlGroup();

  // Sorts the clauses by linking clauses with common synonyms
  void SortClauses();

  // Adds a clause to the group
  // @param the clause ptr
  void AddClause(PqlClause*);

  // Get the clauses in the group
  // @return the clauses
  vector<PqlClause*> GetClauses();

  // Get the group priority
  int GetPriority();

  // Set the group priority
  // @param the priority value
  void SetPriority(int);

  // Sets uses_selection_ to true
  void UsesSelectionTrue();

  // True if any clauses in this group uses a synonym that is selected by the query
  // @return true/false
  bool GetUsesSelection();
};

#endif