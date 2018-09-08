#pragma once

#include "stmt_type_list.h"

StmtTypeList::StmtTypeList() { 
  for (int i = 0; i < kSizeOfVector; i++) {
    stmt_type_vector_.push_back(StmtNumList());
  }
}

void StmtTypeList::InsertAssignStmt(StmtNum stmt_num) {
  // No need to check for empty string or duplicate because insertion into
  // StmtTypeList would only happen after the insertion into StmtTable.
  // StmtTable would have returned false if insertion was unsuccessful
  stmt_type_vector_.at(kAll).push_back(stmt_num);
  stmt_type_vector_.at(kAssign).push_back(stmt_num);
}

void StmtTypeList::InsertWhileStmt(StmtNum stmt_num) {
  stmt_type_vector_.at(kAll).push_back(stmt_num);
  stmt_type_vector_.at(kWhile).push_back(stmt_num);
}

void StmtTypeList::InsertIfStmt(StmtNum stmt_num) {
  stmt_type_vector_.at(kAll).push_back(stmt_num);
  stmt_type_vector_.at(kIf).push_back(stmt_num);
}

void StmtTypeList::InsertReadStmt(StmtNum stmt_num) {
  stmt_type_vector_.at(kAll).push_back(stmt_num);
  stmt_type_vector_.at(kRead).push_back(stmt_num);
}

void StmtTypeList::InsertPrintStmt(StmtNum stmt_num) {
  stmt_type_vector_.at(kAll).push_back(stmt_num);
  stmt_type_vector_.at(kPrint).push_back(stmt_num);
}

StmtNumList StmtTypeList::GetAllStmt() { return stmt_type_vector_.at(kAll); }

StmtNumList StmtTypeList::GetAllAssignStmt() { return stmt_type_vector_.at(kAssign); }

StmtNumList StmtTypeList::GetAllWhileStmt() { return stmt_type_vector_.at(kWhile); }

StmtNumList StmtTypeList::GetAllIfStmt() { return stmt_type_vector_.at(kIf); }

StmtNumList StmtTypeList::GetAllReadStmt() { return stmt_type_vector_.at(kRead); }

StmtNumList StmtTypeList::GetAllPrintStmt() { return stmt_type_vector_.at(kPrint); }