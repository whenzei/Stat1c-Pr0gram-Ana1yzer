#include "pql_pattern.h"

PqlPattern::PqlPattern(PqlPatternType type, string first, PqlDeclarationEntity first_type) {
  type_ = type;
  first_parameter_ = std::make_pair(first, first_type);
}

PqlPatternType PqlPattern::GetType() { return type_; }

pair<string, PqlDeclarationEntity> PqlPattern::GetFirstParameter() { return first_parameter_; }

void PqlPattern::SetAssignExpression(PqlPatternExpressionType type, TokenList tokens) {
  assign_expression_ = std::make_pair(type, tokens);
}