#pragma once

#include "var_list.h"

void VarList::InsertVarName(VarName var_name) {
  if (var_name_set_.insert(var_name).second) {
    var_name_list_.push_back(var_name);
  }
}

VarNameList VarList::GetAllVarName() {
  return var_name_list_;
}

bool VarList::IsVarName(VarName var_name) {
  return var_name_set_.find(var_name) != var_name_set_.end(); 
}