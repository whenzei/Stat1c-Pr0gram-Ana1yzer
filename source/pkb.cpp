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
                           StmtListIndex stmtlist_index, VarName lfs_var_name,
                           VarNameList rhs_var_name_list) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertAssignStmt(stmt_num);
	// TODO: insert variables
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
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertWhileStmt(stmt_num);
    // TODO: insert variables
    // TODO: add follows, parent and uses relationship
    return stmtlist_table_.GetNextStmtListIndex();
  } else {
    return -1;
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

StmtNum PKB::ToString(int stmt_num_int) {
  stringstream stmt_num_ss;
  stmt_num_ss << stmt_num_int;
  return stmt_num_ss.str();
}
