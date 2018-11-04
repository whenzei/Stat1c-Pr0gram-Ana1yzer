#pragma once

#ifndef PQL_CLAUSE
#define PQL_CLAUSE

#include "pql_global.h"

// Priority levels of each clause
const int PRIORITY_CONSTANT_AND_SYNONYM = 10;
const int PRIORITY_FOLLOWS = 9;
const int PRIORITY_FOLLOWS_T = 8;
const int PRIORITY_MODIFIES = 7;
const int PRIORITY_WITH = 6;
const int PRIORITY_NORMAL = 5;
const int PRIORITY_AFFECTS = 4;
const int PRIORITY_AFFECTS_T = 3;

/* A base class for clauses */
class PqlClause {
 public:
  virtual PqlClauseType GetClauseType() = 0;

  // The priority of the clause
  int GetPriority();
  void SetPriority(int);

  // The synonyms used in the clause
  pair<string, string> GetSynonyms();
  void SetSynonyms(string, string);

protected:
  int priority_ = PRIORITY_NORMAL;
  pair<string, string> synonyms_;
};

#endif