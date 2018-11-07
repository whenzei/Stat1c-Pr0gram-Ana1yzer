#pragma once

#ifndef PQL_CLAUSE
#define PQL_CLAUSE

#include "pql_global.h"

// Priority levels of each clause
const int PRIORITY_WITH_CONST_SYN = 16;
const int PRIORITY_WITH_SYN_SYN = 15;
const int PRIORITY_CONSTANT_AND_SYNONYM = 14;
const int PRIORITY_FOLLOWS = 13;
const int PRIORITY_FOLLOWS_T = 12;
const int PRIORITY_MODIFIES = 11;
const int PRIORITY_NORMAL = 10;
const int PRIORITY_AFFECTS_CONST_CONST = 9;
const int PRIORITY_AFFECTS_CONST_SYN = 8;
const int PRIORITY_AFFECTS_SYN_SYN = 7;
const int PRIORITY_AFFECTS_OTHERS = 6;
const int PRIORITY_AFFECTS_UNDERSCORE = 5;
const int PRIORITY_NEXT_T = 4;
const int PRIORITY_AFFECTS_T_CONST_CONST = 3;
const int PRIORITY_AFFECTS_T_CONST_SYN = 2;
const int PRIORITY_AFFECTS_T_SYN_SYN = 1;
const int PRIORITY_AFFECTS_T_OTHERS = 0;
const int PRIORITY_AFFECTS_T_UNDERSCORE = -1;

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