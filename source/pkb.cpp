#pragma once

#include "pkb.h"
#include <sstream>

using std::stringstream;

bool PKB::InsertProcName(ProcName proc_name) {
  return proc_list_.InsertProcName(proc_name);
}

ProcNameList PKB::GetAllProcName() { return proc_list_.GetAllProcName(); }

VarNameList PKB::GetAllVarName() { return var_list_.GetAllVarName(); }

bool PKB::InsertConstValue(ConstValue const_value) {
  return const_list_.InsertConstValue(const_value);
}

ConstValueList PKB::GetAllConstValue() {
  return const_list_.GetAllConstValue();
}

bool PKB::InsertAssignStmt(StmtNumInt stmt_num_int,
                           StmtListIndex stmtlist_index,
                           VarName modified_var_name,
                           VarNameList used_var_name_list) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertAssignStmt(stmt_num);
    var_list_.InsertVarName(modified_var_name);
    // insert variables
    for (VarName& var_name : used_var_name_list) {
      var_list_.InsertVarName(var_name);
    }
    // TODO: add follows, modifies and uses relationship
    return true;
  } else {
    return false;
  }
}

StmtListIndex PKB::InsertWhileStmt(StmtNumInt stmt_num_int,
                                   StmtListIndex stmtlist_index,
                                   VarNameList control_var_name_list) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertWhileStmt(stmt_num);
    // insert variables
    for (VarName& var_name : control_var_name_list) {
      var_list_.InsertVarName(var_name);
    }
    // insert parent relationships
    StmtListIndex child_stmtlist_index = stmtlist_table_.GetNextStmtListIndex();
    parent_table_.InsertDirectParentRelationship(stmt_num,
                                                 child_stmtlist_index);
    StmtNumList indirect_parents = parent_table_.GetParents(stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     child_stmtlist_index);
	}
    // TODO: insert follows and uses relationship
    return child_stmtlist_index;
  } else {
    return -1;
  }
}

pair<StmtListIndex, StmtListIndex> PKB::InsertIfStmt(
    StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
    VarNameList control_var_name_list) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertIfStmt(stmt_num);
	// insert variables
    for (VarName& var_name : control_var_name_list) {
      var_list_.InsertVarName(var_name);
    }
    // insert parent relationships
    pair<StmtListIndex, StmtListIndex> children_stmtlist_indices =
        stmtlist_table_.GetNextTwoStmtListIndices();
    parent_table_.InsertDirectParentRelationship(
        stmt_num, children_stmtlist_indices.first);
    parent_table_.InsertDirectParentRelationship(
        stmt_num, children_stmtlist_indices.second);
    StmtNumList indirect_parents = parent_table_.GetParents(stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(
          indirect_parent, children_stmtlist_indices.first);
      parent_table_.InsertIndirectParentRelationship(
          indirect_parent, children_stmtlist_indices.second);
    }
    // TODO: insert follows and uses relationship
    return children_stmtlist_indices;
  } else {
    return pair<StmtListIndex, StmtListIndex>(-1, -1);
  }
}

bool PKB::InsertReadStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                         VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertReadStmt(stmt_num);
    var_list_.InsertVarName(var_name);
    // TODO: insert follows and modifies relationship
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertPrintStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                          VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertPrintStmt(stmt_num);
    var_list_.InsertVarName(var_name);
    // TODO: insert follows and uses relationship
    return true;
  } else {
    return false;
  }
}

StmtNumList PKB::GetAllStmt() { return stmt_type_list_.GetAllStmt(); }

StmtNumList PKB::GetAllAssignStmt() {
  return stmt_type_list_.GetAllAssignStmt();
}

StmtNumList PKB::GetAllWhileStmt() { return stmt_type_list_.GetAllWhileStmt(); }

StmtNumList PKB::GetAllIfStmt() { return stmt_type_list_.GetAllIfStmt(); }

StmtNumList PKB::GetAllReadStmt() { return stmt_type_list_.GetAllReadStmt(); }

StmtNumList PKB::GetAllPrintStmt() { return stmt_type_list_.GetAllPrintStmt(); }

bool PKB::IsDirectParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsDirectParent(parent_stmt_num, child_stmtlist_index);
}

bool PKB::IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsParent(parent_stmt_num, child_stmtlist_index);
}

StmtNum PKB::GetDirectParent(StmtNum stmt_num) {
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  return parent_table_.GetDirectParent(stmtlist_index);
}

StmtNumList PKB::GetParents(StmtNum stmt_num) {
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  return parent_table_.GetParents(stmtlist_index);
}

StmtNumList PKB::GetDirectChildren(StmtNum stmt_num) {
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetDirectChildren(stmt_num);
  StmtNumList result;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    result.insert(result.end(), stmt_num_list.begin(), stmt_num_list.end());
  }
  return result;
}

StmtNumList PKB::GetChildren(StmtNum stmt_num) {
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetChildren(stmt_num);
  StmtNumList result;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    result.insert(result.end(), stmt_num_list.begin(), stmt_num_list.end());
  }
  return result;
}

bool PKB::HasParentRelationship() { return parent_table_.HasParentRelationship(); }

StmtNum PKB::ToString(int stmt_num_int) {
  stringstream stmt_num_ss;
  stmt_num_ss << stmt_num_int;
  return stmt_num_ss.str();
}
