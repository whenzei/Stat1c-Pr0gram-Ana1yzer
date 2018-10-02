#pragma once

#ifndef PQL_WITH
#define PQL_WITH

#include <string>
#include "pql_clause.h"

using std::pair;
using std::string;

class PqlWith : public PqlClause {
private:
  Parameters parameters_;

public:
  PqlWith(string, PqlDeclarationEntity, string, PqlDeclarationEntity);

  Parameters GetParameters();
  PqlClauseType GetClauseType();
};

#endif