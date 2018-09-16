#pragma once

#include "uses_table.h"

bool UsesTable::InsertUses(VarName var_name, StmtNum stmt_num) {
  // returns false if same uses relationship already exists in the uses map
  if (IsUsedBy(var_name, stmt_num)) { return false; }
  uses_map_[stmt_num].push_back(var_name);
  used_by_map_[var_name].push_back(stmt_num);
  return true;
}

VarList UsesTable::GetAllUsedVar() {
  VarList used_vars;
  for (auto entry : used_by_map_) {
    used_vars.push_back(entry.first);
  }
  return used_vars;
}

VarList UsesTable::GetAllUsedVar(StmtNum stmt_num) {
  VarList used_vars;
  if (uses_map_.find(stmt_num) != uses_map_.end()) {
    used_vars = uses_map_[stmt_num];
  }
  return used_vars;
}

StmtList UsesTable::GetAllUsingStmt() {
  StmtList using_stmts;
  for (auto entry : uses_map_) {
    using_stmts.push_back(entry.first);
  }
  return using_stmts;
}

StmtList UsesTable::GetAllUsingStmt(VarName var_name) {
  StmtList using_stmts;
  if (used_by_map_.find(var_name) != used_by_map_.end()) {
    using_stmts = used_by_map_[var_name];
  }
  return using_stmts;
}

bool UsesTable::IsUsedBy(VarName var_name, StmtNum stmt_num) {
  return (find(uses_map_[stmt_num].begin(), uses_map_[stmt_num].end(), var_name) != uses_map_[stmt_num].end());
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