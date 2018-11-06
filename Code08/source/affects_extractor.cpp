#include "affects_extractor.h"

AffectsExtractor::AffectsExtractor() {}

AffectsExtractor::AffectsExtractor(PKB* pkb) {
  pkb_ = pkb;
  has_set_affects_tables_ = false;
  has_set_affects_bip_tables_ = false;
  affects_table_ = AffectsTable();
  affects_bip_table_ = AffectsTable();
  affected_by_table_ = AffectsTable();
  affected_by_bip_table_ = AffectsTable();
}

/**** Affects(const, const) methods ****/

bool AffectsExtractor::IsAffects(StmtNum stmt_1, StmtNum stmt_2, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.GetNeighboursSet(stmt_1).count(stmt_2);
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affects_bip_table_.GetNeighboursSet(stmt_1).count(stmt_2);
  }

  // not pre-cached, faster to run dfs rather than retrieve table
  return EvaluateIsAffects(stmt_1, stmt_2, is_bip);
}

bool AffectsExtractor::EvaluateIsAffects(StmtNum stmt_1, StmtNum stmt_2,
                                         bool is_bip) {
  ProcName p1 = pkb_->GetProcOfStmt(stmt_1);
  ProcName p2 = pkb_->GetProcOfStmt(stmt_2);

  // if is_bip, don't have to check if same procedure since its one big cfg
  if (p1.empty() || p2.empty()) {
    return false;
  }

  // if is_bip, check if Call*(p1, p2) is true. If not, return false
  if ((!is_bip && p1 != p2) || (is_bip && !pkb_->IsCallT(p1, p2))) {
    return false;
  }

  // Check if both stmts are assignment
  if (pkb_->GetStmtType(stmt_1) != StmtType::kAssign ||
      pkb_->GetStmtType(stmt_2) != StmtType::kAssign) {
    return false;
  }

  VarIndex modified_var = pkb_->GetModifiedVarS(stmt_1).front();

  // Check if variable modified in stmt_1 is used in stmt_2
  if (!pkb_->IsUsedByS(stmt_2, modified_var)) {
    return false;
  }

  // get cfg depending on whether its an AffectsBip query or not
  CFG* cfg = is_bip ? pkb_->GetProgramCFG() : pkb_->GetCFG(p1);

  VertexList neighbours = cfg->GetNeighboursList(stmt_1);
  VisitedMap visited = VisitedMap();
  for (Vertex& neighbour : neighbours) {
    if (DfsIsAffects(neighbour, stmt_2, modified_var, cfg, &visited)) {
      return true;
    }
  }
  return false;
}

bool AffectsExtractor::DfsIsAffects(Vertex curr, Vertex target,
                                    VarIndex affects_var, CFG* cfg,
                                    VisitedMap* visited) {
  if (visited->count(curr)) {
    return false;
  }

  visited->emplace(curr, true);
  StmtType curr_stmt_type = pkb_->GetStmtType(curr);

  // target is guaranteed to be kAssign type
  if (target == curr) {
    return true;
  }

  // current vertex is modified before it reaches target
  if (IsModifyingType(curr_stmt_type) &&
      pkb_->IsModifiedByS(curr, affects_var)) {
    return false;
  }

  VertexList neighbours = cfg->GetNeighboursList(curr);
  for (Vertex& neighbour : neighbours) {
    if (DfsIsAffects(neighbour, target, affects_var, cfg, visited)) {
      return true;
    }
  }

  return false;
}

/**** Affects(const, _) methods ****/

bool AffectsExtractor::IsAffects(StmtNum stmt, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.GetNeighboursSet(stmt).empty();
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affects_bip_table_.GetNeighboursSet(stmt).empty();
  }

  return EvaluateIsAffects(stmt, is_bip);
}

bool AffectsExtractor::EvaluateIsAffects(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return false;
  }

  CFG* cfg = is_bip ? pkb_->GetProgramCFG() : pkb_->GetCFG(p);

  VertexList neighbours = cfg->GetNeighboursList(stmt);
  VarIndex affecting_var = pkb_->GetModifiedVarS(stmt).front();
  VisitedMap visited = VisitedMap();
  for (Vertex& neighbour : neighbours) {
    if (DfsIsAffects(neighbour, affecting_var, cfg, &visited)) {
      return true;
    }
  }

  return false;
}

bool AffectsExtractor::DfsIsAffects(Vertex curr, VarIndex affects_var, CFG* cfg,
                                    VisitedMap* visited) {
  if (visited->count(curr)) {
    return false;
  }

  visited->emplace(curr, true);
  StmtType curr_stmt_type = pkb_->GetStmtType(curr);

  if (curr_stmt_type == StmtType::kAssign &&
      pkb_->IsUsedByS(curr, affects_var)) {
    return true;
  }

  if (IsModifyingType(curr_stmt_type)) {
    if (pkb_->IsModifiedByS(curr, affects_var)) {
      return false;
    }
  }

  VertexList neighbours = cfg->GetNeighboursList(curr);
  for (Vertex& neighbour : neighbours) {
    if (DfsIsAffects(neighbour, affects_var, cfg, visited)) {
      return true;
    }
  }

  return false;
}

/**** Affects(_, const) methods ****/

bool AffectsExtractor::IsAffected(StmtNum stmt, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.GetNeighboursSet(stmt).empty();
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affected_by_bip_table_.GetNeighboursSet(stmt).empty();
  }

  // not pre-cached, faster to run dfs rather than retrieve table
  return EvaluateIsAffected(stmt, is_bip);
}

bool AffectsExtractor::EvaluateIsAffected(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return false;
  }

  CFG* cfg = is_bip ? pkb_->GetReverseProgramCFG() : pkb_->GetReverseCFG(p);

  VertexList neighbours = cfg->GetNeighboursList(stmt);
  VarIndexList var_indices = pkb_->GetUsedVarS(stmt);
  VarIndexSet rhs_vars = VarIndexSet(var_indices.begin(), var_indices.end());
  VisitedMap visited = VisitedMap();
  for (Vertex& neighbour : neighbours) {
    if (DfsIsAffected(neighbour, rhs_vars, VarIndexSet(), cfg, &visited)) {
      return true;
    }
  }

  return false;
}

bool AffectsExtractor::DfsIsAffected(Vertex curr, VarIndexSet used_vars,
                                     VarIndexSet affected_used_vars, CFG* cfg,
                                     VisitedMap* visited) {
  if (visited->count(curr)) {
    return false;
  }

  visited->emplace(curr, true);
  StmtType curr_stmt_type = pkb_->GetStmtType(curr);

  // Check potential affecting statement
  if (curr_stmt_type == StmtType::kAssign) {
    VarIndex curr_modified_var = pkb_->GetModifiedVarS(curr).front();

    // Check if the current assignment statement modifies a variable in the
    // rhs_vars, that has not been modified before
    if (used_vars.count(curr_modified_var) &&
        affected_used_vars.count(curr_modified_var) == 0) {
      return true;
    }
  }

  // Check for modifying statements
  if (IsModifyingType(curr_stmt_type)) {
    // Check if current statement is affecting any of the rhs_vars
    // Update affected_rhs_vars
    for (auto& used_var : used_vars) {
      if (pkb_->IsModifiedByS(curr, used_var)) {
        affected_used_vars.emplace(used_var);
      }
      if (used_vars.size() == affected_used_vars.size()) {
        return false;
      }
    }
  }

  VertexList neighbours = cfg->GetNeighboursList(curr);
  for (Vertex& neighbour : neighbours) {
    if (DfsIsAffected(neighbour, used_vars, affected_used_vars, cfg, visited)) {
      return true;
    }
  }

  return false;
}

/**** Affects(const, syn) methods ****/

VertexSet AffectsExtractor::GetAffects(StmtNum stmt, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.GetNeighboursSet(stmt);
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affects_bip_table_.GetNeighboursSet(stmt);
  }

  // not pre-cached, faster to run dfs rather than retrieve table
  return EvaluateGetAffects(stmt, is_bip);
}

VertexSet AffectsExtractor::EvaluateGetAffects(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return VertexSet();
  }

  CFG* cfg = is_bip ? pkb_->GetProgramCFG() : pkb_->GetCFG(p);

  VertexList neighbours = cfg->GetNeighboursList(stmt);
  VarIndex affecting_var = pkb_->GetModifiedVarS(stmt).front();
  VisitedMap visited = VisitedMap();
  VertexSet res_list = VertexSet();
  for (Vertex neighbour : neighbours) {
    DfsGetAffects(neighbour, affecting_var, &res_list, cfg, &visited);
  }

  return res_list;
}

void AffectsExtractor::DfsGetAffects(Vertex curr, VarIndex affects_var,
                                     VertexSet* res_list, CFG* cfg,
                                     VisitedMap* visited) {
  if (visited->count(curr)) {
    return;
  }

  StmtType curr_stmt_type = pkb_->GetStmtType(curr);
  visited->emplace(curr, true);

  if (curr_stmt_type == StmtType::kAssign) {
    if (pkb_->IsUsedByS(curr, affects_var)) {
      res_list->emplace(curr);
    }
  }

  if (IsModifyingType(curr_stmt_type)) {
    if (pkb_->IsModifiedByS(curr, affects_var)) {
      return;
    }
  }

  VertexList neighbours = cfg->GetNeighboursList(curr);
  for (Vertex neighbour : neighbours) {
    DfsGetAffects(neighbour, affects_var, res_list, cfg, visited);
  }
}

/**** Affects(syn, const) methods ****/

VertexSet AffectsExtractor::GetAffectedBy(StmtNum stmt, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.GetNeighboursSet(stmt);
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affected_by_bip_table_.GetNeighboursSet(stmt);
  }

  // not pre-cached, faster to run dfs rather than retrieve table
  return EvaluateGetAffectedBy(stmt, is_bip);
}

VertexSet AffectsExtractor::EvaluateGetAffectedBy(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return VertexSet();
  }

  CFG* cfg = is_bip ? pkb_->GetReverseProgramCFG() : pkb_->GetReverseCFG(p);

  VertexList neighbours = cfg->GetNeighboursList(stmt);
  VarIndexList var_indices = pkb_->GetUsedVarS(stmt);
  VarIndexSet used_vars = VarIndexSet(var_indices.begin(), var_indices.end());
  VisitedMap visited = VisitedMap();
  VertexSet res_list = VertexSet();
  for (Vertex& neighbour : neighbours) {
    DfsGetAffectedBy(neighbour, used_vars, VarIndexSet(), &res_list, cfg,
                     &visited);
  }

  return res_list;
}

void AffectsExtractor::DfsGetAffectedBy(Vertex curr, VarIndexSet used_vars,
                                        VarIndexSet affected_used_vars,
                                        VertexSet* res_list, CFG* cfg,
                                        VisitedMap* visited) {
  if (visited->count(curr)) {
    return;
  }

  visited->emplace(curr, true);

  bool has_affects = false;
  StmtType curr_stmt_type = pkb_->GetStmtType(curr);

  // Check potential affecting statement
  if (curr_stmt_type == StmtType::kAssign) {
    VarIndex curr_modified_var = pkb_->GetModifiedVarS(curr).front();

    // Check if the current assignment statement modifies a variable in the
    // used_vars, that has not been modified before
    if (used_vars.count(curr_modified_var) &&
        affected_used_vars.count(curr_modified_var) == 0) {
      res_list->emplace(curr);
      affected_used_vars.emplace(curr_modified_var);
      has_affects = true;
    }
  }

  // Check if all rhs_vars are already affected
  if (used_vars.size() == affected_used_vars.size()) {
    return;
  }

  // Check for modifying statements
  if (IsModifyingType(curr_stmt_type) && !has_affects) {
    // Check if current statement is affecting any of the rhs_vars
    // Update affected_rhs_vars
    for (auto& used_var : used_vars) {
      if (pkb_->IsModifiedByS(curr, used_var)) {
        affected_used_vars.emplace(used_var);
      }
      if (used_vars.size() == affected_used_vars.size()) {
        return;
      }
    }
  }

  VertexList neighbours = cfg->GetNeighboursList(curr);
  for (Vertex& neighbour : neighbours) {
    DfsGetAffectedBy(neighbour, used_vars, affected_used_vars, res_list, cfg,
                     visited);
  }
}

VertexSet AffectsExtractor::GetAllAffects(bool is_bip) {
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.GetAllVertices();
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affects_bip_table_.GetAllVertices();
  }

  if (is_bip) {
    return GetAffectsBipTable().GetAllVertices();
  }

  return GetAffectsTable().GetAllVertices();
}

VertexSet AffectsExtractor::GetAllAffectedBy(bool is_bip) {
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.GetAllVertices();
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affected_by_bip_table_.GetAllVertices();
  }

  if (is_bip) {
    return GetAffectedByBipTable().GetAllVertices();
  }

  return GetAffectedByTable().GetAllVertices();
}

AffectsTable AffectsExtractor::GetAffectsTable() {
  if (has_set_affects_tables_) {
    return affects_table_;
  }
  SetAffectsTables();  // populate the tables
  return affects_table_;
}

AffectsTable AffectsExtractor::GetAffectedByTable() {
  if (has_set_affects_tables_) {
    return affected_by_table_;
  }
  SetAffectsTables();  // populate the tables
  return affected_by_table_;
}

/**** AffectsT methods ****/

bool AffectsExtractor::IsAffectsT(StmtNum stmt_1, StmtNum stmt_2, bool is_bip) {
  ProcName p1 = pkb_->GetProcOfStmt(stmt_1);
  ProcName p2 = pkb_->GetProcOfStmt(stmt_2);
  
  if (p1.empty() || p2.empty()) {
    return false;
  }

  if ((is_bip && pkb_->IsCallT(p1, p2)) || (!is_bip && p1!=p2)) {
    return false;
  }

  if (pkb_->GetStmtType(stmt_1) != StmtType::kAssign ||
    pkb_->GetStmtType(stmt_2) != StmtType::kAssign) {
    return false;
  }

  if (!is_bip && has_set_affects_tables_) {
    // return affects_table_.DFS(stmt_1, stmt_2);
  }

  if (is_bip && has_set_affects_bip_tables_) {
    // return affects_bip_table_.DFS(stmt_1, stmt_2);
  }
  // Do a DFS(start, end) on affects_table. Return boolean value from CanReach. 
  /// return GetAffectsTable().CanReach(stmt_1, stmt_2);
  // Dummy
  return false;
}

bool AffectsExtractor::IsAffectsT(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (pkb_->GetStmtType(stmt) != StmtType::kAssign || p.empty()) {
    return false;
  }
  
  if (!is_bip && has_set_affects_tables_) {
    return !affects_table_.GetNeighboursSet(stmt).empty();
  }
  
  if (is_bip && has_set_affects_bip_tables_) {
    return !affects_bip_table_.GetNeighboursSet(stmt).empty();
  }

  // Otherwise, do a DFS on the affects_table_ if !is_bip
  // Do DFS on affects_bip_table_ if is_bip
  // return is_bip ? !GetAffectsBipTable().DFS(stmt).empty() 
  // : !GetAffectsTable().DFS(stmt).empty();
  // return !GetAffectsTable().DFS(stmt).empty();

  // Dummy
  return false;
}

bool AffectsExtractor::IsAffectedByT(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (pkb_->GetStmtType(stmt) != StmtType::kAssign || p.empty()) {
    return false;
  }

  if (!is_bip && has_set_affects_tables_) {
    return !affected_by_table_.GetNeighboursSet(stmt).empty();
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return !affected_by_bip_table_.GetNeighboursSet(stmt).empty();
  }

  // Otherwise, do a DFS on the affected_by_table_ if !is_bip
  // Do DFS on affected_by_bip_table_ if is_bip
  // return is_bip ? !GetAffectedByBipTable().DFS(stmt).empty() :
  // !GetAffectedByTable().DFS(stmt).empty();

  // Dummy
  return false;
}

VertexSet AffectsExtractor::GetAffectsT(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  // If none of the preconditions for Affects match, return empty set.
  if (pkb_->GetStmtType(stmt) != StmtType::kAssign || p.empty()) {
    return VertexSet();
  }

  // If only for local procedure and AffectsTable is already set
  // Perform DFS on AffectsTable and return all the statements that stmt can reach
  // as a set.
  if (!is_bip && has_set_affects_tables_) {
    VertexList affected_stmts_list = affects_table_.DFS(stmt);
    VertexSet affected_stmts_set(affected_stmts_list.begin(),
      affected_stmts_list.end());
    return affected_stmts_set;
  }

  // If for entire program and AffectsBipTable is already set
  // Perform DFS on AffectsBipTable and return all the statements that stmt can
  // reach as a set.
  if (is_bip && has_set_affects_bip_tables_) {
    VertexList affected_bip_stmts_list = affects_bip_table_.DFS(stmt);
    VertexSet affected_bip_stmts_set(affected_bip_stmts_list.begin(),
      affected_bip_stmts_list.end());
    return affected_bip_stmts_set;
  }

  // Otherwise, do a DFS on the affects_table_ and convert into set.
  // Help set the AffectsTable or AffectsBipTable based on is_bip.
  // TODO: Uncomment. Might be better to separate this part out.

  // if(is_bip) {
  // GetAffectsTable();
  // VertexList affected_stmts_list = affects_table_.DFS(stmt);
  // VertexSet affected_stmts_set(affected_stmts_list.begin(),
  // affected_stmts_list.end());
  // return affected_stmts_set;
  // } else {
  // GetAffectsBipTable();
  // VertexList affected_bip_stmts_list = affects_bip_table_.DFS(stmt);
  // VertexSet affected_bip_stmts_set(affected_bip_stmts_list.begin(),
  // affected_bip_stmts_list.end());
  // return affected_bip_stmts_set;
  // }

  // Dummy
  // return VertexSet();
}

VertexSet AffectsExtractor::GetAffectedByT(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  // If none of the preconditions for Affects match, return empty set.
  if (pkb_->GetStmtType(stmt) != StmtType::kAssign || p.empty()) {
    return VertexSet();
  }

  // If only for local procedure and AffectedByTable is already set
  // Perform DFS on AffectedByTable and return all the statements that stmt can reach
  // as a set.
  if (!is_bip && has_set_affects_tables_) {
    VertexList affected_by_stmts_list = affected_by_table_.DFS(stmt);
    VertexSet affected_by_stmts_set(affected_by_stmts_list.begin(),
      affected_by_stmts_list.end());
    return affected_by_stmts_set;
  }

  // If for entire program and AffectsBipTable is already set
  // Perform DFS on AffectsBipTable and return all the statements that stmt can
  // reach as a set.
  if (is_bip && has_set_affects_bip_tables_) {
    VertexList affected_by_bip_stmts_list = affected_by_bip_table_.DFS(stmt);
    VertexSet affected_by_bip_stmts_set(affected_by_bip_stmts_list.begin(),
      affected_by_bip_stmts_list.end());
    return affected_by_bip_stmts_set;
  }

  // Otherwise, do a DFS on the affected_by_table_ and convert into set.
  // Help set the AffectedByTable or AffectedByBipTable based on is_bip.
  // TODO: Uncomment. Might be better to separate this part out.

  // if(is_bip) {
  // GetAffectsTable(); 
  // VertexList affected_by_stmts_list = affected_by_table_.DFS(stmt);
  // VertexSet affected_by_stmts_set(affected_by_stmts_list.begin(),
  // affected_by_stmts_list.end());
  // return affected_by_stmts_set;
  // } else {
  // GetAffectsBipTable();
  // VertexList affected_by_bip_stmts_list = affected_by_bip_table_.DFS(stmt);
  // VertexSet affected_by_bip_stmts_set(affected_by_bip_stmts_list.begin(),
  // affected_by_bip_stmts_list.end());
  // return affected_by_bip_stmts_set;
  // }

  // Dummy
  // return VertexSet();
}

VertexSet AffectsExtractor::GetAllAffectsT(bool is_bip) {
  return GetAllAffects(is_bip);
}

VertexSet AffectsExtractor::GetAllAffectedByT(bool is_bip) {
  return GetAllAffectedBy(is_bip);
}

AffectsTable AffectsExtractor::GetAffectsBipTable() {
  if (has_set_affects_bip_tables_) {
    return affects_bip_table_;
  }
  SetAffectsBipTables();  // populate the tables
  return affects_bip_table_;
}

AffectsTable AffectsExtractor::GetAffectedByBipTable() {
  if (has_set_affects_bip_tables_) {
    return affected_by_bip_table_;
  }
  SetAffectsBipTables();  // populate the tables
  return affected_by_bip_table_;
}

void AffectsExtractor::SetAffectsTables() {
  ProcNameList all_procs = pkb_->GetAllProcNames();
  for (auto proc_name : all_procs) {
    CFG* cfg = pkb_->GetCFG(proc_name);
    // Special DFS each CFG for affects
    DfsSetAffectsTables(cfg->GetRoot(), &affects_table_, &affected_by_table_,
                        LastModMap(), WhileLastModMap(), WhileLastModMap(),
                        cfg);
  }

  has_set_affects_tables_ = true;
}

void AffectsExtractor::SetAffectsBipTables() {
  CFG* cfg = pkb_->GetProgramCFG();
  DfsSetAffectsTables(cfg->GetRoot(), &affects_bip_table_,
                      &affected_by_bip_table_, LastModMap(), WhileLastModMap(),
                      WhileLastModMap(), cfg);

  has_set_affects_bip_tables_ = true;
}

// at -> AffectsTable
// abt -> AffectedByTable
// lmm -> LastModMap
// wlmm -> WhileLastModMap
// pwlmm -> PreviousWhileLastModMap
void AffectsExtractor::DfsSetAffectsTables(Vertex v, AffectsTable* at,
                                           AffectsTable* abt, LastModMap lmm,
                                           WhileLastModMap wlmm,
                                           WhileLastModMap pwlmm, CFG* cfg) {
  StmtType stmt_type = pkb_->GetStmtType(v);
  // only return when hit while loop a second time and last_while_mod_map_ is
  // stable
  if (stmt_type == StmtType::kWhile && wlmm.count(v) && lmm == wlmm[v]) {
    return;
  }

  if (IsModifyingType(stmt_type)) {
    // assert only 1 modified_var
    VarIndex modified_var = pkb_->GetModifiedVarS(v).front();

    if (stmt_type == StmtType::kAssign) {
      // add used to affects table if found in lmm
      VarIndexList used_vars = pkb_->GetUsedVarS(v);
      for (auto& used_var : used_vars) {
        if (lmm.count(used_var)) {
          StmtNum affecting_stmt = lmm[used_var];
          at->AddEdge(affecting_stmt, v);
          abt->AddEdge(v, affecting_stmt);
        }
      }

      // add modified to lmm
      lmm[modified_var] = v;
    } else {
      // not assign statement, but modifies something. Need to clear from lmm
      if (lmm.count(modified_var)) {
        lmm.erase(modified_var);
      }
    }
  }

  // update wlmm
  if (stmt_type == StmtType::kWhile) {
    LastModMap temp = wlmm[v];
    wlmm[v] = lmm;
    if (wlmm.count(v) && pwlmm.count(v) && pwlmm[v] == wlmm[v]) {
      return;
    }
    pwlmm[v] = temp;
  }

  // dfs neighbours
  VertexSet neighbours = cfg->GetNeighboursSet(v);
  for (auto& neighbour : neighbours) {
    DfsSetAffectsTables(neighbour, at, abt, lmm, wlmm, pwlmm, cfg);
  }
}

bool AffectsExtractor::IsModifyingType(StmtType stmt_type) {
  return stmt_type == StmtType::kCall || stmt_type == StmtType::kRead ||
         stmt_type == StmtType::kAssign;
}