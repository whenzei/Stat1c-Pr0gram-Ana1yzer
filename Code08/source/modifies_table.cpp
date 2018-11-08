#pragma once

#include "modifies_table.h"

using std::find;
using std::make_pair;

void ModifiesTable::InsertModifiesS(StmtNum stmt_num, VarIndex var_id) {
  modifying_stmt_set_.insert(stmt_num);
  modifies_s_map_[stmt_num].insert(var_id);
  modified_by_s_map_[var_id].insert(stmt_num);
}

void ModifiesTable::InsertModifiesP(ProcIndex proc_id,
                                    VarIndex var_id) {
  modifying_proc_set_.insert(proc_id);
  modifies_p_map_[proc_id].insert(var_id);
  modified_by_p_map_[var_id].insert(proc_id);
}

bool ModifiesTable::IsModifiedByS(StmtNum stmt_num, VarIndex var_id) {
  ModifiesMap::iterator iter = modifies_s_map_.find(stmt_num);
  if (iter != modifies_s_map_.end()) {
    return (*iter).second.count(var_id);
  } else {
    return false;
  }
}

bool ModifiesTable::IsModifiedByP(ProcIndex proc_id,
                                  VarIndex var_id) {
  ModifiesMap::iterator iter = modifies_p_map_.find(proc_id);
  if (iter != modifies_p_map_.end()) {
    return (*iter).second.count(var_id);
  } else {
    return false;
  }
}

VarIndexSet ModifiesTable::GetModifiedVarS(StmtNum stmt_num) {
  ModifiesMap::iterator iter = modifies_s_map_.find(stmt_num);
  if (iter != modifies_s_map_.end()) {
    return (*iter).second;
  } else {
    return VarIndexSet();
  }
}

VarIndexSet ModifiesTable::GetModifiedVarP(ProcIndex proc_id) {
  ModifiesMap::iterator iter = modifies_p_map_.find(proc_id);
  if (iter != modifies_p_map_.end()) {
    return (*iter).second;
  } else {
    return VarIndexSet();
  }
}

StmtNumSet ModifiesTable::GetModifyingStmt(VarIndex var_id) {
  ModifiedByMap::iterator iter = modified_by_s_map_.find(var_id);
  if (iter != modified_by_s_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtNumSet ModifiesTable::GetAllModifyingStmt() {
  return modifying_stmt_set_;
}

ProcIndexSet ModifiesTable::GetModifyingProc(VarIndex var_id) {
  ModifiedByMap::iterator iter = modified_by_p_map_.find(var_id);
  if (iter != modified_by_p_map_.end()) {
    return (*iter).second;
  } else {
    return ProcIndexSet();
  }
}

ProcIndexSet ModifiesTable::GetAllModifyingProc() {
  return modifying_proc_set_;
}

StmtVarPairSet ModifiesTable::GetAllModifiesPairS() {
  StmtVarPairSet modifies_pair_set;
  for (auto& entry : modifies_s_map_) {
    for (auto& var_id : entry.second) {
      modifies_pair_set.insert(make_pair(entry.first, var_id));
    }
  }
  return modifies_pair_set;
}

ProcVarPairSet ModifiesTable::GetAllModifiesPairP() {
  ProcVarPairSet modifies_pair_set;
  for (auto& entry : modifies_p_map_) {
    for (auto& var_id : entry.second) {
      modifies_pair_set.insert(make_pair(entry.first, var_id));
    }
  }
  return modifies_pair_set;
}
