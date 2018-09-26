#pragma once

#include "stmtlist_table.h"

void StmtListTable::InsertStmt(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  stmtlist_map_[stmtlist_index].push_back(stmt_num);
}

StmtNumList StmtListTable::GetStmtNumList(StmtListIndex stmtlist_index) {
  return stmtlist_map_[stmtlist_index];
}
