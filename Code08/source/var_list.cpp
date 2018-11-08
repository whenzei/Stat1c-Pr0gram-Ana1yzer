#pragma once

#include "var_list.h"

int VarList::InsertVarName(VarName var_name) {
  if (var_index_map_.find(var_name) == var_index_map_.end()) {
    int index = var_count_++;
    var_index_set_.insert(index);
    var_index_twin_set_.insert(make_pair(index, index));
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
      var_read_map_[var_id].insert(stmt_num);
      read_var_set_.insert(var_id);
      read_var_twin_set_.insert(make_pair(var_id, var_id));
      break;
    case PqlDeclarationEntity::kPrint:
      print_var_map_[stmt_num] = var_id;
      var_print_map_[var_id].insert(stmt_num);
      print_var_set_.insert(var_id);
      print_var_twin_set_.insert(make_pair(var_id, var_id));
      break;
  }
}

VarIndexSet VarList::GetAllVarIndices() { return var_index_set_; }

bool VarList::IsVarIndex(VarIndex var_id) {
  return index_var_map_.find(var_id) != index_var_map_.end();
}

VarIndexPairSet VarList::GetAllVarIndexTwin() { return var_index_twin_set_; }

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

StmtNumSet VarList::GetReadStmt(VarIndex var_id) {
  StmtNumSet stmt_num_list;
  if (var_read_map_.count(var_id)) {
    stmt_num_list = var_read_map_[var_id];
  }
  return stmt_num_list;
}

StmtNumSet VarList::GetPrintStmt(VarIndex var_id) {
  StmtNumSet stmt_num_list;
  if (var_print_map_.count(var_id)) {
    stmt_num_list = var_print_map_[var_id];
  }
  return stmt_num_list;
}

bool VarList::IsReadVar(VarIndex var_id) {
  return read_var_set_.find(var_id) != read_var_set_.end();
}

bool VarList::IsPrintVar(VarIndex var_id) {
  return print_var_set_.find(var_id) != print_var_set_.end();
}

VarIndexSet VarList::GetAllReadVar() { return read_var_set_; }

VarIndexSet VarList::GetAllPrintVar() { return print_var_set_; }

VarIndexPairSet VarList::GetAllReadVarTwin() { return read_var_twin_set_; }

VarIndexPairSet VarList::GetAllPrintVarTwin() { return print_var_twin_set_; }
