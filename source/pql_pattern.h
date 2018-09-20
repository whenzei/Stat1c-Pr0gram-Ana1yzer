#pragma once

#ifndef PQL_PATTERN_H
#define PQL_PATTERN_H

#include <string>
#include "tokenizer.h"
#include "pql_enum.h"
using std::string;
using std::pair;
using Expression = pair<PqlPatternExpressionType, TokenList>;

/* Stores information of a pattern clause in a PQL select statement */
class PqlPattern {
private:
  PqlPatternType type_; // assign | if | while
  pair<string, PqlDeclarationEntity> first_parameter_;
  Expression assign_expression_; // 2nd parameter of assign pattern

public:
  PqlPattern(PqlPatternType, string, PqlDeclarationEntity);
  PqlPatternType GetType();
  pair<string, PqlDeclarationEntity> GetFirstParameter();
  Expression GetAssignExpression();

  void SetAssignExpression(PqlPatternExpressionType, TokenList);
};
 #endif