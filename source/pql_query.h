#pragma once

#ifndef PQL_QUERY_H
#define PQL_QUERY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::unordered_set;

#include "pql_suchthat.h"
#include "pql_enum.h"

/*
For each suchthat types, the first and second parameters can only accept a certain type(PqlDeclarationEntity)
For example, Follows(x, y), x can either be a kStmt, kRead, kPrint, ..., kUnderscore
*/

// Follows
const unordered_set<PqlDeclarationEntity> follows_first = { kStmt, kRead, kPrint, kCall, kWhile, kIf, kAssign, kProgline, kInteger, kUnderscore };
const unordered_set<PqlDeclarationEntity> follows_second = { kStmt, kRead, kPrint, kCall, kWhile, kIf, kAssign, kProgline, kInteger, kUnderscore };

// FollowsT
const unordered_set<PqlDeclarationEntity> follows_t_first = { kStmt, kRead, kPrint, kCall, kWhile, kIf, kAssign, kProgline, kInteger, kUnderscore };
const unordered_set<PqlDeclarationEntity> follows_t_second = { kStmt, kRead, kPrint, kCall, kWhile, kIf, kAssign, kProgline, kInteger, kUnderscore };

// Parent
const unordered_set<PqlDeclarationEntity> parent_first = { kStmt, kWhile, kIf, kProgline, kInteger, kUnderscore };
const unordered_set<PqlDeclarationEntity> parent_second = { kStmt, kRead, kPrint, kCall, kWhile, kIf, kAssign, kProgline, kInteger, kUnderscore };

// ParentT
const unordered_set<PqlDeclarationEntity> parent_t_first = { kStmt, kWhile, kIf, kProgline, kInteger, kUnderscore };
const unordered_set<PqlDeclarationEntity> parent_t_second = { kStmt, kRead, kPrint, kCall, kWhile, kIf, kAssign, kProgline, kInteger, kUnderscore };

// UsesS
const unordered_set<PqlDeclarationEntity> uses_s_first = { kStmt, kPrint, kCall, kWhile, kIf, kAssign, kProgline, kInteger };
const unordered_set<PqlDeclarationEntity> uses_s_second = { kVariable, kConstant, kIdent, kUnderscore };

// UsesP
const unordered_set<PqlDeclarationEntity> uses_p_first = { kProcedure, kIdent };
const unordered_set<PqlDeclarationEntity> uses_p_second = { kVariable, kConstant, kIdent, kUnderscore };

// ModifiesS
const unordered_set<PqlDeclarationEntity> modifies_s_first = { kStmt, kRead, kCall, kWhile, kIf, kAssign, kProgline, kInteger };
const unordered_set<PqlDeclarationEntity> modifies_s_second = { kVariable, kIdent, kUnderscore };

// ModifiesP
const unordered_set<PqlDeclarationEntity> modifies_p_first = { kProcedure, kIdent };
const unordered_set<PqlDeclarationEntity> modifies_p_second = { kVariable, kIdent, kUnderscore };

// Group all the sets into a map for easy access
const unordered_map<PqlSuchthatType, pair<unordered_set<PqlDeclarationEntity>, unordered_set<PqlDeclarationEntity>>> suchthat_table = {
  {PqlSuchthatType::kFollows, std::make_pair(follows_first, follows_second)},
  {PqlSuchthatType::kFollowsT, std::make_pair(follows_t_first, follows_t_second)},
  {PqlSuchthatType::kParent, std::make_pair(parent_first, parent_second)},
  {PqlSuchthatType::kParentT, std::make_pair(parent_t_first, parent_t_second)},
  {PqlSuchthatType::kUsesS, std::make_pair(uses_s_first, uses_s_second)},
  {PqlSuchthatType::kUsesP, std::make_pair(uses_p_first, uses_p_second)},
  {PqlSuchthatType::kModifiesS, std::make_pair(modifies_s_first, modifies_s_second)},
  {PqlSuchthatType::kModifiesP, std::make_pair(modifies_p_first, modifies_p_second)},
};

/*
This class parses and stores the PQL query, including the declarations and
'Select' statement.
*/
class PqlQuery {
 private:
  unordered_map<string, PqlDeclarationEntity> declarations_; // the list of declarations
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
  unordered_map<string, PqlDeclarationEntity> GetDeclarations();
  string GetVarName();
  vector<PqlSuchthat> GetSuchThats();
};

#endif  // !QUERY_H
