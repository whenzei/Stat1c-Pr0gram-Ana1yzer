#pragma once

#ifndef PQL_PATTERN_H
#define PQL_PATTERN_H

#include <string>
#include "pql_clause.h"
#include "tokenizer.h"
#include "pql_global.h"
using std::string;
using std::pair;
using Expression = pair<PqlPatternExpressionType, TokenList>;

/* Stores information of a pattern clause in a PQL select statement */
class PqlPattern : public PqlClause {
private:
  pair<string, PqlPatternType> type_; // assign | if | while
  Entity first_parameter_;
  Expression assign_expression_; // 2nd parameter of assign pattern

public:
  PqlPattern(string, PqlPatternType, string, PqlDeclarationEntity);

  pair<string, PqlPatternType> GetType();
  Entity GetFirstParameter();
  Expression GetAssignExpression();

  void SetAssignExpression(PqlPatternExpressionType, TokenList);

  PqlClauseType GetClauseType();
};
 #endif