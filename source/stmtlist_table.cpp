#pragma once

#include "stmtlist_table.h"

StmtListTable::StmtListTable() { max_stmtlist_index_ = -1; }

void StmtListTable::InsertStmt(StmtNum stmt_num, StmtListIndex stmtlist_index) {
  // No need to check for empty string or duplicate because insertion into StmtListTable
  // would only happen after the insertion into StmtTable. StmtTable would have returned false 
  // if insertion was unsuccessful 
  stmtlist_map_[stmtlist_index].push_back(stmt_num);
  if (stmtlist_index > max_stmtlist_index_) {
    max_stmtlist_index_ = stmtlist_index;
  }
}

StmtNumList StmtListTable::GetStmtNumList(StmtListIndex stmtlist_index) {
  return stmtlist_map_[stmtlist_index];
}

StmtListIndex StmtListTable::GetNextStmtListIndex() { return max_stmtlist_index_ + 1; }

pair<StmtListIndex, StmtListIndex> StmtListTable::GetNextTwoStmtListIndices() {
  return pair<StmtListIndex, StmtListIndex>(max_stmtlist_index_ + 1,
                                            max_stmtlist_index_ + 2);
}
