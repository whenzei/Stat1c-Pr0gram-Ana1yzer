#pragma once

#include "modifies_table.h"

using std::find;
using std::make_pair;

void ModifiesTable::InsertModifiesS(StmtNum stmt_num, VarNameIndex var_name_id) {
  if (!IsModifiedByS(stmt_num, var_name_id)) {
    if (modifying_stmt_set_.insert(stmt_num).second) {
      modifying_stmt_list_.push_back(stmt_num);
    }
    modifies_s_map_[stmt_num].push_back(var_name_id);
    modified_by_s_map_[var_name_id].push_back(stmt_num);
  }
}

void ModifiesTable::InsertModifiesP(ProcNameIndex proc_name_id, VarNameIndex var_name_id) {
  if (!IsModifiedByP(proc_name_id, var_name_id)) {
    if (modifying_proc_set_.insert(proc_name_id).second) {
      modifying_proc_list_.push_back(proc_name_id);
    }
    modifies_p_map_[proc_name_id].push_back(var_name_id);
    modified_by_p_map_[var_name_id].push_back(proc_name_id);
  }
}

bool ModifiesTable::IsModifiedByS(StmtNum stmt_num, VarNameIndex var_name_id) {
  ModifiesMap::iterator iter = modifies_s_map_.find(stmt_num);
  if (iter != modifies_s_map_.end()) {
    VarNameIndexList var_name_list = (*iter).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name_id) !=
           var_name_list.end();
  } else {
    return false;
  }
}

bool ModifiesTable::IsModifiedByP(ProcNameIndex proc_name_id, VarNameIndex var_name_id) {
  ModifiesMap::iterator iter = modifies_p_map_.find(proc_name_id);
  if (iter != modifies_p_map_.end()) {
    VarNameIndexList var_name_list = (*iter).second;
    return find(var_name_list.begin(), var_name_list.end(), var_name_id) !=
           var_name_list.end();
  } else {
    return false;
  }
}

VarNameIndexList ModifiesTable::GetModifiedVarS(StmtNum stmt_num) {
  ModifiesMap::iterator iter = modifies_s_map_.find(stmt_num);
  if (iter != modifies_s_map_.end()) {
    return (*iter).second;
  } else {
    return VarNameIndexList();
  }
}

VarNameIndexList ModifiesTable::GetModifiedVarP(ProcNameIndex proc_name_id) {
  ModifiesMap::iterator iter = modifies_p_map_.find(proc_name_id);
  if (iter != modifies_p_map_.end()) {
    return (*iter).second;
  } else {
    return VarNameIndexList();
  }
}

StmtNumList ModifiesTable::GetModifyingStmt(VarNameIndex var_name_id) {
  ModifiedByMap::iterator iter = modified_by_s_map_.find(var_name_id);
  if (iter != modified_by_s_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

StmtNumList ModifiesTable::GetAllModifyingStmt() {
  return modifying_stmt_list_;
}

ProcNameIndexList ModifiesTable::GetModifyingProc(VarNameIndex var_name_id) {
  ModifiedByMap::iterator iter = modified_by_p_map_.find(var_name_id);
  if (iter != modified_by_p_map_.end()) {
    return (*iter).second;
  } else {
    return ProcNameIndexList();
  }
}

ProcNameIndexList ModifiesTable::GetAllModifyingProc() {
  return modifying_proc_list_;
}

StmtVarPairList ModifiesTable::GetAllModifiesPairS() {
  StmtVarPairList modifies_pair_list;
  for (auto entry : modifies_s_map_) {
    for (VarNameIndex& var_name_id : entry.second) {
      modifies_pair_list.push_back(make_pair(entry.first, var_name_id));
	}
  }
  return modifies_pair_list;
}

ProcVarPairList ModifiesTable::GetAllModifiesPairP() {
  ProcVarPairList modifies_pair_list;
  for (auto entry : modifies_p_map_) {
    for (VarNameIndex& var_name_id : entry.second) {
      modifies_pair_list.push_back(make_pair(entry.first, var_name_id));
    }
  }
  return modifies_pair_list;
}
