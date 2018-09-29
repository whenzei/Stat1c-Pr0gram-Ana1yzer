#include "pql_with.h"

PqlWith::PqlWith(string first , PqlDeclarationEntity first_type, PqlAttrName first_attr, 
                 string second, PqlDeclarationEntity second_type, PqlAttrName second_attr) {
  left_ = std::make_pair(std::make_pair(first, first_type), first_attr);
  right_ = std::make_pair(std::make_pair(second, second_type), second_attr);
}

pair<WithType, PqlAttrName> PqlWith::GetLeft() { return left_; }

pair<WithType, PqlAttrName> PqlWith::GetRight() { return right_; }

PqlClauseType PqlWith::GetClauseType() { return PqlClauseType::kWith; }