#pragma once

#include "parent_table.h"
#include <algorithm>

using std::find;

void ParentTable::InsertDirectParentRelationship(StmtNum parent_stmt_num,
                                                 StmtNum child_stmt_num) {
  if (!IsParent(parent_stmt_num, child_stmt_num)) {
    parents_map_[child_stmt_num].push_back(parent_stmt_num);
    direct_parent_map_[child_stmt_num] = parent_stmt_num;
    children_map_[parent_stmt_num].push_back(child_stmt_num);
    direct_children_map_[parent_stmt_num].push_back(child_stmt_num);
  }
  if (parents_set_.insert(parent_stmt_num).second) {
    parents_list_.push_back(parent_stmt_num);
  }
  if (children_set_.insert(child_stmt_num).second) {
    children_list_.push_back(child_stmt_num);
  }
}

void ParentTable::InsertIndirectParentRelationship(StmtNum parent_stmt_num,
                                                   StmtNum child_stmt_num) {
  if (!IsParentT(parent_stmt_num, child_stmt_num)) {
    parents_map_[child_stmt_num].push_back(parent_stmt_num);
    children_map_[parent_stmt_num].push_back(child_stmt_num);
  }
}

bool ParentTable::IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  DirectParentMap::iterator iter = direct_parent_map_.find(child_stmt_num);
  if (iter != direct_parent_map_.end()) {
    StmtNum parent = (*iter).second;
    return parent == parent_stmt_num;
  } else {
    return false;
  }
}

bool ParentTable::IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  ChildrenMap::iterator iter = children_map_.find(parent_stmt_num);
  if (iter != children_map_.end()) {
    StmtNumList children = (*iter).second;
    return find(children.begin(), children.end(), child_stmt_num) !=
           children.end();
  } else {
    return false;
  }
}

StmtNum ParentTable::GetParent(StmtNum child_stmt_num) {
  DirectParentMap::iterator iter = direct_parent_map_.find(child_stmt_num);
  if (iter != direct_parent_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNum();
  }
}

StmtNumList ParentTable::GetParentT(StmtNum child_stmt_num) {
  ParentsMap::iterator iter = parents_map_.find(child_stmt_num);
  if (iter != parents_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

StmtNumList ParentTable::GetAllParent() { return parents_list_; }

StmtNumList ParentTable::GetChild(StmtNum parent_stmt_num) {
  DirectChildrenMap::iterator iter = direct_children_map_.find(parent_stmt_num);
  if (iter != direct_children_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

StmtNumList ParentTable::GetChildT(StmtNum parent_stmt_num) {
  ChildrenMap::iterator iter = children_map_.find(parent_stmt_num);
  if (iter != children_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

StmtNumList ParentTable::GetAllChild() { return children_list_; }

bool ParentTable::HasParentRelationship() { return !parents_map_.empty(); }

StmtNumPairList ParentTable::GetAllParentPair() {
  StmtNumPairList parent_pair_list;
  for (auto entry : direct_parent_map_) {
    parent_pair_list.push_back(make_pair(entry.second, entry.first));
  }
  return parent_pair_list;
}

StmtNumPairList ParentTable::GetAllParentTPair() {
  StmtNumPairList parent_t_pair_list;
  for (auto entry : parents_map_) {
    for (StmtNum& parent_stmt_num : entry.second) {
      parent_t_pair_list.push_back(make_pair(parent_stmt_num, entry.first));
    }
  }
  return parent_t_pair_list;
}
