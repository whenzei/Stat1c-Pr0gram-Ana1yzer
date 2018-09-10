#include "pql_suchthat.h"

PqlSuchthat::PqlSuchthat(PqlSuchthatType type, string first, string second) {
  type_ = type;
  first_ = first;
  second_ = second;
}

PqlSuchthatType PqlSuchthat::GetType() { return type_; }

string PqlSuchthat::GetFirst() { return first_; }

string PqlSuchthat::GetSecond() { return second_; }

pair<string, string> PqlSuchthat::GetParams() { return std::make_pair(first_, second_); }