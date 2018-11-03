#pragma once

#ifndef PQL_CLAUSE
#define PQL_CLAUSE

#include "pql_global.h"

const int PRIORITY_WITH = 10;
const int PRIORITY_CONSTANT_AND_SYNONYM = 9;
const int PRIORITY_FOLLOWS_MODIFIES = 8;
const int PRIORITY_NORMAL = 7;
const int PRIORITY_AFFECTS = 6;

class PqlClause {
 public:
  virtual PqlClauseType GetClauseType() = 0;

  int GetPriority();
  void SetPriority(int);

  string GetSynonym();
  void SetSynonym(string);

  bool Compare(PqlClause*, PqlClause*);

protected:
  int priority_ = PRIORITY_NORMAL;
  string synonym_;
};

#endif