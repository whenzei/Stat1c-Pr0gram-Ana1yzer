#include "pql_pattern.h"

PqlPattern::PqlPattern(string type_name, PqlPatternType type, string first,
                       PqlDeclarationEntity first_type) {
  type_ = std::make_pair(type_name, type);
  first_parameter_ = std::make_pair(first, first_type);
}

pair<string, PqlPatternType> PqlPattern::GetType() { return type_; }

Synonym PqlPattern::GetFirstParameter() { return first_parameter_; }

Expression PqlPattern::GetAssignExpression() { return assign_expression_; }

void PqlPattern::SetAssignExpression(PqlPatternExpressionType type,
                                     TokenList tokens) {
  assign_expression_ = std::make_pair(type, tokens);
}

PqlClauseType PqlPattern::GetClauseType() { return PqlClauseType::kPattern; }