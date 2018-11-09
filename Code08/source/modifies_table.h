#pragma once

#ifndef SPA_MODIFIES_TABLE_H
#define SPA_MODIFIES_TABLE_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "pair_hash.h"

using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = int;
using StmtNumSet = unordered_set<StmtNum>;
using VarIndex = int;
using VarIndexSet = unordered_set<VarIndex>;
using ProcIndex = int;
using ProcIndexSet = unordered_set<ProcIndex>;
// int can be either ProcName or StmtNum.
using ModifiesMap = unordered_map<StmtNum, VarIndexSet>;
using ModifiedByMap = unordered_map<VarIndex, StmtNumSet>;
using StmtVarPairSet = unordered_set<pair<StmtNum, VarIndex>, pair_hash>;
using ProcVarPairSet = unordered_set<pair<ProcIndex, VarIndex>, pair_hash>;

// The modifies table class for the PKB component
// Used to store modifies relationships between stmt/proc and variables that are
// passed into PKB from the parser
class ModifiesTable {
  StmtNumSet modifying_stmt_set_;
  ProcIndexSet modifying_proc_set_;
  ModifiesMap modifies_s_map_;
  ModifiedByMap modified_by_s_map_;
  ModifiesMap modifies_p_map_;
  ModifiedByMap modified_by_p_map_;

 public:
  // inserts a modifies relationship between stmt_num and var_id into
  // modifies_s_map_ and modified_by_s_map
  void InsertModifiesS(StmtNum stmt_num, VarIndex var_id);

  // inserts a modifies relationship between proc_id and var_id into
  // modifies_p_map_ and modified_by_p_map
  void InsertModifiesP(ProcIndex proc_id, VarIndex var_id);

  // @returns true if var_id is modified in stmt_num
  bool IsModifiedByS(StmtNum stmt_num, VarIndex var_id);

  // @returns true if var_id is modified in proc_id
  bool IsModifiedByP(ProcIndex proc_id, VarIndex var_id);

  // @returns a list of variables modified in stmt_num
  VarIndexSet GetModifiedVarS(StmtNum stmt_num);

  // @returns a list of variables modified in proc_id
  VarIndexSet GetModifiedVarP(ProcIndex proc_id);

  // @returns a list of statements that modify var_id
  StmtNumSet GetModifyingStmt(VarIndex var_id);

  // @returns a list of statements that modify some variable
  StmtNumSet GetAllModifyingStmt();

  // @returns a list of procedures that modify var_id
  ProcIndexSet GetModifyingProc(VarIndex var_id);

  // @returns a list of procedures that modify some variable
  ProcIndexSet GetAllModifyingProc();

  // @returns a list of all pairs of <modifying_stmt_num, modified_var_id>
  StmtVarPairSet GetAllModifiesPairS();

  // @returns a list of all pairs of <modifying_proc_id, modified_var_id>
  ProcVarPairSet GetAllModifiesPairP();
};

#endif !SPA_MODIFIES_TABLE_H