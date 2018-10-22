#pragma once

#include "var_list.h"

int VarList::InsertVarName(VarName var_name) {
  if (var_index_map_.find(var_name) == var_index_map_.end()) {
    int index = var_count;
    var_name_index_list_.push_back(index);
    var_name_index_twin_list_.push_back(make_pair(index, index));
    var_index_map_[var_name] = index;
    index_var_map_[index] = var_name;
    var_count++;
    return index;
  }
  return -1;
}

VarNameIndexList VarList::GetAllVarName() {
  return var_name_index_list_;
}

bool VarList::IsVarName(VarName var_name) {
  return var_index_map_.find(var_name) != var_index_map_.end(); 
}

VarNameIndexPairList VarList::GetAllVarNameTwin() {
  return var_name_index_twin_list_;
}

IndexVarMap VarList::GetIndexToVarMapping() {
  return index_var_map_;
}

VarIndexMap VarList::GetVarToIndexMapping() {
  return var_index_map_; }

VarName VarList::GetVarName(VarNameIndex index) { return index_var_map_[index]; }

VarNameIndex VarList::GetVarNameIndex(VarName var_name) {
  return var_index_map_[var_name];
}
