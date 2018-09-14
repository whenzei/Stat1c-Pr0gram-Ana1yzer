#pragma once

#include "parent_table.h"
#include "modifies_table.h"

using std::find;
using std::make_pair;

void ModifiesTable::InsertModifies(StmtNum stmt_num, VarName var_name) {
  if (!IsModifiedBy(stmt_num, var_name)) {
    modifying_stmt_num_list_.push_back(stmt_num);
    modified_var_name_list_.push_back(var_name);
    modifies_map_[stmt_num].push_back(var_name);
    modified_by_map_[var_name].push_back(stmt_num);
  }
}

bool ModifiesTable::IsModifiedBy(StmtNum stmt_num, VarName var_name) {
  ModifiesMap::iterator iter = modifies_map_.find(stmt_num);
  if (iter != modifies_map_.end()) {
    VarNameList var_name_list = (*iter).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name) !=
           var_name_list.end();
  } else {
    return false;
  }
  
}

VarNameList ModifiesTable::GetModifiedVar(StmtNum stmt_num) {
  ModifiesMap::iterator iter = modifies_map_.find(stmt_num);
  if (iter != modifies_map_.end()) {
    return (*iter).second;
  } else {
    return VarNameList();
  }
}

VarNameList ModifiesTable::GetAllModifiedVar() {
  return modified_var_name_list_;
}

StmtNumList ModifiesTable::GetModifyingStmt(VarName var_name) {
  ModifiedByMap::iterator iter = modified_by_map_.find(var_name);
  if (iter != modified_by_map_.end()) {
    return (*iter).second;
  } else {
    return VarNameList();
  }
}

StmtNumList ModifiesTable::GetAllModifyingStmt() { return modifying_stmt_num_list_; }

bool ModifiesTable::HasModifiesRelationship() { return !modifies_map_.empty(); }

StmtVarPairList ModifiesTable::GetAllModifiesPair() {
  StmtVarPairList modifies_pair_list;
  for (auto entry : modifies_map_) {
    for (VarName& var_name : entry.second) {
      modifies_pair_list.push_back(make_pair(entry.first, var_name));
	}
  }
  return modifies_pair_list;
}
