#pragma once

#ifndef SPA_USES_TABLE_H
#define SPA_USES_TABLE_H

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using std::list;
using std::make_pair;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::pair;
using std::vector;

using StmtNum = string;
using StmtList = vector<string>;
using VarName = string;
using VarNameList = vector<string>;
using UsesMap = unordered_map<string, vector<string>>;
using UsedByMap = unordered_map<string, vector<string>>;
using UsesStmtListMap = unordered_map<int, unordered_set<string>>;
using StmtVarPairList = vector<pair<string, string>>;
using StmtListIndex = int;
using VarNameSet = unordered_set<string>;

class UsesTable {
  StmtList using_stmts_list_;
  VarNameList used_vars_list_;
  UsesMap uses_map_;
  UsedByMap used_by_map_;
  UsesStmtListMap uses_stmt_list_map_;

public:
  // Inserts a uses relationship in the Uses Map and Used By Map.
  // @params variable being used, identified by var_name
  // @params statement using the var, identified by stmt_num
  // @params stmt_list_index of the statement
  // @returns true if insertion is successful
  // @returns false if no same relationship already exists in the map or otherwise
  bool InsertUses(VarName var_name, StmtNum stmt_num, StmtListIndex stmt_list_index);

  // Checks and returns all variables used in the program, if any.
  // @returns a list of variables used in the program (can be empty)
  VarNameList GetAllUsedVar();

  // Checks and returns all variables used in the given statement, if any.
  // @params statement number stmt_num
  // @returns a list of variables used in statement identified by stmt_num
  // (can be empty)
  VarNameList GetAllUsedVar(StmtNum stmt_num);

  // Returns a set of all variables used by statements in the given statement index
  // @params stmt list index
  // @returns a set of variables used by statements with the stmt list index
  // can be empty
  VarNameSet GetVarUsedByStmtList(StmtListIndex stmt_list_num);

  // Checks and returns all statements that are using any variables in the program, if any.
  // @returns a list of statement numbers of statements using a variable
  // in the program (can be empty)
  StmtList GetAllUsingStmt();

  // Checks and returns all statements that are using the given variable in the program, if any.
  // @params variable name var_name
  // @returns a list of statement numbers of statements using the variable var_name
  // in the program (can be empty)
  StmtList GetAllUsingStmt(VarName var_name);

  // Checks and returns whether the given variable is used in the statement identified by
  // the given statement number.
  // @params variable name var_name, statement number stmt_num
  // @returns true if var_name is used in statement with stmt_num
  // @returns false otherwise
  bool IsUsedBy(VarName var_name, StmtNum stmt_num);

  // Checks and returns whether the UsesTable has any Uses relationships.
  // @returns true if UsesTable is not empty
  // @returns false otherwise
  bool HasUsesRelationship();

  // Returns a list of <statement number, variable name> pairs in the Uses table.
  // @returns a list of <stmt_num, var_name> uses_pairs (can be empty)
  StmtVarPairList GetAllUsesPair();
};

#endif !SPA_USES_TABLE_H
