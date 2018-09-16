#pragma once

#ifndef PQL_SUCHTHAT_H
#define PQL_SUCHTHAT_H

#include <string>
using std::string;
using std::pair;

#include "pql_enum.h"

using Parameters = pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>>;

/* Stores information of a such that clause in a PQL select statement */
class PqlSuchthat {
private:
  /* The relationship type of the such that clause */
  PqlSuchthatType type_;
  /* The parameters, stored in the form of ((synonym, type), (synonym, type)) */
  Parameters parameters_;

public:
  PqlSuchthat(PqlSuchthatType, string, PqlDeclarationEntity, string, PqlDeclarationEntity);

  PqlSuchthatType GetType();
  Parameters GetParameters();
  static PqlSuchthatType StringToType(string);
};

#endif