#pragma once

#ifndef PQL_QUERY_H
#define PQL_QUERY_H

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

#include "pql_suchthat.h"

enum PqlDeclarationEntity {
  kStmt = 0,
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssign,
  kVariable,
  kConstant,
  kProgline,
  kProcedure
};

/*
This class parses and stores the PQL query, including the declarations and
'Select' statement.
*/
class PqlQuery {
 private:
  map<string, PqlDeclarationEntity> declarations_; // the list of declarations
  string var_name_;  // the variable name of the 'Select' statement
  vector<PqlSuchthat> suchthats_; // collection of such that clauses

 public:
  /* Constructor */
  PqlQuery();

  /* Setters */
  bool AddDeclaration(PqlDeclarationEntity, string);
  void SetVarName(string);
  void AddSuchthat(PqlSuchthat);

  /* Getters */
  map<string, PqlDeclarationEntity> GetDeclarations();
  string GetVarName();
  vector<PqlSuchthat> GetSuchThats();
};

#endif  // !QUERY_H
