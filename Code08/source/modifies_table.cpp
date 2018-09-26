#pragma once

#include "modifies_table.h"

using std::find;
using std::make_pair;

void ModifiesTable::InsertModifiesS(StmtNum stmt_num, VarName var_name) {
  if (!IsModifiedByS(stmt_num, var_name)) {
    if (modifying_stmt_set_.insert(stmt_num).second) {
      modifying_stmt_list_.push_back(stmt_num);
    }
    modifies_s_map_[stmt_num].push_back(var_name);
    modified_by_s_map_[var_name].push_back(stmt_num);
  }
}

void ModifiesTable::InsertModifiesP(ProcName proc_name, VarName var_name) {
  if (!IsModifiedByP(proc_name, var_name)) {
    if (modifying_proc_set_.insert(proc_name).second) {
      modifying_proc_list_.push_back(proc_name);
    }
    modifies_p_map_[proc_name].push_back(var_name);
    modified_by_p_map_[var_name].push_back(proc_name);
  }
}

bool ModifiesTable::IsModifiedByS(StmtNum stmt_num, VarName var_name) {
  ModifiesMap::iterator iter = modifies_s_map_.find(stmt_num);
  if (iter != modifies_s_map_.end()) {
    VarNameList var_name_list = (*iter).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name) !=
           var_name_list.end();
  } else {
    return false;
  }
}

bool ModifiesTable::IsModifiedByP(ProcName proc_name, VarName var_name) {
  ModifiesMap::iterator iter = modifies_p_map_.find(proc_name);
  if (iter != modifies_p_map_.end()) {
    VarNameList var_name_list = (*iter).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name) !=
           var_name_list.end();
  } else {
    return false;
  }
}

VarNameList ModifiesTable::GetModifiedVarS(StmtNum stmt_num) {
  ModifiesMap::iterator iter = modifies_s_map_.find(stmt_num);
  if (iter != modifies_s_map_.end()) {
    return (*iter).second;
  } else {
    return VarNameList();
  }
}

VarNameList ModifiesTable::GetModifiedVarP(ProcName proc_name) {
  ModifiesMap::iterator iter = modifies_p_map_.find(proc_name);
  if (iter != modifies_p_map_.end()) {
    return (*iter).second;
  } else {
    return VarNameList();
  }
}

StmtNumList ModifiesTable::GetModifyingStmt(VarName var_name) {
  ModifiedByMap::iterator iter = modified_by_s_map_.find(var_name);
  if (iter != modified_by_s_map_.end()) {
    return (*iter).second;
  } else {
    return VarNameList();
  }
}

StmtNumList ModifiesTable::GetAllModifyingStmt() {
  return modifying_stmt_list_;
}

ProcNameList ModifiesTable::GetModifyingProc(VarName var_name) {
  ModifiedByMap::iterator iter = modified_by_p_map_.find(var_name);
  if (iter != modified_by_p_map_.end()) {
    return (*iter).second;
  } else {
    return ProcNameList();
  }
}

ProcNameList ModifiesTable::GetAllModifyingProc() {
  return modifying_proc_list_;
}

StmtVarPairList ModifiesTable::GetAllModifiesPairS() {
  StmtVarPairList modifies_pair_list;
  for (auto entry : modifies_s_map_) {
    for (VarName& var_name : entry.second) {
      modifies_pair_list.push_back(make_pair(entry.first, var_name));
	}
  }
  return modifies_pair_list;
}

ProcVarPairList ModifiesTable::GetAllModifiesPairP() {
  ProcVarPairList modifies_pair_list;
  for (auto entry : modifies_p_map_) {
    for (VarName& var_name : entry.second) {
      modifies_pair_list.push_back(make_pair(entry.first, var_name));
    }
  }
  return modifies_pair_list;
}
