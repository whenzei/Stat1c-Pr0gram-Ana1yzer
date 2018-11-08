#pragma once

#include "parent_table.h"
#include <algorithm>

using std::find;

void ParentTable::InsertDirectParentRelationship(StmtNum parent_stmt_num,
                                                 StmtNum child_stmt_num) {
  parents_map_[child_stmt_num].insert(parent_stmt_num);
  direct_parent_map_[child_stmt_num] = parent_stmt_num;
  children_map_[parent_stmt_num].insert(child_stmt_num);
  direct_children_map_[parent_stmt_num].insert(child_stmt_num);
  parents_set_.insert(parent_stmt_num);
  children_set_.insert(child_stmt_num);
}

void ParentTable::InsertIndirectParentRelationship(StmtNum parent_stmt_num,
                                                   StmtNum child_stmt_num) {
  parents_map_[child_stmt_num].insert(parent_stmt_num);
  children_map_[parent_stmt_num].insert(child_stmt_num);
}

bool ParentTable::IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  DirectParentMap::iterator iter = direct_parent_map_.find(child_stmt_num);
  if (iter != direct_parent_map_.end()) {
    return parent_stmt_num == (*iter).second;
  } else {
    return false;
  }
}

bool ParentTable::IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  ChildrenMap::iterator iter = children_map_.find(parent_stmt_num);
  if (iter != children_map_.end()) {
    StmtNumSet children = (*iter).second;
    return (*iter).second.count(child_stmt_num);
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

StmtNumSet ParentTable::GetParentT(StmtNum child_stmt_num) {
  ParentsMap::iterator iter = parents_map_.find(child_stmt_num);
  if (iter != parents_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtNumSet ParentTable::GetAllParent() { return parents_set_; }

StmtNumSet ParentTable::GetChild(StmtNum parent_stmt_num) {
  DirectChildrenMap::iterator iter = direct_children_map_.find(parent_stmt_num);
  if (iter != direct_children_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtNumSet ParentTable::GetChildT(StmtNum parent_stmt_num) {
  ChildrenMap::iterator iter = children_map_.find(parent_stmt_num);
  if (iter != children_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtNumSet ParentTable::GetAllChild() { return children_set_; }

bool ParentTable::HasParentRelationship() { return !parents_map_.empty(); }

StmtNumPairSet ParentTable::GetAllParentPair() {
  StmtNumPairSet parent_pair_list;
  for (auto entry : direct_parent_map_) {
    parent_pair_list.insert(make_pair(entry.second, entry.first));
  }
  return parent_pair_list;
}

StmtNumPairSet ParentTable::GetAllParentTPair() {
  StmtNumPairSet parent_t_pair_list;
  for (auto& entry : parents_map_) {
    for (auto& parent_stmt_num : entry.second) {
      parent_t_pair_list.insert(make_pair(parent_stmt_num, entry.first));
    }
  }
  return parent_t_pair_list;
}
