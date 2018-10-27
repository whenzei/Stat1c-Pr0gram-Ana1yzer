#pragma once

#ifndef PQL_SUCHTHAT_H
#define PQL_SUCHTHAT_H

#include <string>
#include "pql_clause.h"
using std::pair;
using std::string;

#include "pql_global.h"

/* Stores information of a such that clause in a PQL select statement */
class PqlSuchthat : public PqlClause {
 private:
  /* The relationship type of the such that clause */
  PqlSuchthatType type_;
  /* The parameters, stored in the form of ((synonym, type), (synonym, type)) */
  Parameters parameters_;

 public:
  PqlSuchthat(PqlSuchthatType, string, PqlDeclarationEntity, string,
              PqlDeclarationEntity);

  // @returns the type of the "such that" clause
  PqlSuchthatType GetType();

  // @returns the parameters of the clause
  Parameters GetParameters();

  // @returns a PqlSuchThatType enum from the given string
  static PqlSuchthatType StringToType(string);

  // @returns the type of the clause
  PqlClauseType GetClauseType();
};

#endif