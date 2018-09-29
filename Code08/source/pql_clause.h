#pragma once

#ifndef PQL_CLAUSE
#define PQL_CLAUSE

#include "pql_enum.h"

class PqlClause {
public:
  virtual PqlClauseType GetClauseType() = 0;
};

#endif