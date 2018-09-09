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
}

void ParentTable::InsertIndirectParentRelationship(StmtNum stmt_num,
                                           StmtListIndex stmtlist_index) {
  parents_map_[stmtlist_index].push_back(stmt_num);
  children_map_[stmt_num].push_back(stmtlist_index);
}

bool ParentTable::IsDirectParent(StmtNum stmt_num,
                                 StmtListIndex stmtlist_index) {
  StmtNum parent_stmt_num = direct_parent_map_[stmtlist_index];
  return parent_stmt_num == stmt_num;
}

bool ParentTable::IsParent(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  StmtListIndexList child_stmtlist_index_list = children_map_[stmt_num];
  return find(child_stmtlist_index_list.begin(),
              child_stmtlist_index_list.end(),
              stmtlist_index) != child_stmtlist_index_list.end();
}

StmtNum ParentTable::GetDirectParent(StmtListIndex stmtlist_index) { 
  return direct_parent_map_[stmtlist_index];
}

StmtNumList ParentTable::GetParents(StmtListIndex stmtlist_index) {
  return parents_map_[stmtlist_index];
}

StmtListIndexList ParentTable::GetDirectChildren(StmtNum stmt_num) {
  return direct_children_map_[stmt_num];
}

StmtListIndexList ParentTable::GetChildren(StmtNum stmt_num) {
  return children_map_[stmt_num];
}

bool ParentTable::HasParentRelationship() { return parents_map_.size() != 0; }
