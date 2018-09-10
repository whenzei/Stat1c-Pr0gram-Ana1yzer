#pragma once

#ifndef PQL_SUCHTHAT_H
#define PQL_SUCHTHAT_H

#include <string>
using std::string;
using std::pair;

enum PqlSuchthatType {
  kModifiesP = 0,
  kModifiesS,
  kUsesP,
  kUsesS,
  Calls,
  CallsT,
  Parent,
  ParentT,
  Follows,
  FollowsT,
  Next,
  NextT,
  Affects,
  AffectsT
};

class PqlSuchthat {
  private:
    PqlSuchthatType type_;
    // parameters
    string first_;
    string second_;

  public:
    PqlSuchthat(PqlSuchthatType, string, string);

    PqlSuchthatType GetType();
    string GetFirst();
    string GetSecond();
    pair<string, string> GetParams(); // returns first_ and second_ as a pair object
};

#endif