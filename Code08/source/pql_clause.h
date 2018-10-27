#pragma once

#ifndef PQL_CLAUSE
#define PQL_CLAUSE

#include "pql_global.h"

const int PRIORITY_WITH = 0;
const int PRIORITY_CONSTANT_SYNONYM = 1;
const int PRIORITY_FOLLOWS_MODIFIES = 2;
const int PRIORITY_NORMAL = 3;
const int PRIORITY_AFFECTS = 4;

class PqlClause {
public:
  virtual PqlClauseType GetClauseType() = 0;
  int GetPriority() { return priority_; }

protected:
  int priority_;
};

#endif