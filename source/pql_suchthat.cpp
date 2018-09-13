#include "pql_suchthat.h"
using std::make_pair;

PqlSuchthat::PqlSuchthat(PqlSuchthatType type, string first, PqlDeclarationEntity first_type, string second, PqlDeclarationEntity second_type) {
  type_ = type;
  parameters_ = make_pair(make_pair(first, first_type), make_pair(second, second_type));
}

PqlSuchthatType PqlSuchthat::GetType() { return type_; }

pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>> PqlSuchthat::GetParameters() { return parameters_; }