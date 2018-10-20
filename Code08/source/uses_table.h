#pragma once

#ifndef SPA_USES_TABLE_H
#define SPA_USES_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = int;
using StmtNumList = vector<StmtNum>;
using StmtNumSet = unordered_set<StmtNum>;
using VarName = int;
using VarNameList = vector<VarName>;
using ProcName = int;
using ProcNameList = vector<ProcName>;
using ProcNameSet = unordered_set<ProcName>;
using UsesMap = unordered_map<StmtNum, vector<VarName>>;
using UsesProcMap = unordered_map<ProcName, vector<VarName>>; // Proc, List of Var
using UsedByMap = unordered_map<VarName, vector<StmtNum>>;
using UsedByProcMap = unordered_map<VarName, vector<ProcName>>; // Var, List of Proc
using StmtVarPairList = vector<pair<StmtNum, VarName>>;
using ProcVarPairList = vector<pair<ProcName, VarName>>;

// The uses table class for the PKB component
// Used to store uses relationships between stmt/proc and variables that are
// passed into PKB from the parser
class UsesTable {
  StmtNumList using_stmt_list_;
  ProcNameList using_proc_list_;
  StmtNumSet using_stmt_set_;
  ProcNameSet using_proc_set_;
  UsesMap uses_s_map_;
  UsedByMap used_by_s_map_;
  UsesProcMap uses_p_map_;
  UsedByProcMap used_by_p_map_;

 public:
  // Inserts a uses relationship between stmt_num and var_name in the
  // uses_s_map_ and used_by_s_map_
  void InsertUsesS(StmtNum stmt_num, VarName var_name);

  // Inserts a uses relationship between proc_name and var_name in the
  // uses_p_map_ and used_by_p_map_
  void InsertUsesP(ProcName proc_name, VarName var_name);

  // @returns a list of variables used in statement identified by stmt_num
  // (can be empty)
  VarNameList GetUsedVarS(StmtNum stmt_num);

  // @returns a list of variables used in procedure identified by proc_name
  // (can be empty)
  VarNameList GetUsedVarP(ProcName proc_name);

  // @returns a list of statements that use some variable (can be empty)
  StmtNumList GetAllUsingStmt();

  // @returns a list of procedures that use some variable (can be empty)
  ProcNameList GetAllUsingProc();

  // @returns a list of statements that use var_name (can be empty)
  StmtNumList GetUsingStmt(VarName var_name);

  // @returns a list of procedures that use var_name (can be empty)
  ProcNameList GetUsingProc(VarName var_name);

  // @returns true if var_name is used in statement with stmt_num
  bool IsUsedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if var_name is used in procedure with proc_name
  bool IsUsedByP(ProcName proc_name, VarName var_name);

  // @returns a list of all <stmt_num, var_name> uses_pairs (can be empty)
  StmtVarPairList GetAllUsesSPair();

  // @returns a list of all <proc_name, var_name> uses_pairs (can be empty)
  ProcVarPairList GetAllUsesPPair();
};

#endif !SPA_USES_TABLE_H
