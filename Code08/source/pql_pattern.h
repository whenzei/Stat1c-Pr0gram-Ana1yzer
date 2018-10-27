#pragma once

#ifndef PQL_PATTERN_H
#define PQL_PATTERN_H

#include <string>
#include "pql_clause.h"
#include "pql_global.h"
#include "tokenizer.h"
using std::pair;
using std::string;
using Expression = pair<PqlPatternExpressionType, TokenList>;

/* Stores information of a pattern clause in a PQL select statement */
class PqlPattern : public PqlClause {
 private:
  pair<string, PqlPatternType> type_;  // assign | if | while
  Synonym first_parameter_;
  Expression assign_expression_;  // 2nd parameter of assign pattern

 public:
  PqlPattern(string, PqlPatternType, string, PqlDeclarationEntity);

  // @returns a pair containing the name of the type and the PqlPatternType enum
  pair<string, PqlPatternType> GetType();

  // @returns the synonym of the first parameter
  Synonym GetFirstParameter();

  // @returns the assignment expression
  Expression GetAssignExpression();

  // Sets the expression in this object with the given type and
  // TokenList
  // @params: PqlPatternExpressionType the type of the pattern expression
  // @params: TokenList the expression to be set
  void SetAssignExpression(PqlPatternExpressionType, TokenList);

  // @returns the clause type of this object
  PqlClauseType GetClauseType();
};
#endif