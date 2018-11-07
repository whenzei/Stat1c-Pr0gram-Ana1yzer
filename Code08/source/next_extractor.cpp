#include "next_extractor.h"

NextExtractor::NextExtractor() {}

NextExtractor::NextExtractor(PKB* pkb) {
  pkb_ = pkb;
  next_t_table_ = NextTTable();
  previous_t_table_ = NextTTable();
}

bool NextExtractor::IsNextT(StmtNum previous_stmt, StmtNum next_stmt) {
  // if is pre-cached
  if (!next_t_table_.IsEmpty()) {
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
  if (!next_t_table_.IsEmpty()) {
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
  if (!previous_t_table_.IsEmpty()) {
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
  if (next_t_table_.IsEmpty()) {
    StmtNumList prev_list = pkb_->GetAllPrevious();
    StmtNumPairList res_list;

    for (auto& prev : prev_list) {
      BFSSetNextTTables(prev);
    }
  }
  return next_t_table_.GetAdjSet();
}

void NextExtractor::BFSSetNextTTables(StmtNum start) {
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

    StmtNumList curr_prev_stmts = pkb_->GetNext(curr_stmt);
    for (StmtNum curr_next : curr_prev_stmts) {
      if (!visited.count(curr_next)) {
        prev_stmt_queue.push(curr_next);
      }
    }
  }
}