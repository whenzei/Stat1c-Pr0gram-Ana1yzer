#include <unordered_set>

#include "pql_extractor.h"

using std::unordered_set;

PqlExtractor::PqlExtractor(PKB pkb) { pkb_ = pkb; }

bool PqlExtractor::IsSameProcedure(StmtNum stmt_1, StmtNum stmt_2) {
  return true;
}

bool PqlExtractor::IsAssignStmt(StmtNum stmt) {
  return true;
}

bool PqlExtractor::IsAffectsT(StmtNum stmt_1, StmtNum stmt_2) {
  if (!(IsAssignStmt(stmt_1) && IsAssignStmt(stmt_2))) {
    return false;
  }
  if (!IsSameProcedure(stmt_1, stmt_2)) {
    return false;
  }

  // StmtNumList temp_list;
  // StmtNumList affected_by_s1 = GetAffectedBy(stmt_1);
  // Double loop, check until affected_by_s1 contains stmt_2 and return true if found
  // Keep expanding list every loop

  return false;
}

StmtNumList PqlExtractor::GetAffectsT(StmtNum stmt) {
  //StmtList affected_stmts = GetAffectedBy(stmt);
  StmtNumList final_stmt_list;
  StmtNumList temp_list;

  // final_stmt_list.insert(final_stmt_list.end(), affected_stmts.begin(), affected_stmts.end());

  // Iterate through affected_stmts, do GetAffectedBy of each statements
  // and store all stmts in the final_stmt_list
  return final_stmt_list;
}

StmtNumList PqlExtractor::GetAffectedByT(StmtNum stmt) {
  //StmtList affecting_stmts = GetAffects(stmt);
  StmtNumList final_stmt_list;
  StmtNumList temp_list;

  // final_stmt_list.insert(final_stmt_list.end(), affecting_stmts.begin(), affecting_stmts.end());

  // Iterate through affecting_stmts, do GetAffects of each statements
  // and store all stmts in the final_stmt_list

  return final_stmt_list;
}

StmtNumPairList PqlExtractor::GetAllAffectTPairs() {
  
  return StmtNumPairList();
}

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