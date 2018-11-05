#include "pql_extractor.h"

PqlExtractor::PqlExtractor(PKB* pkb) {
  ae_ = AffectsExtractor(pkb);
  ne_ = NextExtractor(pkb);
}

bool PqlExtractor::IsNextT(StmtNum previous_stmt, StmtNum next_stmt) {
  return ne_.IsNextT(previous_stmt, next_stmt);
}

bool PqlExtractor::IsNextT(StmtNum stmt_num) {
  return ne_.IsNextT(stmt_num);
}

bool PqlExtractor::IsPreviousT(StmtNum stmt_num) {
  return ne_.IsPreviousT(stmt_num);
}

StmtNumList PqlExtractor::GetNextT(StmtNum stmt_num) {
  return ne_.GetNextT(stmt_num);
}

StmtNumList PqlExtractor::GetPreviousT(StmtNum stmt_num) {
  return ne_.GetPreviousT(stmt_num);
}

StmtNumPairList PqlExtractor::GetAllNextTPairs() {
  return ne_.GetAllNextTPairs();
}

bool PqlExtractor::IsAffects(StmtNum stmt_1, StmtNum stmt_2) {
  return ae_.IsAffects(stmt_1, stmt_2);
}

bool PqlExtractor::IsAffects(StmtNum stmt_num) {
  return ae_.IsAffects(stmt_num);
}

bool PqlExtractor::IsAffected(StmtNum stmt_num) {
  return ae_.IsAffected(stmt_num);
}

VertexSet PqlExtractor::GetAffects(StmtNum stmt_1) {
  return ae_.GetAffects(stmt_1);
}

VertexSet PqlExtractor::GetAffectedBy(StmtNum stmt_num) {
  return ae_.GetAffectedBy(stmt_num);
}

VertexSet PqlExtractor::GetAllAffects() {
  return ae_.GetAllAffects();
}

VertexSet PqlExtractor::GetAllAffectedBy() {
  return ae_.GetAllAffectedBy();
}

AffectsTable PqlExtractor::GetAffectsTable() {
  return ae_.GetAffectsTable();
}

AffectsTable PqlExtractor::GetAffectedByTable() {
  return ae_.GetAffectedByTable();
}

bool PqlExtractor::IsAffectsBip(StmtNum stmt_1, StmtNum stmt_2) {
  return ae_.IsAffects(stmt_1, stmt_2, true);
}

bool PqlExtractor::IsAffectsBip(StmtNum stmt_num) {
  return ae_.IsAffects(stmt_num, true);
}

bool PqlExtractor::IsAffectedBip(StmtNum stmt_num) {
  return ae_.IsAffected(stmt_num, true);
}

VertexSet PqlExtractor::GetAffectsBip(StmtNum stmt_1) {
  return ae_.GetAffects(stmt_1, true);
}

VertexSet PqlExtractor::GetAffectedByBip(StmtNum stmt_num) {
  return ae_.GetAffectedBy(stmt_num, true);
}

AffectsTable PqlExtractor::GetAffectsBipTable() {
  return ae_.GetAffectsBipTable();
}

AffectsTable PqlExtractor::GetAffectedByBipTable() {
  return ae_.GetAffectedByBipTable();
}


