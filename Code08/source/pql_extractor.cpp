#include <unordered_set>

#include "pql_extractor.h"

using std::unordered_set;

PqlExtractor::PqlExtractor(PKB pkb) { pkb_ = pkb; }

bool PqlExtractor::IsNextT(StmtNum previous_stmt, StmtNum next_stmt) {
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> next_stmt_queue;

  StmtNumList temp_next_stmts = pkb_.GetNext(previous_stmt);
  for (auto& temp_next_stmt : temp_next_stmts) {
    next_stmt_queue.push(temp_next_stmt);
  }

  // BFS
  while (!next_stmt_queue.empty()) {
    StmtNum curr_stmt = next_stmt_queue.front();
    next_stmt_queue.pop();

    if (visited_stmts.count(curr_stmt) == 1) {
      continue;
    }

    visited_stmts.emplace(curr_stmt);
    if (curr_stmt == next_stmt) {
      return true;
    }

    StmtNumList curr_next_stmts = pkb_.GetNext(curr_stmt);
    for (StmtNum curr_next : curr_next_stmts) {
      if (visited_stmts.count(curr_next) == 0) {
        next_stmt_queue.push(curr_next);
      }
    }
  }
  return false;
}

bool PqlExtractor::IsNextT(StmtNum stmt_num) {
  return !(pkb_.GetPrevious(stmt_num)).empty();
}

bool PqlExtractor::IsPreviousT(StmtNum stmt_num) {
  return !(pkb_.GetNext(stmt_num)).empty();
}

StmtNumList PqlExtractor::GetNextT(StmtNum stmt_num) {
  StmtNumList res_list;
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> next_stmt_queue;

  for (auto& next_stmt : pkb_.GetNext(stmt_num)) {
    next_stmt_queue.push(next_stmt);
  }

  // BFS
  while (!next_stmt_queue.empty()) {
    StmtNum curr_stmt = next_stmt_queue.front();
    next_stmt_queue.pop();

    if (visited_stmts.count(curr_stmt) == 1) {
      continue;
    }
    visited_stmts.emplace(curr_stmt);
    res_list.push_back(curr_stmt);

    StmtNumList curr_next_stmts = pkb_.GetNext(curr_stmt);
    for (StmtNum curr_next : curr_next_stmts) {
      if (visited_stmts.count(curr_next) == 0) {
        next_stmt_queue.push(curr_next);
      }
    }
  }

  return res_list;
}

StmtNumList PqlExtractor::GetPreviousT(StmtNum stmt_num) {
  StmtNumList res_list;
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> prev_stmt_queue;

  for (auto& next_stmt : pkb_.GetPrevious(stmt_num)) {
    prev_stmt_queue.push(next_stmt);
  }

  // BFS
  while (!prev_stmt_queue.empty()) {
    StmtNum curr_stmt = prev_stmt_queue.front();
    prev_stmt_queue.pop();

    if (visited_stmts.count(curr_stmt) == 1) {
      continue;
    }

    visited_stmts.emplace(curr_stmt);
    res_list.push_back(curr_stmt);

    StmtNumList curr_prev_stmts = pkb_.GetPrevious(curr_stmt);
    for (StmtNum curr_prev : curr_prev_stmts) {
      if (visited_stmts.count(curr_prev) == 0) {
        prev_stmt_queue.push(curr_prev);
      }
    }
  }

  return res_list;
}

StmtNumPairList PqlExtractor::GetAllNextTPairs() {
  StmtNumList prev_list = pkb_.GetAllPrevious();
  StmtNumPairList res_list;

  for (auto& prev : prev_list) {
    FormPairBFS(prev, &res_list);
  }

  return res_list;
}



bool PqlExtractor::isAffects(StmtNum stmt_1, StmtNum stmt_2) {
  
  ProcName p1 = pkb_.GetProcOfStmt(stmt_1);
  ProcName p2 = pkb_.GetProcOfStmt(stmt_2);

  if (p1 == ProcName() || p2== ProcName()) {
    return false;
  }

  // Check if both stmts are assignment
  if (pkb_.GetStmtType(stmt_1) != StmtType::kAssign &&
      pkb_.GetStmtType(stmt_2) != StmtType::kAssign) {
    return false;
  }

  VarIndex modified_var = pkb_.GetModifiedVarS(stmt_1).front();

  // Check if variable modified in stmt_1 is used in stmt_2
  if (!pkb_.IsUsedByS(stmt_2, pkb_.GetVarName(modified_var))) {
    return false;
  }
  
  curr_affects_cfg_ = pkb_.GetCFG(p1);

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(stmt_1);

  bool flag = false;
  for (Vertex neighbour : neighbours) {
    flag = flag || DfsAffects(neighbour, stmt_1, stmt_2);
  }

  ClearVisitedMap();
  return flag;
}



// Helper Methods

void PqlExtractor::FormPairBFS(StmtNum start, StmtNumPairList* res_list) {
  unordered_set<StmtNum> visited_stmts;
  queue<StmtNum> prev_stmt_queue;

  for (auto& next_stmt : pkb_.GetNext(start)) {
    prev_stmt_queue.push(next_stmt);
  }

  // BFS
  while (!prev_stmt_queue.empty()) {
    StmtNum curr_stmt = prev_stmt_queue.front();
    prev_stmt_queue.pop();

    if (visited_stmts.count(curr_stmt) == 1) {
      continue;
    }

    visited_stmts.emplace(curr_stmt);

    (*res_list).push_back(make_pair(start, curr_stmt));

    StmtNumList curr_prev_stmts = pkb_.GetNext(curr_stmt);
    for (StmtNum curr_next : curr_prev_stmts) {
      if (visited_stmts.count(curr_next) == 0) {
        prev_stmt_queue.push(curr_next);
      }
    }
  }
}


bool PqlExtractor::DfsAffects(Vertex curr, Vertex start, Vertex target) {
  if (curr_visited_.count(curr) == 1) {
    return false;
  }

  StmtType curr_stmt_type = pkb_.GetStmtType(curr);
  curr_visited_.emplace(curr, true);

  if (curr_stmt_type == StmtType::kAssign && target == curr) {
    return true;
  }

  if (curr_stmt_type == StmtType::kCall || curr_stmt_type == StmtType::kRead ||
      curr_stmt_type == StmtType::kAssign) {
    VarIndex start_var = pkb_.GetModifiedVarS(start).front();
    if (pkb_.IsModifiedByS(curr, pkb_.GetVarName(start_var))) {
      return false;
    }
  }

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(curr);
  bool flag = false;
  for (Vertex neighbour : neighbours) {
    flag = flag || DfsAffects(neighbour, start, target);
    if (flag) {
      return flag;
    }
  }

  return flag;
}

void PqlExtractor::ClearVisitedMap() { curr_visited_.clear(); }