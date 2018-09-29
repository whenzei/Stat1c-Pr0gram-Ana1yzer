#pragma once

#ifndef PQL_WITH
#define PQL_WITH

#include <string>
#include "pql_clause.h"

using std::pair;
using std::string;
using WithType = pair<string, PqlDeclarationEntity>;

class PqlWith : public PqlClause {
private:
  pair<WithType, PqlAttrName> left_;
  pair<WithType, PqlAttrName> right_;

public:
  PqlWith(string, PqlDeclarationEntity, PqlAttrName, string, PqlDeclarationEntity, PqlAttrName);

  pair<WithType, PqlAttrName> GetLeft();
  pair<WithType, PqlAttrName> GetRight();
  PqlClauseType GetClauseType();
};

#endif