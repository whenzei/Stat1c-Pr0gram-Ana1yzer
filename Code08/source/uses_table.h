#pragma once

#ifndef SPA_USES_TABLE_H
#define SPA_USES_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::list;
using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = string;
using StmtNumList = vector<string>;
using StmtNumSet = unordered_set<string>;
using VarName = string;
using VarNameList = vector<string>;
using VarNameSet = unordered_set<string>;
using ProcName = string;
using ProcNameList = vector<string>;
using ProcNameSet = unordered_set<string>;
using UsesMap = unordered_map<string, vector<string>>;
using UsedByMap = unordered_map<string, vector<string>>;
using StmtVarPairList = vector<pair<string, string>>;
using ProcVarPairList = vector<pair<string, string>>;

class UsesTable {
  StmtNumList using_stmt_list_;
  VarNameList used_var_list_;
  ProcNameList using_proc_list_;
  StmtNumSet using_stmt_set_;
  VarNameSet used_var_set_;
  ProcNameSet using_proc_set_;
  UsesMap uses_s_map_;
  UsedByMap used_by_s_map_;
  UsesMap uses_p_map_;
  UsedByMap used_by_p_map_;

 public:
  // Inserts a uses relationship in the Uses Map and Used By Map.
  // @params variable being used, identified by var_name
  // @params statement using the var, identified by stmt_num
  // @returns true if insertion is successful
  // @returns false if no same relationship already exists in the map or
  // otherwise
  void InsertUsesS(StmtNum stmt_num, VarName var_name);

  void InsertUsesP(ProcName proc_name, VarName var_name);

  // Checks and returns all variables used in the program, if any.
  // @returns a list of variables used in the program (can be empty)
  VarNameList GetAllUsedVar();

  // Checks and returns all variables used in the given statement, if any.
  // @params statement number stmt_num
  // @returns a list of variables used in statement identified by stmt_num
  // (can be empty)
  VarNameList GetUsedVarS(StmtNum stmt_num);

  VarNameList GetUsedVarP(ProcName proc_name);

  // Checks and returns all statements that are using any variables in the
  // program, if any.
  // @returns a list of statement numbers of statements using a variable
  // in the program (can be empty)
  StmtNumList GetAllUsingStmt();

  ProcNameList GetAllUsingProc();

  // Checks and returns all statements that are using the given variable in the
  // program, if any.
  // @params variable name var_name
  // @returns a list of statement numbers of statements using the variable
  // var_name in the program (can be empty)
  StmtNumList GetUsingStmt(VarName var_name);

  ProcNameList GetUsingProc(VarName var_name);

  // Checks and returns whether the given variable is used in the statement
  // identified by the given statement number.
  // @params variable name var_name, statement number stmt_num
  // @returns true if var_name is used in statement with stmt_num
  // @returns false otherwise
  bool IsUsedByS(StmtNum stmt_num, VarName var_name);

  bool IsUsedByP(ProcName proc_name, VarName var_name);

  // Checks and returns whether the UsesTable has any Uses relationships.
  // @returns true if UsesTable is not empty
  // @returns false otherwise
  bool HasUsesRelationship();

  // Returns a list of <statement number, variable name> pairs in the Uses
  // table.
  // @returns a list of <stmt_num, var_name> uses_pairs (can be empty)
  StmtVarPairList GetAllUsesSPair();

  ProcVarPairList GetAllUsesPPair();
};

#endif !SPA_USES_TABLE_H
