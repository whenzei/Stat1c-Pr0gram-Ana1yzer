#include "pql_suchthat.h"
using std::make_pair;

PqlSuchthat::PqlSuchthat(PqlSuchthatType type, string first,
                         PqlDeclarationEntity first_type, string second,
                         PqlDeclarationEntity second_type) {
  type_ = type;
  parameters_ =
      make_pair(make_pair(first, first_type), make_pair(second, second_type));
}

PqlSuchthatType PqlSuchthat::GetType() { return type_; }

Parameters PqlSuchthat::GetParameters() { return parameters_; }

PqlSuchthatType PqlSuchthat::StringToType(string input) {
  if (input == "Modifies") {
    return PqlSuchthatType::kModifies;
  } else if (input == "Uses") {
    return PqlSuchthatType::kUses;
  } else if (input == "Calls") {
    return PqlSuchthatType::kCalls;
  } else if (input == "Calls*") {
    return PqlSuchthatType::kCallsT;
  } else if (input == "Parent") {
    return PqlSuchthatType::kParent;
  } else if (input == "Parent*") {
    return PqlSuchthatType::kParentT;
  } else if (input == "Follows") {
    return PqlSuchthatType::kFollows;
  } else if (input == "Follows*") {
    return PqlSuchthatType::kFollowsT;
  } else if (input == "Next") {
    return PqlSuchthatType::kNext;
  } else if (input == "Next*") {
    return PqlSuchthatType::kNextT;
  } else if (input == "Affects") {
    return PqlSuchthatType::kAffects;
  } else if (input == "Affects*") {
    return PqlSuchthatType::kAffectsT;
  } else {
    return PqlSuchthatType::kNone;
  }
}

PqlClauseType PqlSuchthat::GetClauseType() { return PqlClauseType::kSuchthat; }