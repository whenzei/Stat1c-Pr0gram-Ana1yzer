#pragma once

#include "parent_table.h"
#include <algorithm>

using std::find;

void ParentTable::InsertDirectParentRelationship(StmtNum stmt_num,
                                                 StmtListIndex stmtlist_index) {
  if (!IsParent(stmt_num, stmtlist_index)) {
    parents_map_[stmtlist_index].push_back(stmt_num);
    direct_parent_map_[stmtlist_index] = stmt_num;
    children_map_[stmt_num].push_back(stmtlist_index);
    direct_children_map_[stmt_num].push_back(stmtlist_index);
  }
  if (parents_set_.find(stmt_num) == parents_set_.end()) {
    parents_set_.insert(stmt_num);
  }
  if (children_set_.find(stmtlist_index) == children_set_.end()) {
    children_set_.insert(stmtlist_index);
  }
}

void ParentTable::InsertIndirectParentRelationship(
    StmtNum stmt_num, StmtListIndex stmtlist_index) {
  if (!IsParentT(stmt_num, stmtlist_index)) {
    parents_map_[stmtlist_index].push_back(stmt_num);
    children_map_[stmt_num].push_back(stmtlist_index);
  }
}

bool ParentTable::IsParent(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  DirectParentMap::iterator iter = direct_parent_map_.find(stmtlist_index);
  if (iter != direct_parent_map_.end()) {
    StmtNum parent_stmt_num = (*iter).second;
    return parent_stmt_num == stmt_num;
  } else {
    return false;
  }
}

bool ParentTable::IsParentT(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  ChildrenMap::iterator iter = children_map_.find(stmt_num);
  if (iter != children_map_.end()) {
    StmtListIndexList child_stmtlist_index_list = (*iter).second;
    return find(child_stmtlist_index_list.begin(),
                child_stmtlist_index_list.end(),
                stmtlist_index) != child_stmtlist_index_list.end();
  } else {
    return false;
  }
}

StmtNum ParentTable::GetParent(StmtListIndex stmtlist_index) {
  DirectParentMap::iterator iter = direct_parent_map_.find(stmtlist_index);
  if (iter != direct_parent_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNum();
  }
}

StmtNumList ParentTable::GetParentT(StmtListIndex stmtlist_index) {
  ParentsMap::iterator iter = parents_map_.find(stmtlist_index);
  if (iter != parents_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

ParentsSet ParentTable::GetParentsSet() { return parents_set_; }

StmtListIndexList ParentTable::GetChild(StmtNum stmt_num) {
  DirectChildrenMap::iterator iter = direct_children_map_.find(stmt_num);
  if (iter != direct_children_map_.end()) {
    return (*iter).second;
  } else {
    return StmtListIndexList();
  }
}

StmtListIndexList ParentTable::GetChildT(StmtNum stmt_num) {
  ChildrenMap::iterator iter = children_map_.find(stmt_num);
  if (iter != children_map_.end()) {
    return (*iter).second;
  } else {
    return StmtListIndexList();
  }
}

ChildrenSet ParentTable::GetChildrenSet() { return children_set_; }

bool ParentTable::HasParentRelationship() {
  return !parents_map_.empty();
}

DirectParentMap ParentTable::GetDirectParentMap() { return direct_parent_map_; }

ParentsMap ParentTable::GetParentsMap() { return parents_map_; }
