#pragma once

#include "stmt_type_list.h"

// add more entity types as needed
StmtTypeList::StmtTypeList() {
  stmt_type_map_.emplace(PqlDeclarationEntity::kAll, StmtNumSet());
  stmt_type_map_.emplace(PqlDeclarationEntity::kAssign, StmtNumSet());
  stmt_type_map_.emplace(PqlDeclarationEntity::kWhile, StmtNumSet());
  stmt_type_map_.emplace(PqlDeclarationEntity::kIf, StmtNumSet());
  stmt_type_map_.emplace(PqlDeclarationEntity::kRead, StmtNumSet());
  stmt_type_map_.emplace(PqlDeclarationEntity::kPrint, StmtNumSet());
  stmt_type_map_.emplace(PqlDeclarationEntity::kCall, StmtNumSet());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kAll, StmtNumPairSet());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kAssign, StmtNumPairSet());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kWhile, StmtNumPairSet());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kIf, StmtNumPairSet());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kRead, StmtNumPairSet());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kPrint, StmtNumPairSet());
  stmt_type_twin_map_.emplace(PqlDeclarationEntity::kCall, StmtNumPairSet());
}

void StmtTypeList::InsertStmt(StmtNum stmt_num, StmtType type) {
  stmt_type_map_.at(type).insert(stmt_num);
  stmt_type_map_.at(StmtType::kAll).insert(stmt_num);
  stmt_type_twin_map_.at(type).insert(std::make_pair(stmt_num, stmt_num));
  stmt_type_twin_map_.at(StmtType::kAll)
      .insert(std::make_pair(stmt_num, stmt_num));
}

StmtNumSet StmtTypeList::GetAllStmt() {
  return stmt_type_map_.at(StmtType::kAll);
}

StmtNumPairSet StmtTypeList::GetAllStmtTwin() {
  return stmt_type_twin_map_.at(StmtType::kAll);
}

StmtNumSet StmtTypeList::GetAllAssignStmt() {
  return stmt_type_map_.at(StmtType::kAssign);
}

StmtNumPairSet StmtTypeList::GetAllAssignStmtTwin() {
  return stmt_type_twin_map_.at(StmtType::kAssign);
}

StmtNumSet StmtTypeList::GetAllWhileStmt() {
  return stmt_type_map_.at(StmtType::kWhile);
}

StmtNumPairSet StmtTypeList::GetAllWhileStmtTwin() {
  return stmt_type_twin_map_.at(StmtType::kWhile);
}

StmtNumSet StmtTypeList::GetAllIfStmt() {
  return stmt_type_map_.at(StmtType::kIf);
}

StmtNumPairSet StmtTypeList::GetAllIfStmtTwin() {
  return stmt_type_twin_map_.at(StmtType::kIf);
}

StmtNumSet StmtTypeList::GetAllReadStmt() {
  return stmt_type_map_.at(StmtType::kRead);
}

StmtNumPairSet StmtTypeList::GetAllReadStmtTwin() {
  return stmt_type_twin_map_.at(StmtType::kRead);
}

StmtNumSet StmtTypeList::GetAllPrintStmt() {
  return stmt_type_map_.at(StmtType::kPrint);
}

StmtNumPairSet StmtTypeList::GetAllPrintStmtTwin() {
  return stmt_type_twin_map_.at(StmtType::kPrint);
}

StmtNumSet StmtTypeList::GetAllCallStmt() {
  return stmt_type_map_.at(StmtType::kCall);
}

StmtNumPairSet StmtTypeList::GetAllCallStmtTwin() {
  return stmt_type_twin_map_.at(StmtType::kCall);
}
