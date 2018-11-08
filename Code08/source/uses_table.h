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
using VarIndex = int;
using VarIndexList = vector<VarIndex>;
using VarIndexSet = unordered_set<VarIndex>;
using ProcIndex = int;
using ProcIndexList = vector<ProcIndex>;
// using ProcNameIndexSet = unordered_set<ProcIndex>;
using ProcIndexSet = unordered_set<ProcIndex>;
// int can be StmtNum or ProcIndex
using UsesMap = unordered_map<int, VarIndexSet>;
using UsedByMap = unordered_map<VarIndex, unordered_set<int>>;
using StmtVarIndexPairList = vector<pair<StmtNum, VarIndex>>;
using StmtVarIndexPairSet = unordered_set<pair<StmtNum, VarIndex>>;
using ProcVarPairList = vector<pair<ProcIndex, VarIndex>>;
using ProcVarPairSet = unordered_set<pair<StmtNum, VarIndex>>;

// The uses table class for the PKB component
// Used to store uses relationships between stmt/proc and variables that are
// passed into PKB from the parser
class UsesTable {
  StmtNumList using_stmt_list_;
  ProcIndexList using_proc_list_;
  StmtNumSet using_stmt_set_;
  ProcIndexSet using_proc_set_;
  UsesMap uses_s_map_;
  UsedByMap used_by_s_map_;
  UsesMap uses_p_map_;
  UsedByMap used_by_p_map_;

 public:
  // Inserts a uses relationship between stmt_num and var_name in the
  // uses_s_map_ and used_by_s_map_
  void InsertUsesS(StmtNum stmt_num, VarIndex var_name_id);

  // Inserts a uses relationship between proc_name and var_name in the
  // uses_p_map_ and used_by_p_map_
  void InsertUsesP(ProcIndex proc_name_id, VarIndex var_name_id);

  // @returns a set of variables used in statement identified by stmt_num
  // (can be empty)
  VarIndexSet GetUsedVarS(StmtNum stmt_num);

  // @returns a set of variables used in procedure identified by proc_name
  // (can be empty)
  VarIndexSet GetUsedVarP(ProcIndex proc_name_id);

  // @returns a set of statements that use some variable (can be empty)
  StmtNumSet GetAllUsingStmt();

  // @returns a set of procedures that use some variable (can be empty)
  ProcIndexSet GetAllUsingProc();

  // @returns a set of statements that use var_name (can be empty)
  StmtNumSet GetUsingStmt(VarIndex var_name_id);

  // @returns a set of procedures that use var_name (can be empty)
  ProcIndexSet GetUsingProc(VarIndex var_name_id);

  // @returns true if var_name is used in statement with stmt_num
  bool IsUsedByS(StmtNum stmt_num, VarIndex var_name_id);

  // @returns true if var_name is used in procedure with proc_name
  bool IsUsedByP(ProcIndex proc_name_id, VarIndex var_name_id);

  // @returns a set of all <stmt_num, var_name> uses_pairs (can be empty)
  StmtVarIndexPairSet GetAllUsesSPair();

  // @returns a set of all <proc_name, var_name> uses_pairs (can be empty)
  ProcVarPairSet GetAllUsesPPair();
};

#endif !SPA_USES_TABLE_H