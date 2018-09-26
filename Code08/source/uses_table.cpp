#pragma once

#include "uses_table.h"

void UsesTable::InsertUsesS(StmtNum stmt_num, VarName var_name) {
  if (!IsUsedByS(stmt_num, var_name)) {
    if (using_stmt_set_.insert(stmt_num).second) {
      using_stmt_list_.push_back(stmt_num);
    }
    uses_s_map_[stmt_num].push_back(var_name);
    used_by_s_map_[var_name].push_back(stmt_num);
  }
}

void UsesTable::InsertUsesP(ProcName proc_name, VarName var_name) {
  if (!IsUsedByP(proc_name, var_name)) {
    if (using_proc_set_.insert(proc_name).second) {
      using_proc_list_.push_back(proc_name);
    }
    uses_p_map_[proc_name].push_back(var_name);
    used_by_p_map_[var_name].push_back(proc_name);
  }
}

VarNameList UsesTable::GetUsedVarS(StmtNum stmt_num) {
  VarNameList used_vars;
  if (uses_s_map_.find(stmt_num) != uses_s_map_.end()) {
    used_vars = uses_s_map_[stmt_num];
  }
  return used_vars;
}

VarNameList UsesTable::GetUsedVarP(ProcName proc_name) {
  VarNameList used_vars;
  if (uses_p_map_.find(proc_name) != uses_p_map_.end()) {
    used_vars = uses_p_map_[proc_name];
  }
  return used_vars;
}

StmtNumList UsesTable::GetAllUsingStmt() {
  return using_stmt_list_; 
}

ProcNameList UsesTable::GetAllUsingProc() {
  return using_proc_list_;
}

StmtNumList UsesTable::GetUsingStmt(VarName var_name) {
  StmtNumList using_stmts;
  if (used_by_s_map_.find(var_name) != used_by_s_map_.end()) {
    using_stmts = used_by_s_map_[var_name];
  }
  return using_stmts;
}

ProcNameList UsesTable::GetUsingProc(VarName var_name) {
  ProcNameList using_proc;
  if (used_by_p_map_.find(var_name) != used_by_p_map_.end()) {
    using_proc = used_by_p_map_[var_name];
  }
  return using_proc;
}

bool UsesTable::IsUsedByS(StmtNum stmt_num, VarName var_name) {
  if (uses_s_map_.find(stmt_num) != uses_s_map_.end()) {
    VarNameList var_name_list = (*uses_s_map_.find(stmt_num)).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name) !=
      var_name_list.end();
  }
  else {
    return false;
  }
}

bool UsesTable::IsUsedByP(ProcName proc_name, VarName var_name) {
  if (uses_p_map_.find(proc_name) != uses_p_map_.end()) {
    VarNameList var_name_list = (*uses_p_map_.find(proc_name)).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name) !=
           var_name_list.end();
  } else {
    return false;
  }
}

StmtVarPairList UsesTable::GetAllUsesSPair() {
  StmtVarPairList uses_pairs;
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
