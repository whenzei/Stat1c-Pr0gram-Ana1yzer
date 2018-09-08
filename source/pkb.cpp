#pragma once

#include "pkb.h"
#include "proc_list.h"
#include "assign_table.h"

bool PKB::InsertProcName(ProcName proc_name) { return proc_list_.InsertProcName(proc_name); }

ProcNameList PKB::GetAllProcName() { return proc_list_.GetProcNameList(); }

VarNameList PKB::GetAllVarName() { return var_list_.GetAllVarName(); }

int PKB::InsertAssignStmt(StmtNum statement_number, Stmt assign_statement) {
  // to-do: insert to statement table (to be added when integrating with Sophie's StmtTable class)
  return assign_table_.InsertAssignStmt(statement_number, assign_statement);
}

StmtList PKB::GetAllAssignStmt() { return assign_table_.GetAssignTable(); }
