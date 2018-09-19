#pragma once

#include "stmt_type_list.h"

StmtTypeList::StmtTypeList() { 
  for (int i = 0; i < kSizeOfVector; i++) {
    stmt_type_vector_.push_back(StmtNumList());
  }
}

void StmtTypeList::InsertStmt(StmtNum stmt_num, StmtTypeEnum type) {
	stmt_type_vector_.at(kAll).push_back(stmt_num);
	stmt_type_vector_.at(type).push_back(stmt_num);
}

StmtNumList StmtTypeList::GetAllStmt() { return stmt_type_vector_.at(kAll); }

StmtNumList StmtTypeList::GetAllAssignStmt() { return stmt_type_vector_.at(kAssign); }

StmtNumList StmtTypeList::GetAllWhileStmt() { return stmt_type_vector_.at(kWhile); }

StmtNumList StmtTypeList::GetAllIfStmt() { return stmt_type_vector_.at(kIf); }

StmtNumList StmtTypeList::GetAllReadStmt() { return stmt_type_vector_.at(kRead); }

StmtNumList StmtTypeList::GetAllPrintStmt() { return stmt_type_vector_.at(kPrint); }