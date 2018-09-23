#pragma once

#include "uses_table.h"

bool UsesTable::InsertUses(VarName var_name, StmtNum stmt_num) {
  // returns false if same uses relationship already exists in the uses map
  if (IsUsedBy(var_name, stmt_num)) { return false; }
  // check for duplicates
  if (uses_map_.count(stmt_num) == 0) {
    using_stmts_list_.push_back(stmt_num);
  }
  if (used_by_map_.count(var_name) == 0) { used_vars_list_.push_back(var_name); }
  uses_map_[stmt_num].push_back(var_name);
  used_by_map_[var_name].push_back(stmt_num);
  return true;
}

VarNameList UsesTable::GetAllUsedVar() {
  return used_vars_list_;
}

VarNameList UsesTable::GetAllUsedVar(StmtNum stmt_num) {
  VarNameList used_vars;
  if (uses_map_.find(stmt_num) != uses_map_.end()) {
    used_vars = uses_map_[stmt_num];
  }
  return used_vars;
}

StmtList UsesTable::GetAllUsingStmt() {
  return using_stmts_list_;
}

StmtList UsesTable::GetAllUsingStmt(VarName var_name) {
  StmtList using_stmts;
  if (used_by_map_.find(var_name) != used_by_map_.end()) {
    using_stmts = used_by_map_[var_name];
  }
  return using_stmts;
}

bool UsesTable::IsUsedBy(VarName var_name, StmtNum stmt_num) {
  if (uses_map_.find(stmt_num) != uses_map_.end()) {
    VarNameList var_name_list = (*uses_map_.find(stmt_num)).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name) !=
      var_name_list.end();
  }
  else {
    return false;
  }
}

bool UsesTable::HasUsesRelationship() {
  return (!uses_map_.empty());
}

StmtVarPairList UsesTable::GetAllUsesPair() {
  StmtVarPairList uses_pairs;
  if (!HasUsesRelationship()) { return uses_pairs; }
  for (auto entry : uses_map_) {
    for (auto var_name : entry.second) {
      uses_pairs.push_back(make_pair(entry.first, var_name));
    }
  }
  return uses_pairs;
}