#pragma once

#include "stmtlist_table.h"

void StmtListTable::InsertStmt(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  // No need to check for empty string or duplicate because insertion into StmtListTable
  // would only happen after the insertion into StmtTable. StmtTable would have returned false 
  // if insertion was unsuccessful 
  stmtlist_map_[stmtlist_index].push_back(stmt_num);
}

StmtNumList StmtListTable::GetStmtNumList(StmtListIndex stmtlist_index) {
  return stmtlist_map_[stmtlist_index];
}