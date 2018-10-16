#pragma once

#include "var_list.h"

void VarList::InsertVarName(VarName var_name) {
  if (var_name_set_.insert(var_name).second) {
    var_name_list_.push_back(var_name);
    var_name_twin_list_.push_back(make_pair(var_name, var_name));
    var_index_map_[var_name] = var_count;
    var_count++;
  }
}

VarNameList VarList::GetAllVarName() {
  return var_name_list_;
}

bool VarList::IsVarName(VarName var_name) {
  return var_name_set_.find(var_name) != var_name_set_.end(); 
}

VarNamePairList VarList::GetAllVarNameTwin() {
  return var_name_twin_list_;
}

int VarList::GetIndexForVar(VarName var_name) {
  if (var_index_map_.find(var_name) != var_index_map_.end()) {
    return var_index_map_[var_name];
  }
  return -1;
}

VarName VarList::GetVarForIndex(int index) {
  if (index_var_map_.find(index) != index_var_map_.end()) {
    return index_var_map_[index];
  }
  return VarName();
}