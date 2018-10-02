#include "pql_with.h"

PqlWith::PqlWith(string first , PqlDeclarationEntity first_type, string second, PqlDeclarationEntity second_type) {
  parameters_ = std::make_pair(std::make_pair(first, first_type) ,std::make_pair(second, second_type));
}

Parameters PqlWith::GetParameters() { return parameters_; }

PqlClauseType PqlWith::GetClauseType() { return PqlClauseType::kWith; }