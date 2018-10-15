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
#include "pql_with.h"
#include "pql_global.h"

using Declarations = unordered_map<string, PqlDeclarationEntity>;
using EntitySet = unordered_set<PqlDeclarationEntity>;
using SuchthatParameters = pair<EntitySet, EntitySet>;
using SuchthatTable = unordered_map<PqlSuchthatType, SuchthatParameters>;
using WithTable = unordered_map<PqlDeclarationEntity, EntitySet>;

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

// Calls
const EntitySet calls_first = {
PqlDeclarationEntity::kProcedure, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};
const EntitySet calls_second = {
PqlDeclarationEntity::kProcedure, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};

// CallsT
const EntitySet calls_t_first = {
PqlDeclarationEntity::kProcedure, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};
const EntitySet calls_t_second = {
PqlDeclarationEntity::kProcedure, PqlDeclarationEntity::kIdent, PqlDeclarationEntity::kUnderscore
};

// Next
const EntitySet next_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};
const EntitySet next_second = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};

// NextT
const EntitySet next_t_first = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};
const EntitySet next_t_second = {
PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf,
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kInteger,
PqlDeclarationEntity::kUnderscore
};

// Affects
const EntitySet affects_first = {
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kUnderscore, PqlDeclarationEntity::kInteger
};
const EntitySet affects_second = {
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kUnderscore, PqlDeclarationEntity::kInteger
};

// AffectsT
const EntitySet affects_t_first = {
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kUnderscore, PqlDeclarationEntity::kInteger
};
const EntitySet affects_t_second = {
PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kUnderscore, PqlDeclarationEntity::kInteger
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
  {PqlSuchthatType::kCalls, std::make_pair(calls_first, calls_second)},
  {PqlSuchthatType::kCallsT, std::make_pair(calls_t_first, calls_t_second)},
  {PqlSuchthatType::kNext, std::make_pair(next_first, next_second)},
  {PqlSuchthatType::kNextT, std::make_pair(next_t_first, next_t_second)},
  {PqlSuchthatType::kAffects, std::make_pair(affects_first, affects_second)},
  {PqlSuchthatType::kAffectsT, std::make_pair(affects_t_first, affects_t_second)}
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
SELECT WITH CLAUSE
The first parameters can only be compatible to a certain type in the second parameter
For example, a1.stmt# = c.value is valid
*/

const EntitySet number = {
PqlDeclarationEntity::kProgline, PqlDeclarationEntity::kStmt, PqlDeclarationEntity::kRead, PqlDeclarationEntity::kPrint,
PqlDeclarationEntity::kWhile, PqlDeclarationEntity::kIf, PqlDeclarationEntity::kAssign, PqlDeclarationEntity::kConstant,
PqlDeclarationEntity::kCall, PqlDeclarationEntity::kInteger
};

const EntitySet name = {
PqlDeclarationEntity::kVariable, PqlDeclarationEntity::kProcedure, PqlDeclarationEntity::kCallName, PqlDeclarationEntity::kIdent
};

const WithTable with_table = {
{PqlDeclarationEntity::kProgline, number},
{PqlDeclarationEntity::kStmt, number},
{PqlDeclarationEntity::kRead, number},
{PqlDeclarationEntity::kPrint, number},
{PqlDeclarationEntity::kWhile, number},
{PqlDeclarationEntity::kIf, number},
{PqlDeclarationEntity::kAssign, number},
{PqlDeclarationEntity::kCall, number},
{PqlDeclarationEntity::kConstant, number},
{PqlDeclarationEntity::kVariable, name},
{PqlDeclarationEntity::kCallName, name},
{PqlDeclarationEntity::kProcedure, name}
};

/*
This class stores the PQL query, including the declarations and
'Select' statement.
*/
class PqlQuery {
private:
  /* a map that maps the name to the entity type */
  Declarations declarations_;
  /* collect of selection */
  vector<Synonym> selections_;
  /* collection of clauses in the 'Select' statement */
  vector<PqlClause*> clauses_;

  /* LEGACY: TO BE DELETED */
  string var_name_;
  vector<PqlSuchthat> suchthats_;
  vector<PqlPattern> patterns_;
  /* LEGACY: TO BE DELETED */

public:
  /* Constructor */
  PqlQuery();

  /* Setters */
  bool AddDeclaration(PqlDeclarationEntity, string);
  void AddSelection(string, PqlDeclarationEntity);
  void AddClause(PqlClause*);

  /* Getters */
  Declarations GetDeclarations();
  vector<Synonym> GetSelections();
  vector<PqlClause*> GetClauses();

  static PqlDeclarationEntity DeclarationStringToType(string);

  /* LEGACY: TO BE DELETED */
  void SetVarName(string);
  string GetVarName();
  void AddSuchthat(PqlSuchthat);
  void AddPattern(PqlPattern);
  vector<PqlSuchthat> GetSuchThats();
  vector<PqlPattern> GetPatterns();
  /* LEGACY: TO BE DELETED */
};

#endif  // !QUERY_H
