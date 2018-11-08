#include "next_extractor.h"

NextExtractor::NextExtractor() {}

NextExtractor::NextExtractor(PKB* pkb) { pkb_ = pkb; }

bool NextExtractor::IsNextT(StmtNum previous_stmt, StmtNum next_stmt) {
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

  StmtNumSet temp_next_stmts = pkb_->GetNext(previous_stmt);
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

    StmtNumSet curr_next_stmts = pkb_->GetNext(curr_stmt);
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

StmtNumSet NextExtractor::GetNextT(StmtNum stmt_num) {
  StmtNumSet res_list;
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> next_stmt_queue;

  ProcName p = pkb_->GetProcOfStmt(stmt_num);
  if (p.empty()) {
    return StmtNumSet();
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
    res_list.emplace(curr_stmt);

    StmtNumSet curr_next_stmts = pkb_->GetNext(curr_stmt);
    for (StmtNum curr_next : curr_next_stmts) {
      if (visited_stmts.count(curr_next) == 0) {
        next_stmt_queue.push(curr_next);
      }
    }
  }

  return res_list;
}

StmtNumSet NextExtractor::GetPreviousT(StmtNum stmt_num) {
  StmtNumSet res_list;
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> prev_stmt_queue;

  ProcName p = pkb_->GetProcOfStmt(stmt_num);

  if (p.empty()) {
    return StmtNumSet();
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
    res_list.emplace(curr_stmt);

    StmtNumSet curr_prev_stmts = pkb_->GetPrevious(curr_stmt);
    for (StmtNum curr_prev : curr_prev_stmts) {
      if (visited_stmts.count(curr_prev) == 0) {
        prev_stmt_queue.push(curr_prev);
      }
    }
  }

  return res_list;
}

StmtNumPairSet NextExtractor::GetAllNextTPairs() {
  StmtNumSet prev_list = pkb_->GetAllPrevious();
  StmtNumPairSet res_list;

  for (auto& prev : prev_list) {
    FormPairBFS(prev, &res_list);
  }

  return res_list;
}

void NextExtractor::FormPairBFS(StmtNum start, StmtNumPairSet* res_list) {
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> prev_stmt_queue;

  for (auto& next_stmt : pkb_->GetNext(start)) {
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

    (*res_list).emplace(start, curr_stmt);

    StmtNumSet curr_prev_stmts = pkb_->GetNext(curr_stmt);
    for (StmtNum curr_next : curr_prev_stmts) {
      if (visited_stmts.count(curr_next) == 0) {
        prev_stmt_queue.push(curr_next);
      }
    }
  }
}