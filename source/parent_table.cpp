#pragma once

#include "parent_table.h"
#include <algorithm>

using std::find;

void ParentTable::InsertDirectParentRelationship(StmtNum stmt_num,
                                                 StmtListIndex stmtlist_index) {
  parents_map_[stmtlist_index].push_back(stmt_num);
  direct_parent_map_[stmtlist_index] = stmt_num;
  children_map_[stmt_num].push_back(stmtlist_index);
  direct_children_map_[stmt_num].push_back(stmtlist_index);
  parents_set_.insert(stmt_num);
  children_set_.insert(stmtlist_index);
}

void ParentTable::InsertIndirectParentRelationship(
    StmtNum stmt_num, StmtListIndex stmtlist_index) {
  parents_map_[stmtlist_index].push_back(stmt_num);
  children_map_[stmt_num].push_back(stmtlist_index);
}

bool ParentTable::IsParent(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  StmtNum parent_stmt_num = direct_parent_map_[stmtlist_index];
  return parent_stmt_num == stmt_num;
}

bool ParentTable::IsParentT(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  StmtListIndexList child_stmtlist_index_list = children_map_[stmt_num];
  return find(child_stmtlist_index_list.begin(),
              child_stmtlist_index_list.end(),
              stmtlist_index) != child_stmtlist_index_list.end();
}

StmtNum ParentTable::GetParent(StmtListIndex stmtlist_index) {
  return direct_parent_map_[stmtlist_index];
}

StmtNumList ParentTable::GetParentT(StmtListIndex stmtlist_index) {
  return parents_map_[stmtlist_index];
}

ParentsSet ParentTable::GetAllParent() { return parents_set_; }

StmtListIndexList ParentTable::GetChild(StmtNum stmt_num) {
  return direct_children_map_[stmt_num];
}

StmtListIndexList ParentTable::GetChildT(StmtNum stmt_num) {
  return children_map_[stmt_num];
}

ChildrenSet ParentTable::GetAllChild() { return children_set_; }

bool ParentTable::HasParentRelationship() {
  for (const auto& map_entry : parents_map_) {
    if (!map_entry.second.empty()) {
      return true;
    }
  }
  return false;
}

DirectParentMap ParentTable::GetAllParentPair() { return direct_parent_map_; }

ParentsMap ParentTable::GetAllParentTPair() { return parents_map_; }
