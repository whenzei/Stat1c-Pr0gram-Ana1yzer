#include "pql_extractor.h"

PqlExtractor::PqlExtractor(PKB* pkb) {
  ae_ = AffectsExtractor(pkb);
  ne_ = NextExtractor(pkb);
}

/**********************************
 * NextT  *
 **********************************/

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

/**********************************
 * Affects *
 **********************************/

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

/**********************************
 * AffectsT *
 **********************************/

bool PqlExtractor::IsAffectsT(StmtNum stmt_1, StmtNum stmt_2) {
  return ae_.IsAffectsT(stmt_1, stmt_2);
}

bool PqlExtractor::IsAffectsT(StmtNum stmt) {
  return ae_.IsAffectsT(stmt);
}

bool PqlExtractor::IsAffectedT(StmtNum stmt) {
  return ae_.IsAffectedT(stmt);
}

VertexSet PqlExtractor::GetAffectsT(StmtNum stmt) {
  return ae_.GetAffectsT(stmt);
}

VertexSet PqlExtractor::GetAffectedByT(StmtNum stmt) {
  return ae_.GetAffectedByT(stmt);
}

VertexSet PqlExtractor::GetAllAffectsT() {
  return ae_.GetAllAffectsT();
}

VertexSet PqlExtractor::GetAllAffectedByT() {
  return ae_.GetAllAffectedByT();
}

AffectsTable PqlExtractor::GetAffectsTTable() {
  return ae_.GetAffectsTTable();
}

AffectsTable PqlExtractor::GetAffectedByTTable() {
  return ae_.GetAffectedByTTable();
}

/**********************************
 * AffectsBip *
 **********************************/

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

/**********************************
 * AffectsBipT *
 **********************************/

bool PqlExtractor::IsAffectsBipT(StmtNum stmt_1, StmtNum stmt_2) {
  return ae_.IsAffectsT(stmt_1, stmt_2, true);
}

bool PqlExtractor::IsAffectsBipT(StmtNum stmt) {
  return ae_.IsAffectsT(stmt, true);
}

bool PqlExtractor::IsAffectedBipT(StmtNum stmt) {
  return ae_.IsAffectedT(stmt, true);
}

VertexSet PqlExtractor::GetAffectsBipT(StmtNum stmt) {
  return ae_.GetAffectsT(stmt, true);
}

VertexSet PqlExtractor::GetAffectedByBipT(StmtNum stmt) {
  return ae_.GetAffectedByT(stmt, true);
}

VertexSet PqlExtractor::GetAllAffectsBipT() {
  return ae_.GetAllAffectsT(true);
}

VertexSet PqlExtractor::GetAllAffectedByBipT() {
  return ae_.GetAllAffectedByT(true);
}

AffectsTable PqlExtractor::GetAffectsBipTTable() {
  return ae_.GetAffectsBipTTable();
}

AffectsTable PqlExtractor::GetAffectedByBipTTable() {
  return ae_.GetAffectedByBipTTable();
}