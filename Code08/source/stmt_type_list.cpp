#pragma once

#include "stmt_type_list.h"

// add more entity types as needed
StmtTypeList::StmtTypeList() {
  stmt_type_map_.emplace(PqlDeclarationEntity::kAll, StmtNumList());
  stmt_type_map_.emplace(PqlDeclarationEntity::kAssign, StmtNumList());
  stmt_type_map_.emplace(PqlDeclarationEntity::kWhile, StmtNumList());
  stmt_type_map_.emplace(PqlDeclarationEntity::kIf, StmtNumList());
  stmt_type_map_.emplace(PqlDeclarationEntity::kRead, StmtNumList());
  stmt_type_map_.emplace(PqlDeclarationEntity::kPrint, StmtNumList());
  stmt_type_map_.emplace(PqlDeclarationEntity::kCall, StmtNumList());
}

void StmtTypeList::InsertStmt(StmtNum stmt_num, PqlDeclarationEntity type) {
  stmt_type_map_.at(type).push_back(stmt_num);
  stmt_type_map_.at(PqlDeclarationEntity::kAll).push_back(stmt_num);
}

StmtNumList StmtTypeList::GetAllStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kAll);
}

StmtNumList StmtTypeList::GetAllAssignStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kAssign);
}

StmtNumList StmtTypeList::GetAllWhileStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kWhile);
}

StmtNumList StmtTypeList::GetAllIfStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kIf);
}

StmtNumList StmtTypeList::GetAllReadStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kRead);
}

StmtNumList StmtTypeList::GetAllPrintStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kPrint);
}

StmtNumList StmtTypeList::GetAllCallStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kCall);
}