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

  if ((!is_bip && p1 != p2)) {
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
    return affects_table_.GetParentVertices();
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affects_bip_table_.GetParentVertices();
  }

  if (is_bip) {
    return GetAffectsBipTable().GetParentVertices();
  }

  return GetAffectsTable().GetParentVertices();
}

VertexSet AffectsExtractor::GetAllAffectedBy(bool is_bip) {
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.GetParentVertices();
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affected_by_bip_table_.GetParentVertices();
  }

  if (is_bip) {
    return GetAffectedByBipTable().GetParentVertices();
  }

  return GetAffectedByTable().GetParentVertices();
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
  if (IsAffects(stmt_1, stmt_2)) {
    return true;
  }

  ProcName p1 = pkb_->GetProcOfStmt(stmt_1);
  ProcName p2 = pkb_->GetProcOfStmt(stmt_2);
  
  if (p1.empty() || p2.empty()) {
    return false;
  }

  if (!is_bip && p1!=p2) {
    return false;
  }

  if (pkb_->GetStmtType(stmt_1) != StmtType::kAssign ||
    pkb_->GetStmtType(stmt_2) != StmtType::kAssign) {
    return false;
  }

  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.CanReach(stmt_1, stmt_2);
  }

  if (is_bip && has_set_affects_bip_tables_) {
    return affects_bip_table_.CanReach(stmt_1, stmt_2);
  }

  // Do a DFS(start, end) on affects_table. Return boolean value from CanReach. 
  return GetAffectsTable().CanReach(stmt_1, stmt_2);
}

bool AffectsExtractor::IsAffectsT(StmtNum stmt, bool is_bip) {
  return IsAffects(stmt, is_bip);
}

bool AffectsExtractor::IsAffectedT(StmtNum stmt, bool is_bip) {
  return IsAffected(stmt, is_bip);
}

VertexSet AffectsExtractor::GetAffectsT(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  // If none of the preconditions for Affects match, return empty set.
  if (pkb_->GetStmtType(stmt) != StmtType::kAssign || p.empty()) {
    return VertexSet();
  }

  // If only looking for Affects* in local procedure and AffectsTable is already set,
  // perform DFS on AffectsTable and return all the statements that stmt can reach,
  // as a set.
  if (!is_bip && has_set_affects_tables_) {
    VertexList affected_stmts_list = affects_table_.DFS(stmt);
    VertexSet affected_stmts_set(affected_stmts_list.begin(),
      affected_stmts_list.end());
    return affected_stmts_set;
  }

  // If looking for Affects* in entire program and AffectsBipTable is already set,
  // perform DFS on AffectsBipTable and return all the statements that stmt can
  // reach, as a set.
  if (is_bip && has_set_affects_bip_tables_) {
    VertexList affected_bip_stmts_list = affects_bip_table_.DFS(stmt);
    VertexSet affected_bip_stmts_set(affected_bip_stmts_list.begin(),
      affected_bip_stmts_list.end());
    return affected_bip_stmts_set;
  }

  // Otherwise, help set the AffectsTable or AffectsBipTable based on is_bip.
  // Then, do the same DFS as above and return the reachable stmts, as a set.
  if(!is_bip) {
    VertexList affected_stmts_list = GetAffectsTable().DFS(stmt);
    VertexSet affected_stmts_set(affected_stmts_list.begin(),
    affected_stmts_list.end());
    return affected_stmts_set;
  } else {
    VertexList affected_bip_stmts_list = GetAffectsBipTable().DFS(stmt);
    VertexSet affected_bip_stmts_set(affected_bip_stmts_list.begin(),
    affected_bip_stmts_list.end());
    return affected_bip_stmts_set;
  }
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

  // Otherwise, set the AffectedByTable or AffectedByBipTable based on is_bip.
  // Otherwise, do a DFS on the affected_by_(bip)_table_ and convert into set.
  if(!is_bip) {
    VertexList affected_by_stmts_list = GetAffectedByTable().DFS(stmt);
    VertexSet affected_by_stmts_set(affected_by_stmts_list.begin(),
      affected_by_stmts_list.end());
    return affected_by_stmts_set;
  } else {
    VertexList affected_by_bip_stmts_list = GetAffectedByBipTable().DFS(stmt);
    VertexSet affected_by_bip_stmts_set(affected_by_bip_stmts_list.begin(),
    affected_by_bip_stmts_list.end());
    return affected_by_bip_stmts_set;
  }
}

VertexSet AffectsExtractor::GetAllAffectsT(bool is_bip) {
  return GetAllAffects(is_bip);
}

VertexSet AffectsExtractor::GetAllAffectedByT(bool is_bip) {
  return GetAllAffectedBy(is_bip);
}

/***************
* Table Getters *
****************/

AffectsTable AffectsExtractor::GetAffectsTTable() {
  if (has_set_affects_t_tables_) {
    return affects_t_table_;
  }
  SetAffectsTTables();  // populate the tables
  return affects_t_table_;
}

AffectsTable AffectsExtractor::GetAffectedByTTable() {
  if (has_set_affects_t_tables_) {
    return affected_by_t_table_;
  }
  SetAffectsTTables();
  return affected_by_t_table_;
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

AffectsTable AffectsExtractor::GetAffectsBipTTable() {
  if (has_set_affects_bip_t_tables_) {
    return affects_bip_t_table_;
  }
  SetAffectsBipTTables();  // populate the tables
  return affects_bip_table_;
}

AffectsTable AffectsExtractor::GetAffectedByBipTTable() {
  if (has_set_affects_bip_t_tables_) {
    return affected_by_bip_t_table_;
  }
  SetAffectsBipTTables();  // populate the tables
  return affected_by_bip_t_table_;
}

/***************
* Table Setters *
****************/

void AffectsExtractor::SetAffectsTables() {
  ProcNameList all_procs = pkb_->GetAllProcNames();
  for (auto proc_name : all_procs) {
    CFG* cfg = pkb_->GetCFG(proc_name);
    // Special DFS each CFG for affects
    DfsSetAffectsTables(cfg->GetRoot(), &affects_table_, &affected_by_table_,
                        &VisitedMap(), LastModMap(), WhileLastModMap(),
                        WhileLastModMap(), cfg);
  }

  has_set_affects_tables_ = true;
}

void AffectsExtractor::SetAffectsTTables() {
  AffectsTable affects_table = GetAffectsTable();
  AffectsTable affected_by_table = GetAffectedByTable();
  VertexSet vertices = affects_table.GetAllVertices();

  /* For each node in the AffectsTable
  Get all nodes that are reachable from that node (using DFS)
  And add Edge(node, reachable_node) in the new AffectsTTable */
  for (auto& vertex : vertices) {
    VertexList reachable_nodes = affects_table.DFS(vertex);
    for (auto& reachable_node : reachable_nodes) {
      // duplicates will be disregarded by AddEdge
      affects_t_table_.AddEdge(vertex, reachable_node);
    }
  }

  // Do the same for AffectedByT Table (the inverse)
  vertices = affected_by_table.GetAllVertices();
  for (auto& vertex : vertices) {
    VertexList reachable_nodes = affected_by_table.DFS(vertex);
    for (auto& reachable_node : reachable_nodes) {
      // duplicates will be disregarded by AddEdge
      affected_by_t_table_.AddEdge(vertex, reachable_node);
    }
  }

  has_set_affects_t_tables_ = true;
}

void AffectsExtractor::SetAffectsBipTables() {
  CFG* cfg = pkb_->GetProgramCFG();
  VisitedMap visited = VisitedMap();
  // since program cfg must start from 1 and go to n, loop through
  for (int i = 1; i <= cfg->GetSize(); i++) {
    if (!visited.count(i)) {
      DfsSetAffectsTables(i, &affects_bip_table_, &affected_by_bip_table_,
                          &visited, LastModMap(), WhileLastModMap(),
                          WhileLastModMap(), cfg);
    }
  }

  has_set_affects_bip_tables_ = true;
}

void AffectsExtractor::SetAffectsBipTTables() {
  AffectsTable affects_bip_table = GetAffectsBipTable();
  AffectsTable affected_by_bip_table = GetAffectedByBipTable();
  VertexSet vertices = affects_bip_table.GetAllVertices();

  /* For each node in the AffectsBipTable
  Get all nodes that are reachable from that node (using DFS)
  And add Edge(node, reachable_node) in the new AffectsBipTTable */
  for (auto& vertex : vertices) {
    VertexList reachable_nodes = affects_bip_table.DFS(vertex);
    for (auto& reachable_node : reachable_nodes) {
      // duplicates will be disregarded by Graph.AddEdge
      affects_bip_t_table_.AddEdge(vertex, reachable_node);
    }
  }

  vertices = affected_by_bip_table.GetAllVertices();

  // Do the same for AffectedByBipT Table (the inverse)
  for (auto& vertex : vertices) {
    VertexList reachable_nodes = affected_by_bip_table.DFS(vertex);
    for (auto& reachable_node : reachable_nodes) {
      // duplicates will be disregarded by Graph.AddEdge
      affected_by_bip_t_table_.AddEdge(vertex, reachable_node);
    }
  }

  has_set_affects_bip_t_tables_ = true;
}

/***************
* DFS / Util *
****************/

// at -> AffectsTable
// abt -> AffectedByTable
// lmm -> LastModMap
// wlmm -> WhileLastModMap
// pwlmm -> PreviousWhileLastModMap
void AffectsExtractor::DfsSetAffectsTables(Vertex v, AffectsTable* at,
                                           AffectsTable* abt,
                                           VisitedMap* visited, LastModMap lmm,
                                           WhileLastModMap wlmm,
                                           WhileLastModMap pwlmm, CFG* cfg) {
  StmtType stmt_type = pkb_->GetStmtType(v);
  // only return when hit while loop a second time and last_while_mod_map_ is
  // stable
  if (stmt_type == StmtType::kWhile && wlmm.count(v) && lmm == wlmm[v]) {
    return;
  }

  visited->emplace(v, true);

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
    DfsSetAffectsTables(neighbour, at, abt, visited, lmm, wlmm, pwlmm, cfg);
  }
}

bool AffectsExtractor::IsModifyingType(StmtType stmt_type) {
  return stmt_type == StmtType::kCall || stmt_type == StmtType::kRead ||
         stmt_type == StmtType::kAssign;
}
