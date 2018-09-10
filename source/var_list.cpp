#pragma once

#include "var_list.h"

bool VarList::InsertVarName(VarName var_name) {
  if (var_name_set_.find(var_name) != var_name_set_.end()) {
    return false;
  } else {
    var_name_list_.push_back(var_name);
    var_name_set_.insert(var_name);
    return true;
  }
}

VarNameList VarList::GetAllVarName() {
  return var_name_list_;
}