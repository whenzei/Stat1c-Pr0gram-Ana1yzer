#pragma once

#include "var_list.h"

void VarList::InsertVarName(VarName var_name) {
  if (var_name_set_.insert(var_name).second) {
    var_name_list_.push_back(var_name);
    var_name_twin_list_.push_back(make_pair(var_name, var_name));
  }
}

VarNameList VarList::GetAllVarName() {
  return var_name_list_; }

VarNamePairList VarList::GetAllVarNameTwin() {
  return var_name_twin_list_;
}
