#pragma once

#ifndef PQL_PATTERN_H
#define PQL_PATTERN_H

#include <string>
#include "tokenizer.h"
#include "pql_enum.h"
using std::string;
using std::pair;
using Expression = pair<PqlPatternExpressionType, TokenList>;

class PqlPattern {
private:
  PqlPatternType type_;
  pair<string, PqlDeclarationEntity> first_parameter_;
  Expression assign_expression_;

public:
  PqlPattern(PqlPatternType, string, PqlDeclarationEntity);
  PqlPatternType GetType();
  pair<string, PqlDeclarationEntity> GetFirstParameter();
  Expression GetAssignExpression();

  void SetAssignExpression(PqlPatternExpressionType, TokenList);
};
 #endif