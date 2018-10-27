#pragma once 

#include "stmt_table.h"

bool StmtTable::InsertStmt(StmtNum stmt_num, StmtType stmt_type, ProcIndex proc_index) {
  if (stmt_map_.find(stmt_num) != stmt_map_.end()) {
    return false;
  } else {
    stmt_map_[stmt_num].first = stmt_type;
    stmt_map_[stmt_num].second = proc_index;
    return true;
  }
}

ProcIndex StmtTable::GetProcOfStmt(StmtNum stmt_num) {
  if (stmt_map_.find(stmt_num) != stmt_map_.end()) {
    return stmt_map_[stmt_num].second;
  } else {
    return ProcIndex();
  }
}

StmtType StmtTable::GetStmtType(StmtNum stmt_num) {
  if (stmt_map_.find(stmt_num) != stmt_map_.end()) {
    return stmt_map_[stmt_num].first;
  } else {
    return StmtType();
  }
}

bool StmtTable::IsStmtNum(StmtNum stmt_num) {
  return stmt_map_.find(stmt_num) != stmt_map_.end();
}