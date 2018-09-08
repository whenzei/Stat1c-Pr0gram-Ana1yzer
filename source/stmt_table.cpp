#pragma once 

#include "stmt_table.h"

bool StmtTable::InsertStmt(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  if (stmt_num.compare("") == 0) {
    return false;
  } else if (stmt_map_.find(stmt_num) != stmt_map_.end()) {
    return false;
  } else {
    stmt_map_[stmt_num] = stmtlist_index;
    return true;
  }
}

StmtListIndex StmtTable::GetStmtListIndex(StmtNum stmt_num) {
  return stmt_map_[stmt_num];
}