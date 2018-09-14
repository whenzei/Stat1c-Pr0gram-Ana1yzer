#pragma once

#ifndef PQL_SUCHTHAT_H
#define PQL_SUCHTHAT_H

#include <string>
using std::string;
using std::pair;

using Parameters = pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>>;

#include "pql_enum.h"

class PqlSuchthat {
  private:
    PqlSuchthatType type_;
    Parameters parameters_; // ((synonym, type), (synonym, type))

  public:
    PqlSuchthat(PqlSuchthatType, string, PqlDeclarationEntity, string, PqlDeclarationEntity);

    PqlSuchthatType GetType();
    Parameters GetParameters();
    static PqlSuchthatType StringToType(string);
};

#endif