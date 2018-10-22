#pragma once

#ifndef SPA_MODIFIES_TABLE_H
#define SPA_MODIFIES_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "var_list.h"
#include "proc_list.h"

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
using ProcIndex = int;
using ProcIndexList = vector<ProcIndex>;
using ProcNameIndexSet = unordered_set<ProcIndex>;
// int can be either ProcName or StmtNum.
using ModifiesMap = unordered_map<int, vector<VarIndex>>;
using ModifiedByMap = unordered_map<VarIndex, vector<int>>;
using StmtVarPairList = vector<pair<StmtNum, VarIndex>>;
using ProcVarPairList = vector<pair<ProcIndex, VarIndex>>;

// The modifies table class for the PKB component
// Used to store modifies relationships between stmt/proc and variables that are
// passed into PKB from the parser
class ModifiesTable {
  StmtNumList modifying_stmt_list_;
  StmtNumSet modifying_stmt_set_;
  ProcIndexList modifying_proc_list_;
  ProcNameIndexSet modifying_proc_set_;
  ModifiesMap modifies_s_map_;
  ModifiedByMap modified_by_s_map_;
  ModifiesMap modifies_p_map_;
  ModifiedByMap modified_by_p_map_;
  ProcList proc_list_;
  VarList var_list_;

 public:
  // inserts a modifies relationship between stmt_num and var_name into
  // modifies_s_map_ and modified_by_s_map
  void InsertModifiesS(StmtNum stmt_num, VarName var_name);

  // inserts a modifies relationship between proc_name and var_name into
  // modifies_p_map_ and modified_by_p_map
  void InsertModifiesP(ProcName proc_name, VarName var_name);

  // @returns true if var_name is modified in stmt_num
  bool IsModifiedByS(StmtNum stmt_num, VarIndex var_name_id);

  // @returns true if var_name is modified in proc_name
  bool IsModifiedByP(ProcIndex proc_name_id, VarIndex var_name_id);

  // @returns a list of variables modified in stmt_num
  VarIndexList GetModifiedVarS(StmtNum stmt_num);

  // @returns a list of variables modified in proc_name
  VarIndexList GetModifiedVarP(ProcIndex proc_name_id);

  // @returns a list of statements that modify var_name
  StmtNumList GetModifyingStmt(VarIndex var_name_id);

  // @returns a list of statements that modify some variable
  StmtNumList GetAllModifyingStmt();

  // @returns a list of procedures that modify var_name
  ProcIndexList GetModifyingProc(VarIndex var_name_id);

  // @returns a list of procedures that modify some variable
  ProcIndexList GetAllModifyingProc();

  // @returns a list of all pairs of <modifying_stmt_num, modified_var_name>
  StmtVarPairList GetAllModifiesPairS();

  // @returns a list of all pairs of <modifying_proc_name, modified_var_name>
  ProcVarPairList GetAllModifiesPairP();
};

#endif !SPA_MODIFIES_TABLE_H