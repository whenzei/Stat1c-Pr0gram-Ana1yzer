#pragma once

#include "var_list.h"

int VarList::InsertVarName(VarName var_name) {
  if (var_index_map_.find(var_name) == var_index_map_.end()) {
    int index = var_count_++;
    var_index_list_.push_back(index);
    var_index_twin_list_.push_back(make_pair(index, index));
    var_index_map_[var_name] = index;
    index_var_map_[index] = var_name;
    return index;
  }
  return -1;
}

VarIndexList VarList::GetAllVarIndices() { return var_index_list_; }

bool VarList::IsVarIndex(VarIndex var_id) {
  return index_var_map_.find(var_id) != index_var_map_.end();
}

VarIndexPairList VarList::GetAllVarIndexTwin() { return var_index_twin_list_; }

bool VarList::IsVarName(VarName var_name) {
  return var_index_map_.find(var_name) != var_index_map_.end();
}

IndexVarMap VarList::GetIndexToVarMapping() { return index_var_map_; }

VarIndexMap VarList::GetVarToIndexMapping() { return var_index_map_; }

VarName VarList::GetVarName(VarIndex index) {
  IndexVarMap::iterator iter = index_var_map_.find(index);
  if (iter != index_var_map_.end()) {
    return (*iter).second;
  } else {
    return VarName();
  }
}

VarIndex VarList::GetVarIndex(VarName var_name) {
  VarIndexMap::iterator iter = var_index_map_.find(var_name);
  if (iter != var_index_map_.end()) {
    return (*iter).second;
  } else {
    return -1;
  }
}
