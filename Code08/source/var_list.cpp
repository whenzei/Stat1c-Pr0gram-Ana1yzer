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

void VarList::InsertVarName(VarName var_name, PqlDeclarationEntity stmt_type,
                           StmtNum stmt_num) {
  VarIndex var_id = InsertVarName(var_name);
  if (var_id == -1) {
    var_id = GetVarIndex(var_name);
  }
  switch (stmt_type) {
    case PqlDeclarationEntity::kRead:
      read_var_map_[stmt_num] = var_id;
      if (read_var_set_.insert(var_id).second) {
        read_var_list_.push_back(var_id);
        read_var_twin_list_.push_back(make_pair(var_id, var_id));
      }
      break;
    case PqlDeclarationEntity::kPrint:
      print_var_map_[stmt_num] = var_id;
      if (print_var_set_.insert(var_id).second) {
        print_var_list_.push_back(var_id);
        print_var_twin_list_.push_back(make_pair(var_id, var_id));
      }
      break;
  }
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
    return VarIndex();
  }
}

VarIndex VarList::GetReadVar(StmtNum stmt_num) {
  StmtVarMap::iterator iter = read_var_map_.find(stmt_num);
  if (iter != read_var_map_.end()) {
    return (*iter).second;
  } else {
    return VarIndex();
  }
}

VarIndex VarList::GetPrintVar(StmtNum stmt_num) {
  StmtVarMap::iterator iter = print_var_map_.find(stmt_num);
  if (iter != print_var_map_.end()) {
    return (*iter).second;
  } else {
    return VarIndex();
  }
}

bool VarList::IsReadVar(VarIndex var_id) {
  return read_var_set_.find(var_id) != read_var_set_.end();
}

bool VarList::IsPrintVar(VarIndex var_id) {
  return print_var_set_.find(var_id) != print_var_set_.end();
}

VarIndexList VarList::GetAllReadVar() { return read_var_list_; }

VarIndexList VarList::GetAllPrintVar() { return print_var_list_; }

VarIndexPairList VarList::GetAllReadVarTwin() { return read_var_twin_list_; }

VarIndexPairList VarList::GetAllPrintVarTwin() { return print_var_twin_list_; }
