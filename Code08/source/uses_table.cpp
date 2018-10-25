#pragma once

#include "uses_table.h"

void UsesTable::InsertUsesS(StmtNum stmt_num, VarIndex var_name_id) {
  if (!IsUsedByS(stmt_num, var_name_id)) {
    if (using_stmt_set_.insert(stmt_num).second) {
      using_stmt_list_.push_back(stmt_num);
    }
    uses_s_map_[stmt_num].push_back(var_name_id);
    used_by_s_map_[var_name_id].push_back(stmt_num);
  }
}

void UsesTable::InsertUsesP(ProcIndex proc_name_id, VarIndex var_name_id) {
  if (!IsUsedByP(proc_name_id, var_name_id)) {
    if (using_proc_set_.insert(proc_name_id).second) {
      using_proc_list_.push_back(proc_name_id);
    }
    uses_p_map_[proc_name_id].push_back(var_name_id);
    used_by_p_map_[var_name_id].push_back(proc_name_id);
  }
}

VarIndexList UsesTable::GetUsedVarS(StmtNum stmt_num) {
  VarIndexList used_vars;
  if (uses_s_map_.find(stmt_num) != uses_s_map_.end()) {
    used_vars = uses_s_map_[stmt_num];
  }
  return used_vars;
}

VarIndexList UsesTable::GetUsedVarP(ProcIndex proc_name_id) {
  VarIndexList used_vars;
  if (uses_p_map_.find(proc_name_id) != uses_p_map_.end()) {
    used_vars = uses_p_map_[proc_name_id];
  }
  return used_vars;
}

StmtNumList UsesTable::GetAllUsingStmt() {
  return using_stmt_list_; 
}

ProcIndexList UsesTable::GetAllUsingProc() {
  return using_proc_list_;
}

StmtNumList UsesTable::GetUsingStmt(VarIndex var_name_id) {
  StmtNumList using_stmts;
  if (used_by_s_map_.find(var_name_id) != used_by_s_map_.end()) {
    using_stmts = used_by_s_map_[var_name_id];
  }
  return using_stmts;
}

ProcIndexList UsesTable::GetUsingProc(VarIndex var_name_id) {
  ProcIndexList using_proc;
  if (used_by_p_map_.find(var_name_id) != used_by_p_map_.end()) {
    using_proc = used_by_p_map_[var_name_id];
  }
  return using_proc;
}

bool UsesTable::IsUsedByS(StmtNum stmt_num, VarIndex var_name_id) {
  if (uses_s_map_.find(stmt_num) != uses_s_map_.end()) {
    VarIndexList var_name_list = (*uses_s_map_.find(stmt_num)).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name_id) !=
      var_name_list.end();
  }
  else {
    return false;
  }
}

bool UsesTable::IsUsedByP(ProcIndex proc_name_id, VarIndex var_name_id) {
  if (uses_p_map_.find(proc_name_id) != uses_p_map_.end()) {
    VarIndexList var_name_list = (*uses_p_map_.find(proc_name_id)).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name_id) !=
           var_name_list.end();
  } else {
    return false;
  }
}

StmtVarIndexPairList UsesTable::GetAllUsesSPair() {
  StmtVarIndexPairList uses_pairs;
  for (auto entry : uses_s_map_) {
    for (auto var_name : entry.second) {
      uses_pairs.push_back(make_pair(entry.first, var_name));
    }
  }
  return uses_pairs;
}

ProcVarPairList UsesTable::GetAllUsesPPair() {
  ProcVarPairList uses_pairs;
  for (auto entry : uses_p_map_) {
    for (auto var_name : entry.second) {
      uses_pairs.push_back(make_pair(entry.first, var_name));
    }
  }
  return uses_pairs;
}
