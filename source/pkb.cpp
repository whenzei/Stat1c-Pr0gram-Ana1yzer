#pragma once

#include "pkb.h"
#include "proc_table.h"
#include "assign_table.h"

int PKB::InsertProc(ProcName proc_name) { return proc_table_.InsertProcName(proc_name); }

ProcList PKB::GetAllProc() { return proc_table_.GetProcTable(); }

int PKB::InsertAssignStmt(StmtNum statement_number, Stmt assign_statement) {
  // to-do: insert to statement table (to be added when integrating with Sophie's StmtTable class)
  return assign_table_.InsertAssignStmt(statement_number, assign_statement);
}

StmtList PKB::GetAllAssignStmt() { return assign_table_.GetAssignTable(); }
