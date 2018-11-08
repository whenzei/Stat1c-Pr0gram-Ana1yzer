#pragma once

#include "uses_table.h"

void UsesTable::InsertUsesS(StmtNum stmt_num, VarIndex var_name_id) {
  using_stmt_set_.insert(stmt_num);
  uses_s_map_[stmt_num].insert(var_name_id);
  used_by_s_map_[var_name_id].insert(stmt_num);
}

void UsesTable::InsertUsesP(ProcIndex proc_name_id, VarIndex var_name_id) {
  using_proc_set_.insert(proc_name_id);
  uses_p_map_[proc_name_id].insert(var_name_id);
  used_by_p_map_[var_name_id].insert(proc_name_id);
}

VarIndexSet UsesTable::GetUsedVarS(StmtNum stmt_num) {
  if (uses_s_map_.count(stmt_num)) {
    return uses_s_map_[stmt_num];
  }

  return VarIndexSet();
}

VarIndexSet UsesTable::GetUsedVarP(ProcIndex proc_name_id) {
  VarIndexSet used_vars;
  if (uses_p_map_.find(proc_name_id) != uses_p_map_.end()) {
    used_vars = uses_p_map_[proc_name_id];
  }
  return used_vars;
}

StmtNumSet UsesTable::GetAllUsingStmt() { return using_stmt_set_; }

ProcIndexSet UsesTable::GetAllUsingProc() { return using_proc_set_; }

StmtNumSet UsesTable::GetUsingStmt(VarIndex var_name_id) {
  StmtNumSet using_stmts;
  if (used_by_s_map_.find(var_name_id) != used_by_s_map_.end()) {
    using_stmts = used_by_s_map_[var_name_id];
  }
  return using_stmts;
}

ProcIndexSet UsesTable::GetUsingProc(VarIndex var_name_id) {
  ProcIndexSet using_proc;
  if (used_by_p_map_.find(var_name_id) != used_by_p_map_.end()) {
    using_proc = used_by_p_map_[var_name_id];
  }
  return using_proc;
}

bool UsesTable::IsUsedByS(StmtNum stmt_num, VarIndex var_name_id) {
  if (uses_s_map_.find(stmt_num) != uses_s_map_.end()) {
    VarIndexSet var_name_set = (*uses_s_map_.find(stmt_num)).second;
    return var_name_set.find(var_name_id) != var_name_set.end();
  } else {
    return false;
  }
}

bool UsesTable::IsUsedByP(ProcIndex proc_name_id, VarIndex var_name_id) {
  if (uses_p_map_.find(proc_name_id) != uses_p_map_.end()) {
    VarIndexSet var_name_set = (*uses_p_map_.find(proc_name_id)).second;
    return find(var_name_set.begin(), var_name_set.end(), var_name_id) !=
      var_name_set.end();
  } else {
    return false;
  }
}

StmtVarPairSet UsesTable::GetAllUsesSPair() {
  StmtVarPairSet uses_pairs;
  for (auto entry : uses_s_map_) {
    for (auto var_name : entry.second) {
      uses_pairs.insert(make_pair(entry.first, var_name));
    }
  }
  return uses_pairs;
}

ProcVarPairSet UsesTable::GetAllUsesPPair() {
  ProcVarPairSet uses_pairs;
  for (auto entry : uses_p_map_) {
    for (auto var_name : entry.second) {
      uses_pairs.insert(make_pair(entry.first, var_name));
    }
  }
  return uses_pairs;
}
