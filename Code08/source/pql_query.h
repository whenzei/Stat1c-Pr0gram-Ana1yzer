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

#include "pql_clause.h"
#include "pql_suchthat.h"
#include "pql_pattern.h"
#include "pql_enum.h"

using Declarations = unordered_map<string, PqlDeclarationEntity>;
using EntitySet = unordered_set<PqlDeclarationEntity>;
using SuchthatParameters = pair<EntitySet, EntitySet>;
using SuchthatTable = unordered_map<PqlSuchthatType, SuchthatParameters>;
using PqlPatternTable = unordered_map<PqlSuchthatType, EntitySet>;

/*
SELECT SUCH THAT CLAUSE
For each suchthat types, the first and second parameters can only accept a certain type of PqlDeclarationEntity
For example, Follows(x, y), x can either be a kStmt, kRead, kPrint, ..., kUnderscore
*/

// Follows
const EntitySet follows_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};
const EntitySet follows_second = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};

// FollowsT
const EntitySet follows_t_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};
const EntitySet follows_t_second = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};

// Parent
const EntitySet parent_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger, PqlDeclarationEntity::kUnderscore
};
const EntitySet parent_second = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};

// ParentT
const EntitySet parent_t_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf, PqlDeclarationEntity::kProgline,
PqlDeclarationEntity::kInteger, PqlDeclarationEntity::kUnderscore
};
const EntitySet parent_t_second = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};

// UsesS
const EntitySet uses_s_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kPrint, PqlDeclarationEntity::kCall,
PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf, PqlDeclarationEntity::kAssign,
PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger
};
const EntitySet uses_s_second = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};

// UsesP
const EntitySet uses_p_first = {
PqlDeclarationEntity::kProcedure, PqlDeclarationEntity::kIdent
};
const EntitySet uses_p_second = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};

// ModifiesS
const EntitySet modifies_s_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kCall,
PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf, PqlDeclarationEntity::kAssign,
PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger
};
const EntitySet modifies_s_second = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};

// ModifiesP
const EntitySet modifies_p_first = {
PqlDeclarationEntity::kProcedure, PqlDeclarationEntity::kIdent
};
const EntitySet modifies_p_second = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};

// Group all the sets into a map for easy access
const SuchthatTable suchthat_table = {
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
SELECT PATTERN CLAUSE
For each pattern types, the first parameters can only accept a certain type of PqlDeclarationEntity
For example, Follows(x, y), x can either be a kStmt, kRead, kPrint, ..., kUnderscore
*/

// Assign
const EntitySet pattern_assign = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kUnderscore, PqlDeclarationEntity::kIdent
};

// While
const EntitySet pattern_while = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kConstant, PqlDeclarationEntity::kUnderscore, PqlDeclarationEntity::kIdent
};

// If
const EntitySet pattern_if = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kConstant, PqlDeclarationEntity::kUnderscore, PqlDeclarationEntity::kIdent
};

/*
This class stores the PQL query, including the declarations and
'Select' statement.
*/
class PqlQuery {
private:
  /* a map that maps the name to the entity type */
  Declarations declarations_;
  /* the variable name of the 'Select' statement */
  string var_name_;
  /* collection of such that clauses in the 'Select' statement */
  vector<PqlSuchthat> suchthats_;
  /* collection of pattern clauses in the 'Select' statement */
  vector<PqlPattern> patterns_;

public:
  /* Constructor */
  PqlQuery();

  /* Setters */
  bool AddDeclaration(PqlDeclarationEntity, string);
  void SetVarName(string);
  void AddSuchthat(PqlSuchthat);
  void AddPattern(PqlPattern);

  /* Getters */
  Declarations GetDeclarations();
  string GetVarName();
  vector<PqlSuchthat> GetSuchThats();
  vector<PqlPattern> GetPatterns();
  vector<PqlClause> GetClauses();
  static PqlDeclarationEntity DeclarationStringToType(string);
};

#endif  // !QUERY_H
