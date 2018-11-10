#include "next_extractor.h"

NextExtractor::NextExtractor() {}

NextExtractor::NextExtractor(PKB* pkb) {
  pkb_ = pkb;
  next_t_table_ = NextTTable();
  previous_t_table_ = NextTTable();

  // set up all types to be false
  type_done_map_.emplace(StmtType::kAssign, false);
  type_done_map_.emplace(StmtType::kCall, false);
  type_done_map_.emplace(StmtType::kIf, false);
  type_done_map_.emplace(StmtType::kPrint, false);
  type_done_map_.emplace(StmtType::kRead, false);
  type_done_map_.emplace(StmtType::kWhile, false);
  type_done_map_.emplace(StmtType::kAll, false);

  // set up typed Next* table
  typed_next_t_table_.emplace(StmtType::kAssign, NextTTable());
  typed_next_t_table_.emplace(StmtType::kCall, NextTTable());
  typed_next_t_table_.emplace(StmtType::kIf, NextTTable());
  typed_next_t_table_.emplace(StmtType::kPrint, NextTTable());
  typed_next_t_table_.emplace(StmtType::kRead, NextTTable());
  typed_next_t_table_.emplace(StmtType::kWhile, NextTTable());
  typed_next_t_table_.emplace(StmtType::kAll, NextTTable());
}

bool NextExtractor::IsNextT(StmtNum previous_stmt, StmtNum next_stmt) {
  // if is pre-cached
  StmtType stmt_type = pkb_->GetStmtType(previous_stmt);
  if (type_done_map_.count(stmt_type) && type_done_map_[stmt_type]) {
    return next_t_table_.GetNeighboursSet(previous_stmt).count(next_stmt);
  }

  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> next_stmt_queue;

  ProcName p1 = pkb_->GetProcOfStmt(previous_stmt);
  ProcName p2 = pkb_->GetProcOfStmt(next_stmt);

  if (p1.empty() || p2.empty()) {
    return false;
  }

  if (p1 != p2) {
    return false;
  }

  StmtNumList temp_next_stmts = pkb_->GetNext(previous_stmt);
  for (auto& temp_next_stmt : temp_next_stmts) {
    next_stmt_queue.push(temp_next_stmt);
  }

  // BFS
  while (!next_stmt_queue.empty()) {
    StmtNum curr_stmt = next_stmt_queue.front();
    next_stmt_queue.pop();

    if (visited_stmts.count(curr_stmt)) {
      continue;
    }

    visited_stmts.emplace(curr_stmt);
    if (curr_stmt == next_stmt) {
      return true;
    }

    StmtNumList curr_next_stmts = pkb_->GetNext(curr_stmt);
    for (StmtNum curr_next : curr_next_stmts) {
      if (visited_stmts.count(curr_next) == 0) {
        next_stmt_queue.push(curr_next);
      }
    }
  }
  return false;
}

bool NextExtractor::IsNextT(StmtNum stmt_num) { return pkb_->IsNext(stmt_num); }

bool NextExtractor::IsPreviousT(StmtNum stmt_num) {
  return pkb_->IsPrevious(stmt_num);
}

StmtNumList NextExtractor::GetNextT(StmtNum stmt_num) {
  // if pre-cached
  StmtType stmt_type = pkb_->GetStmtType(stmt_num);
  if (type_done_map_.count(stmt_type) && type_done_map_[stmt_type]) {
    return next_t_table_.GetNeighboursList(stmt_num);
  }

  StmtNumList res_list;
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> next_stmt_queue;

  ProcName p = pkb_->GetProcOfStmt(stmt_num);
  if (p.empty()) {
    return StmtNumList();
  }

  for (auto& next_stmt : pkb_->GetNext(stmt_num)) {
    next_stmt_queue.push(next_stmt);
  }

  // BFS
  while (!next_stmt_queue.empty()) {
    StmtNum curr_stmt = next_stmt_queue.front();
    next_stmt_queue.pop();

    if (visited_stmts.count(curr_stmt)) {
      continue;
    }
    visited_stmts.emplace(curr_stmt);
    res_list.push_back(curr_stmt);

    StmtNumList curr_next_stmts = pkb_->GetNext(curr_stmt);
    for (StmtNum curr_next : curr_next_stmts) {
      if (visited_stmts.count(curr_next) == 0) {
        next_stmt_queue.push(curr_next);
      }
    }
  }

  return res_list;
}

StmtNumList NextExtractor::GetPreviousT(StmtNum stmt_num) {
  // pre-cached
  StmtType stmt_type = pkb_->GetStmtType(stmt_num);
  if (type_done_map_.count(stmt_type) && type_done_map_[stmt_type]) {
    return previous_t_table_.GetNeighboursList(stmt_num);
  }

  StmtNumList res_list;
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> prev_stmt_queue;

  ProcName p = pkb_->GetProcOfStmt(stmt_num);

  if (p.empty()) {
    return StmtNumList();
  }

  for (auto& next_stmt : pkb_->GetPrevious(stmt_num)) {
    prev_stmt_queue.push(next_stmt);
  }

  // BFS
  while (!prev_stmt_queue.empty()) {
    StmtNum curr_stmt = prev_stmt_queue.front();
    prev_stmt_queue.pop();

    if (visited_stmts.count(curr_stmt)) {
      continue;
    }

    visited_stmts.emplace(curr_stmt);
    res_list.push_back(curr_stmt);

    StmtNumList curr_prev_stmts = pkb_->GetPrevious(curr_stmt);
    for (StmtNum curr_prev : curr_prev_stmts) {
      if (visited_stmts.count(curr_prev) == 0) {
        prev_stmt_queue.push(curr_prev);
      }
    }
  }

  return res_list;
}

NextTMap NextExtractor::GetNextTMap() {
  if (!type_done_map_[StmtType::kAll]) {
    SetNextTTables();
  }

  return next_t_table_.GetAdjSet();
}

void NextExtractor::SetNextTTables() {
  for (auto& kv : type_done_map_) {
    StmtType type = kv.first;
    if (!type_done_map_[type]) {
      SetNextTTables(type);
    }
  }

  type_done_map_[StmtType::kAll] = true;
}

void NextExtractor::SetNextTTables(StmtType type) {
  StmtNumList stmts;

  switch (type) {
    case StmtType::kAssign:
      stmts = pkb_->GetAllAssignStmt();
      break;
    case StmtType::kCall:
      stmts = pkb_->GetAllCallStmt();
      break;
    case StmtType::kIf:
      stmts = pkb_->GetAllIfStmt();
      break;
    case StmtType::kPrint:
      stmts = pkb_->GetAllPrintStmt();
      break;
    case StmtType::kRead:
      stmts = pkb_->GetAllReadStmt();
      break;
    case StmtType::kWhile:
      stmts = pkb_->GetAllWhileStmt();
  }

  for (auto& stmt : stmts) {
    BFSSetNextTTables(stmt, type);
  }

  type_done_map_[type] = true;
}

void NextExtractor::BFSSetNextTTables(StmtNum start, StmtType type) {
  VisitedMap visited;
  queue<StmtNum> prev_stmt_queue;

  for (auto& next_stmt : pkb_->GetNext(start)) {
    prev_stmt_queue.emplace(next_stmt);
  }

  // BFS
  while (!prev_stmt_queue.empty()) {
    StmtNum curr_stmt = prev_stmt_queue.front();
    prev_stmt_queue.pop();

    if (visited.count(curr_stmt)) {
      continue;
    }

    visited.emplace(curr_stmt, true);

    next_t_table_.AddEdge(start, curr_stmt);
    previous_t_table_.AddEdge(curr_stmt, start);

    typed_next_t_table_[type].AddEdge(start, curr_stmt);

    StmtNumList curr_prev_stmts = pkb_->GetNext(curr_stmt);
    for (StmtNum curr_next : curr_prev_stmts) {
      if (!visited.count(curr_next)) {
        prev_stmt_queue.push(curr_next);
      }
    }
  }
}

/* Helper API for PQLEvaluator to call specific typed NextT table */

NextTMap NextExtractor::GetAssignNextTMap() {
  return GetTypedNextTMap(StmtType::kAssign);
}

NextTMap NextExtractor::GetWhileNextTMap() {
  return GetTypedNextTMap(StmtType::kWhile);
}

NextTMap NextExtractor::GetIfNextTMap() {
  return GetTypedNextTMap(StmtType::kIf);
}

NextTMap NextExtractor::GetCallNextTMap() {
  return GetTypedNextTMap(StmtType::kCall);
}

NextTMap NextExtractor::GetReadNextTMap() {
  return GetTypedNextTMap(StmtType::kRead);
}

NextTMap NextExtractor::GetPrintNextTMap() {
  return GetTypedNextTMap(StmtType::kPrint);
}

NextTMap NextExtractor::GetTypedNextTMap(StmtType type) {
  if (!type_done_map_[type]) {
    SetNextTTables(type);
  }
  
  return typed_next_t_table_[type].GetAdjSet();
}