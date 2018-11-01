
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

    if (visited_stmts.count(curr_stmt)) {
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

    if (visited_stmts.count(curr_stmt)) {
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

    if (visited_stmts.count(curr_stmt)) {
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

bool PqlExtractor::IsAffects(StmtNum stmt_1, StmtNum stmt_2) {
  ProcName p1 = pkb_.GetProcOfStmt(stmt_1);
  ProcName p2 = pkb_.GetProcOfStmt(stmt_2);

  if (p1.empty() || p2.empty() || p1 != p2) {
    return false;
  }

  // Check if both stmts are assignment
  if (pkb_.GetStmtType(stmt_1) != StmtType::kAssign ||
      pkb_.GetStmtType(stmt_2) != StmtType::kAssign) {
    return false;
  }

  VarIndex modified_var = pkb_.GetModifiedVarS(stmt_1).front();

  // Check if variable modified in stmt_1 is used in stmt_2
  if (!pkb_.IsUsedByS(stmt_2, modified_var)) {
    return false;
  }

  curr_affects_cfg_ = pkb_.GetCFG(p1);

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(stmt_1);

  bool flag = false;
  for (Vertex neighbour : neighbours) {
    flag = flag || DfsAffects(neighbour, stmt_2, modified_var);
    if (flag) {
      break;
    }
  }

  ClearAffectsMaps();
  return flag;
}

StmtNumList PqlExtractor::GetAffects(StmtNum stmt_1) {
  ProcName p = pkb_.GetProcOfStmt(stmt_1);

  if (p.empty()) {
    return StmtNumList();
  }

  if (pkb_.GetStmtType(stmt_1) != StmtType::kAssign) {
    return StmtNumList();
  }

  curr_affects_cfg_ = pkb_.GetCFG(p);

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(stmt_1);
  VarIndex affecting_var = pkb_.GetModifiedVarS(stmt_1).front();
  ;

  StmtNumList res_list = StmtNumList();
  for (Vertex neighbour : neighbours) {
    DfsAffects(neighbour, affecting_var, &res_list);
  }

  ClearAffectsMaps();
  return res_list;
}

StmtNumList PqlExtractor::GetAffectedBy(StmtNum stmt_num) {
  ProcName p = pkb_.GetProcOfStmt(stmt_num);

  if (p == ProcName()) {
    return StmtNumList();
  }

  if (pkb_.GetStmtType(stmt_num) != StmtType::kAssign) {
    return StmtNumList();
  }

  curr_affects_cfg_ = pkb_.GetReverseCFG(p);

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(stmt_num);
  VarIndexList var_indices = pkb_.GetUsedVarS(stmt_num);
  VarIndexSet rhs_vars;

  for (VarIndex var_index : var_indices) {
    rhs_vars.emplace(var_index);
  }

  StmtNumList res_list = StmtNumList();
  for (Vertex neighbour : neighbours) {
    DfsAffects(neighbour, rhs_vars, VarIndexSet(), &res_list);
  }

  ClearAffectsMaps();
  return res_list;
}

AffectsTable PqlExtractor::GetAffectsTable() {
  AffectsTable affects_table;

  ProcNameList all_procs = pkb_.GetAllProcNames();
  for (auto proc_name : all_procs) {
    curr_affects_cfg_ = pkb_.GetCFG(proc_name);
    // SpecialDFS each CFG for affects
    DfsAllAffects(curr_affects_cfg_->GetRoot(), &affects_table, LastModMap(),
                  WhileLastModMap());
  }

  return affects_table;
}

// Helper Methods
void PqlExtractor::DfsAllAffects(Vertex v, AffectsTable* affects_table,
                                 LastModMap lmm, WhileLastModMap wlmm) {
  StmtType stmt_type = pkb_.GetStmtType(v);
  // only return when hit while loop a second time and last_while_mod_map_ is
  // stable
  if (stmt_type == StmtType::kWhile && wlmm.count(v) && lmm == wlmm[v]) {
    return;
  }

  if (IsModifyingType(stmt_type)) {
    // assert only 1 modified_var
    VarIndex modified_var = pkb_.GetModifiedVarS(v).front();

    if (stmt_type == StmtType::kAssign) {
      // add used to affects table if found in lmm
      VarIndexList used_vars = pkb_.GetUsedVarS(v);
      for (auto& used_var : used_vars) {
        if (lmm.count(used_var)) {
          StmtNum affecting_stmt = lmm[used_var];
          (*affects_table)[affecting_stmt].emplace(v);
        }
      }

      // add modified to lmm
      lmm.emplace(modified_var, v);
    } else {
      // not assign statement, but modifies something. Need to clear from lmm
      if (lmm.count(modified_var)) {
        lmm.erase(modified_var);
      }
    }
  }

  // update wlmm
  if (stmt_type == StmtType::kWhile) {
    wlmm[v] = lmm;
  }

  // dfs neighbours
  VertexSet neighbours = curr_affects_cfg_->GetNeighboursSet(v);
  for (auto& neighbour : neighbours) {
    DfsAllAffects(neighbour, affects_table, lmm, wlmm);
  }
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

    if (visited_stmts.count(curr_stmt)) {
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

bool PqlExtractor::DfsAffects(Vertex curr, Vertex target,
                              VarIndex affects_var) {
  if (curr_visited_.count(curr)) {
    return false;
  }

  StmtType curr_stmt_type = pkb_.GetStmtType(curr);
  curr_visited_.emplace(curr, true);

  if (curr_stmt_type == StmtType::kAssign && target == curr) {
    return true;
  }

  if (IsModifyingType(curr_stmt_type)) {
    if (pkb_.IsModifiedByS(curr, affects_var)) {
      return false;
    }
  }

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(curr);
  for (Vertex neighbour : neighbours) {
    if (DfsAffects(neighbour, target, affects_var)) {
      return true;
    }
  }

  return false;
}

void PqlExtractor::DfsAffects(Vertex curr, VarIndex affects_var,
                              StmtNumList* res_list) {
  if (curr_visited_.count(curr)) {
    return;
  }

  StmtType curr_stmt_type = pkb_.GetStmtType(curr);
  curr_visited_.emplace(curr, true);

  if (curr_stmt_type == StmtType::kAssign) {
    if (pkb_.IsUsedByS(curr, affects_var)) {
      res_list->push_back(curr);
    }
  }

  if (IsModifyingType(curr_stmt_type)) {
    if (pkb_.IsModifiedByS(curr, affects_var)) {
      return;
    }
  }

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(curr);
  for (Vertex neighbour : neighbours) {
    DfsAffects(neighbour, affects_var, res_list);
  }
}

void PqlExtractor::DfsAffects(Vertex curr, VarIndexSet rhs_vars,
                              VarIndexSet affected_rhs_vars,
                              StmtNumList* res_list) {
  if (curr_visited_.count(curr)) {
    return;
  }

  bool has_affects = false;
  StmtType curr_stmt_type = pkb_.GetStmtType(curr);
  curr_visited_.emplace(curr, true);

  // Check potential affecting statement
  if (curr_stmt_type == StmtType::kAssign) {
    VarIndex curr_modified_var = pkb_.GetModifiedVarS(curr).front();

    // Check if the current assignment statement modifies a variable in the
    // rhs_vars, that has not been modified before
    if (rhs_vars.count(curr_modified_var) &&
        affected_rhs_vars.count(curr_modified_var) == 0) {
      res_list->push_back(curr);
      affected_rhs_vars.emplace(curr_modified_var);
      has_affects = true;
    }
  }

  // Check if all rhs_vars are already affected
  if (rhs_vars.size() == affected_rhs_vars.size()) {
    return;
  }

  // Check for modifying statements
  if (IsModifyingType(curr_stmt_type) && !has_affects) {
    // Check if current statement is affecting any of the rhs_vars
    // Update affected_rhs_vars
    for (VarIndex rhs_var : rhs_vars) {
      if (pkb_.IsModifiedByS(curr, rhs_var)) {
        affected_rhs_vars.emplace(rhs_var);
      }
      if (rhs_vars.size() == affected_rhs_vars.size()) {
        return;
      }
    }
  }

  VertexList neighbours = curr_affects_cfg_->GetNeighboursList(curr);
  for (Vertex neighbour : neighbours) {
    DfsAffects(neighbour, rhs_vars, affected_rhs_vars, res_list);
  }
}

void PqlExtractor::ClearAffectsMaps() { curr_visited_.clear(); }

bool PqlExtractor::IsModifyingType(StmtType stmt_type) {
  return stmt_type == StmtType::kCall || stmt_type == StmtType::kRead ||
         stmt_type == StmtType::kAssign;
}