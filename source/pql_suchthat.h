#pragma once

#ifndef PQL_SUCHTHAT_H
#define PQL_SUCHTHAT_H

#include <string>
using std::string;
using std::pair;

#include "pql_enum.h"

class PqlSuchthat {
  private:
    PqlSuchthatType type_;
    pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>> parameters_; // ((synonym, type), (synonym, type))

  public:
    PqlSuchthat(PqlSuchthatType, string, PqlDeclarationEntity, string, PqlDeclarationEntity);

    PqlSuchthatType GetType();
    pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>> GetParameters();
};

#endif