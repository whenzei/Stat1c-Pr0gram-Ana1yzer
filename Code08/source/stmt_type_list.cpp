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
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kAll, StmtNumPairList());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kAssign, StmtNumPairList());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kWhile, StmtNumPairList());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kIf, StmtNumPairList());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kRead, StmtNumPairList());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kPrint, StmtNumPairList());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kCall, StmtNumPairList());
}

void StmtTypeList::InsertStmt(StmtNum stmt_num, PqlDeclarationEntity type) {
  stmt_type_map_.at(type).push_back(stmt_num);
  stmt_type_map_.at(PqlDeclarationEntity::kAll).push_back(stmt_num);
  stmt_type_twin_map_.at(type).push_back(std::make_pair(stmt_num, stmt_num));
  stmt_type_twin_map_.at(PqlDeclarationEntity::kAll)
      .push_back(std::make_pair(stmt_num, stmt_num));
}

StmtNumList StmtTypeList::GetAllStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kAll);
}

StmtNumPairList StmtTypeList::GetAllStmtTwin() {
  return stmt_type_twin_map_.at(PqlDeclarationEntity::kAll);
}

StmtNumList StmtTypeList::GetAllAssignStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kAssign);
}

StmtNumPairList StmtTypeList::GetAllAssignStmtTwin() {
  return stmt_type_twin_map_.at(PqlDeclarationEntity::kAssign);
}

StmtNumList StmtTypeList::GetAllWhileStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kWhile);
}

StmtNumPairList StmtTypeList::GetAllWhileStmtTwin() {
  return stmt_type_twin_map_.at(PqlDeclarationEntity::kWhile);
}

StmtNumList StmtTypeList::GetAllIfStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kIf);
}

StmtNumPairList StmtTypeList::GetAllIfStmtTwin() {
  return stmt_type_twin_map_.at(PqlDeclarationEntity::kIf);
}

StmtNumList StmtTypeList::GetAllReadStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kRead);
}

StmtNumPairList StmtTypeList::GetAllReadStmtTwin() {
  return stmt_type_twin_map_.at(PqlDeclarationEntity::kRead);
}

StmtNumList StmtTypeList::GetAllPrintStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kPrint);
}

StmtNumPairList StmtTypeList::GetAllPrintStmtTwin() {
  return stmt_type_twin_map_.at(PqlDeclarationEntity::kPrint);
}

StmtNumList StmtTypeList::GetAllCallStmt() {
  return stmt_type_map_.at(PqlDeclarationEntity::kCall);
}

StmtNumPairList StmtTypeList::GetAllCallStmtTwin() {
  return stmt_type_twin_map_.at(PqlDeclarationEntity::kCall);
}
