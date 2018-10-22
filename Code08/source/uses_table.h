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
using VarNameIndex = int;
using VarNameIndexList = vector<VarNameIndex>;
using ProcNameIndex = int;
using ProcNameIndexList = vector<ProcNameIndex>;
using ProcNameIndexSet = unordered_set<ProcNameIndex>;
// int can be StmtNum or ProcNameIndex
using UsesMap = unordered_map<int, vector<VarNameIndex>>;
using UsedByMap = unordered_map<VarNameIndex, vector<int>>;
using StmtVarPairList = vector<pair<StmtNum, VarNameIndex>>;
using ProcVarPairList = vector<pair<ProcNameIndex, VarNameIndex>>;

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
  UsesMap uses_p_map_;
  UsedByMap used_by_p_map_;

 public:
  // Inserts a uses relationship between stmt_num and var_name in the
  // uses_s_map_ and used_by_s_map_
  void InsertUsesS(StmtNum stmt_num, VarNameIndex var_name_id);

  // Inserts a uses relationship between proc_name and var_name in the
  // uses_p_map_ and used_by_p_map_
  void InsertUsesP(ProcNameIndex proc_name_id, VarNameIndex var_name_id);

  // @returns a list of variables used in statement identified by stmt_num
  // (can be empty)
  VarNameIndexList GetUsedVarS(StmtNum stmt_num);

  // @returns a list of variables used in procedure identified by proc_name
  // (can be empty)
  VarNameIndexList GetUsedVarP(ProcNameIndex proc_name_id);

  // @returns a list of statements that use some variable (can be empty)
  StmtNumList GetAllUsingStmt();

  // @returns a list of procedures that use some variable (can be empty)
  ProcNameIndexList GetAllUsingProc();

  // @returns a list of statements that use var_name (can be empty)
  StmtNumList GetUsingStmt(VarNameIndex var_name_id);

  // @returns a list of procedures that use var_name (can be empty)
  ProcNameIndexList GetUsingProc(VarNameIndex var_name_id);

  // @returns true if var_name is used in statement with stmt_num
  bool IsUsedByS(StmtNum stmt_num, VarNameIndex var_name_id);

  // @returns true if var_name is used in procedure with proc_name
  bool IsUsedByP(ProcNameIndex proc_name_id, VarNameIndex var_name_id);

  // @returns a list of all <stmt_num, var_name> uses_pairs (can be empty)
  StmtVarPairList GetAllUsesSPair();

  // @returns a list of all <proc_name, var_name> uses_pairs (can be empty)
  ProcVarPairList GetAllUsesPPair();
};

#endif !SPA_USES_TABLE_H
