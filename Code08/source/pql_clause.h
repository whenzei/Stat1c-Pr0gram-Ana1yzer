#pragma once

#ifndef PQL_CLAUSE
#define PQL_CLAUSE

#include "pql_global.h"


const int PRIORITY_CONSTANT_AND_SYNONYM = 10;
const int PRIORITY_FOLLOWS = 9;
const int PRIORITY_FOLLOWS_T = 8;
const int PRIORITY_MODIFIES = 7;
const int PRIORITY_WITH = 6;
const int PRIORITY_NORMAL = 5;
const int PRIORITY_AFFECTS = 4;
const int PRIORITY_AFFECTS_T = 3;

class PqlClause {
 public:
  virtual PqlClauseType GetClauseType() = 0;

  int GetPriority();
  void SetPriority(int);

  pair<string, string> GetSynonyms();
  void SetSynonyms(string, string);

protected:
  int priority_ = PRIORITY_NORMAL;
  pair<string, string> synonyms_;
};

#endif