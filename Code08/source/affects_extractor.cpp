
#include "affects_extractor.h"

AffectsExtractor::AffectsExtractor() {}

AffectsExtractor::AffectsExtractor(PKB* pkb) {
  pkb_ = pkb;
  has_set_affects_tables_ = false;
  has_set_affects_t_tables_ = false;
  has_set_affects_bip_tables_ = false;
  has_set_affects_bip_t_tables_ = false;
  has_checked_affects_relationship_ = false;

  has_affects_relationship_ = false;

  affects_table_ = AffectsTable();
  affects_t_table_ = AffectsTable();
  affects_bip_table_ = AffectsTable();
  affects_bip_t_table_ = AffectsTable();
  affected_by_table_ = AffectsTable();
  affected_by_t_table_ = AffectsTable();
  affected_by_bip_table_ = AffectsTable();
  affected_by_bip_t_table_ = AffectsTable();
}

/**** Affects(const, const) methods ****/

bool AffectsExtractor::IsAffects(StmtNum stmt_1, StmtNum stmt_2, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.GetNeighboursSet(stmt_1).count(stmt_2);
  }

  // need table regardless for bips since the programcfg has multiple entry and
  // exit points.
  // Not pre-cached, Affects faster to run dfs rather than retrieve table for
  // Affects
  return is_bip ? EvaluateIsAffectsBip(stmt_1, stmt_2)
                : EvaluateIsAffects(stmt_1, stmt_2);
}

bool AffectsExtractor::EvaluateIsAffects(StmtNum stmt_1, StmtNum stmt_2) {
  ProcName p1 = pkb_->GetProcOfStmt(stmt_1);
  ProcName p2 = pkb_->GetProcOfStmt(stmt_2);

  if (p1.empty() || p2.empty() || p1 != p2) {
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

  CFG* cfg = pkb_->GetCFG(p1);

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

bool AffectsExtractor::EvaluateIsAffectsBip(StmtNum stmt_1, StmtNum stmt_2) {
  ProcName p1 = pkb_->GetProcOfStmt(stmt_1);
  ProcName p2 = pkb_->GetProcOfStmt(stmt_2);

  if (p1.empty() || p2.empty()) {
    return false;
  }

  // Check if both stmts are assignment
  if (pkb_->GetStmtType(stmt_1) != StmtType::kAssign ||
      pkb_->GetStmtType(stmt_2) != StmtType::kAssign) {
    return false;
  }

  return GetAffectsBipTable().GetNeighboursSet(stmt_1).count(stmt_2);
}

/**** Affects(_, _) methods ****/

bool AffectsExtractor::HasAffectsRelationship() {
  // already pre-cached
  if (has_set_affects_tables_) {
    return !affects_table_.IsEmpty();
  }

  if (has_checked_affects_relationship_) {
    return has_affects_relationship_;
  }

  has_checked_affects_relationship_ = true;

  // else check if any assign stmts affects with early termination
  StmtNumList assign_stmts = pkb_->GetAllAssignStmt();
  for (auto& assign_stmt : assign_stmts) {
    has_affects_relationship_ =
        has_affects_relationship_ || IsAffects(assign_stmt);
    if (has_affects_relationship_) {
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

  return is_bip ? EvaluateIsAffectsBip(stmt) : EvaluateIsAffects(stmt);
}

bool AffectsExtractor::EvaluateIsAffects(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return false;
  }

  CFG* cfg = pkb_->GetCFG(p);

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

bool AffectsExtractor::EvaluateIsAffectsBip(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return false;
  }

  return !GetAffectsBipTable().GetNeighboursSet(stmt).empty();
}

/**** Affects(_, const) methods ****/

bool AffectsExtractor::IsAffected(StmtNum stmt, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.GetNeighboursSet(stmt).empty();
  }

  // not pre-cached, faster to run dfs rather than retrieve table for Affects
  return is_bip ? EvaluateIsAffectedBip(stmt) : EvaluateIsAffected(stmt);
}

bool AffectsExtractor::EvaluateIsAffected(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return false;
  }

  CFG* cfg = pkb_->GetReverseCFG(p);

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

bool AffectsExtractor::EvaluateIsAffectedBip(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return false;
  }

  return !GetAffectedByBipTable().GetNeighboursSet(stmt).empty();
}

/**** Affects(const, syn) methods ****/

VertexSet AffectsExtractor::GetAffects(StmtNum stmt, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.GetNeighboursSet(stmt);
  }

  // not pre-cached, faster to run dfs rather than retrieve table for bips
  return is_bip ? EvaluateGetAffectsBip(stmt) : EvaluateGetAffects(stmt);
}

VertexSet AffectsExtractor::EvaluateGetAffects(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return VertexSet();
  }

  CFG* cfg = pkb_->GetCFG(p);

  VertexList neighbours = cfg->GetNeighboursList(stmt);
  VarIndex affecting_var = pkb_->GetModifiedVarS(stmt).front();
  VisitedMap visited = VisitedMap();
  VertexSet res_list = VertexSet();
  for (Vertex neighbour : neighbours) {
    DfsGetAffects(neighbour, affecting_var, &res_list, cfg, visited);
  }

  return res_list;
}

void AffectsExtractor::DfsGetAffects(Vertex curr, VarIndex affects_var,
                                     VertexSet* res_list, CFG* cfg,
                                     VisitedMap visited) {
  if (visited.count(curr)) {
    return;
  }

  StmtType curr_stmt_type = pkb_->GetStmtType(curr);
  visited.emplace(curr, true);

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

VertexSet AffectsExtractor::EvaluateGetAffectsBip(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return VertexSet();
  }

  return GetAffectsBipTable().GetNeighboursSet(stmt);
}

/**** Affects(syn, const) methods ****/

VertexSet AffectsExtractor::GetAffectedBy(StmtNum stmt, bool is_bip) {
  // already pre-cached
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.GetNeighboursSet(stmt);
  }

  // not pre-cached, faster to run dfs rather than retrieve table for Affects
  return is_bip ? EvaluateGetAffectedByBip(stmt) : EvaluateGetAffectedBy(stmt);
}

VertexSet AffectsExtractor::EvaluateGetAffectedBy(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return VertexSet();
  }

  CFG* cfg = pkb_->GetReverseCFG(p);

  VertexList neighbours = cfg->GetNeighboursList(stmt);
  VarIndexList var_indices = pkb_->GetUsedVarS(stmt);
  VarIndexSet used_vars = VarIndexSet(var_indices.begin(), var_indices.end());
  VisitedMap visited = VisitedMap();
  VertexSet res_list = VertexSet();
  for (Vertex& neighbour : neighbours) {
    DfsGetAffectedBy(neighbour, used_vars, VarIndexSet(), &res_list, cfg,
                     visited);
  }

  return res_list;
}

void AffectsExtractor::DfsGetAffectedBy(Vertex curr, VarIndexSet used_vars,
                                        VarIndexSet affected_used_vars,
                                        VertexSet* res_list, CFG* cfg,
                                        VisitedMap visited) {
  if (visited.count(curr)) {
    return;
  }

  visited.emplace(curr, true);

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

VertexSet AffectsExtractor::EvaluateGetAffectedByBip(StmtNum stmt) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  if (p.empty() || pkb_->GetStmtType(stmt) != StmtType::kAssign) {
    return VertexSet();
  }

  return GetAffectedByBipTable().GetNeighboursSet(stmt);
}

/* Obtaining left or right hand side of the Affects tables*/

VertexSet AffectsExtractor::GetAllAffects(bool is_bip) {
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.GetParentVertices();
  }

  return is_bip ? GetAffectsBipTable().GetParentVertices()
                : GetAffectsTable().GetParentVertices();
}

VertexSet AffectsExtractor::GetAllAffectedBy(bool is_bip) {
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.GetParentVertices();
  }

  return is_bip ? GetAffectedByBipTable().GetParentVertices()
                : GetAffectedByTable().GetParentVertices();
}

AffectsMap AffectsExtractor::GetAffectsMap() {
  if (!has_set_affects_tables_) {
    SetAffectsTables();  // populate the tables if not set yet
  }
  return affects_table_.GetAdjSet();
}

AffectsMap AffectsExtractor::GetAffectedByMap() {
  if (!has_set_affects_tables_) {
    SetAffectsTables();  // populate the tables
  }
  return affected_by_table_.GetAdjSet();
}

AffectsTable AffectsExtractor::GetAffectsTable() {
  if (!has_set_affects_tables_) {
    SetAffectsTables();  // populate the tables if not set yet
  }
  return affects_table_;
}

AffectsTable AffectsExtractor::GetAffectedByTable() {
  if (!has_set_affects_tables_) {
    SetAffectsTables();  // populate the tables if not set yet
  }
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

  if (!is_bip && p1 != p2) {
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

  // Do a DFS(start, end) on affects_table. Return boolean value from
  // CanReach.
  return is_bip ? GetAffectsBipTable().CanReach(stmt_1, stmt_2)
                : GetAffectsTable().CanReach(stmt_1, stmt_2);
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

  // If only looking for Affects* in local procedure and AffectsTable is
  // already set, perform DFS on AffectsTable and return all the statements
  // that stmt can reach.
  if (!is_bip && has_set_affects_tables_) {
    return affects_table_.DFSNeighbours(stmt);
  }

  // If looking for Affects* in entire program and AffectsBipTable is already
  // set, perform DFS on AffectsBipTable and return all the statements that
  // stmt can reach.
  if (is_bip && has_set_affects_bip_tables_) {
    return affects_bip_table_.DFSNeighbours(stmt);
  }

  // Otherwise, help set the AffectsTable or AffectsBipTable based on is_bip.
  // Then, do the same DFS as above and return the reachable stmts.
  if (!is_bip) {
    return GetAffectsTable().DFSNeighbours(stmt);
  } else {
    return GetAffectsBipTable().DFSNeighbours(stmt);
  }
}

VertexSet AffectsExtractor::GetAffectedByT(StmtNum stmt, bool is_bip) {
  ProcName p = pkb_->GetProcOfStmt(stmt);

  // If none of the preconditions for Affects match, return empty set.
  if (pkb_->GetStmtType(stmt) != StmtType::kAssign || p.empty()) {
    return VertexSet();
  }

  // If only for local procedure and AffectedByTable is already set
  // Perform DFS on AffectedByTable and return all the statements that stmt
  // can reach.
  if (!is_bip && has_set_affects_tables_) {
    return affected_by_table_.DFSNeighbours(stmt);
  }

  // If for entire program and AffectsBipTable is already set
  // Perform DFS on AffectsBipTable and return all the statements that stmt
  // can reach.
  if (is_bip && has_set_affects_bip_tables_) {
    return affected_by_bip_table_.DFSNeighbours(stmt);
  }

  // Otherwise, set the AffectedByTable or AffectedByBipTable based on is_bip
  // perform DFSNeighbours on the respective table.
  if (!is_bip) {
    return GetAffectedByTable().DFSNeighbours(stmt);
  } else {
    return GetAffectedByBipTable().DFSNeighbours(stmt);
  }
}

VertexSet AffectsExtractor::GetAllAffectsT(bool is_bip) {
  return GetAllAffects(is_bip);
}

VertexSet AffectsExtractor::GetAllAffectedByT(bool is_bip) {
  return GetAllAffectedBy(is_bip);
}

/***********************
 * Map Getters for PQL *
 ***********************/

AffectsMap AffectsExtractor::GetAffectsTMap() {
  if (!has_set_affects_t_tables_) {
    SetAffectsTTables();  // populate the tables
  }
  return affects_t_table_.GetAdjSet();
}

AffectsMap AffectsExtractor::GetAffectedByTMap() {
  if (!has_set_affects_t_tables_) {
    SetAffectsTTables();
  }
  return affected_by_t_table_.GetAdjSet();
}

AffectsMap AffectsExtractor::GetAffectsBipMap() {
  if (!has_set_affects_bip_tables_) {
    SetAffectsBipTables();  // populate the tables
  }
  return affects_bip_table_.GetAdjSet();
}

AffectsMap AffectsExtractor::GetAffectedByBipMap() {
  if (!has_set_affects_bip_tables_) {
    SetAffectsBipTables();  // populate the tables
  }
  return affected_by_bip_table_.GetAdjSet();
}

AffectsMap AffectsExtractor::GetAffectsBipTMap() {
  if (!has_set_affects_bip_t_tables_) {
    SetAffectsBipTTables();  // populate the tables
  }
  return affects_bip_t_table_.GetAdjSet();
}

AffectsMap AffectsExtractor::GetAffectedByBipTMap() {
  if (!has_set_affects_bip_t_tables_) {
    SetAffectsBipTTables();  // populate the tables
  }
  return affected_by_bip_t_table_.GetAdjSet();
}

/***************
 * Table Getters *
 ****************/

AffectsTable AffectsExtractor::GetAffectsTTable() {
  if (!has_set_affects_t_tables_) {
    SetAffectsTTables();  // populate the tables
  }
  return affects_t_table_;
}

AffectsTable AffectsExtractor::GetAffectedByTTable() {
  if (!has_set_affects_t_tables_) {
    SetAffectsTTables();
  }
  return affected_by_t_table_;
}

AffectsTable AffectsExtractor::GetAffectsBipTable() {
  if (!has_set_affects_bip_tables_) {
    SetAffectsBipTables();  // populate the tables
  }
  return affects_bip_table_;
}

AffectsTable AffectsExtractor::GetAffectedByBipTable() {
  if (!has_set_affects_bip_tables_) {
    SetAffectsBipTables();  // populate the tables
  }
  return affected_by_bip_table_;
}

AffectsTable AffectsExtractor::GetAffectsBipTTable() {
  if (!has_set_affects_bip_t_tables_) {
    SetAffectsBipTTables();  // populate the tables
  }
  return affects_bip_t_table_;
}

AffectsTable AffectsExtractor::GetAffectedByBipTTable() {
  if (!has_set_affects_bip_t_tables_) {
    SetAffectsBipTTables();  // populate the tables
  }
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
                        &VisitedMap(), LastModMap(), VisitedCountMap(), cfg,
                        false);
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
    VertexSet reachable_nodes = affects_table.DFSNeighbours(vertex);
    for (auto& reachable_node : reachable_nodes) {
      // duplicates will be disregarded by AddEdge
      affects_t_table_.AddEdge(vertex, reachable_node);
    }
  }

  // Do the same for AffectedByT Table (the inverse)
  vertices = affected_by_table.GetAllVertices();
  for (auto& vertex : vertices) {
    VertexSet reachable_nodes = affected_by_table.DFSNeighbours(vertex);
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
      DfsSetAffectsBipTables(i, &affects_bip_table_, &affected_by_bip_table_,
                             &visited, LastModMap(), VisitedCountMap(), cfg,
                             stack<Vertex>());
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
    VertexSet reachable_nodes = affects_bip_table.DFSNeighbours(vertex);
    for (auto& reachable_node : reachable_nodes) {
      // duplicates will be disregarded by Graph.AddEdge
      affects_bip_t_table_.AddEdge(vertex, reachable_node);
    }
  }

  vertices = affected_by_bip_table.GetAllVertices();

  // Do the same for AffectedByBipT Table (the inverse)
  for (auto& vertex : vertices) {
    VertexSet reachable_nodes = affected_by_bip_table.DFSNeighbours(vertex);
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

void AffectsExtractor::DfsSetAffectsBipTables(Vertex v, AffectsTable* at,
                                              AffectsTable* abt,
                                              VisitedMap* visited,
                                              LastModMap lmm,
                                              VisitedCountMap vcm, CFG* cfg,
                                              stack<Vertex> incoming_edges) {
  // return if this is the third time reaching this vertex
  StmtType stmt_type = pkb_->GetStmtType(v);
  if (vcm.count(v)) {
    if (vcm[v] < 2) {
      vcm[v] += 1;
    } else {
      if (!incoming_edges.empty()) {
        StmtNumList nexts = pkb_->GetNext(incoming_edges.top());
        incoming_edges.pop();
        for (auto& stmt : nexts) {
          DfsSetAffectsBipTables(stmt, at, abt, visited, lmm, vcm, cfg,
                                 incoming_edges);
        }
      } else {
        return;
      }
    }
  } else {
    vcm.emplace(v, 1);
  }

  visited->emplace(v, true);

  if (IsModifyingType(stmt_type, true)) {
    // assert only 1 modified_var
    VarIndexList modified_vars = pkb_->GetModifiedVarS(v);

    if (stmt_type == StmtType::kAssign) {
      VarIndex modified_var = modified_vars.front();
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
      for (auto& modified_var : modified_vars) {
        if (lmm.count(modified_var)) {
          lmm.erase(modified_var);
        }
      }
    }
  }

  if (stmt_type == StmtType::kCall) {
    incoming_edges.push(v);
    Vertex called_proc_root =
        pkb_->GetCFG(pkb_->GetProcName(pkb_->GetCalledProcedure(v)))->GetRoot();
    DfsSetAffectsBipTables(called_proc_root, at, abt, visited, lmm, vcm, cfg,
                           incoming_edges);
  } else {
    // this vertex is a terminal vertex and has a incoming edge
    if (pkb_->GetNext(v).empty()) {
      if (!incoming_edges.empty()) {
        StmtNumList nexts = pkb_->GetNext(incoming_edges.top());
        incoming_edges.pop();
        for (auto& stmt : nexts) {
          DfsSetAffectsBipTables(stmt, at, abt, visited, lmm, vcm, cfg,
                                 incoming_edges);
        }
      } else {
        return;
      }
    } else {
      // dfs neighbours
      VertexSet neighbours = cfg->GetNeighboursSet(v);
      for (auto& neighbour : neighbours) {
        DfsSetAffectsBipTables(neighbour, at, abt, visited, lmm, vcm, cfg,
                               incoming_edges);
      }
    }
  }
}

// at -> AffectsTable
// abt -> AffectedByTable
// lmm -> LastModMap
// vcm -> VisitedCountMap
void AffectsExtractor::DfsSetAffectsTables(Vertex v, AffectsTable* at,
                                           AffectsTable* abt,
                                           VisitedMap* visited, LastModMap lmm,
                                           VisitedCountMap vcm, CFG* cfg,
                                           bool is_bip) {
  // return if this is the third time reaching this vertex
  if (vcm.count(v)) {
    if (vcm[v] < 2) {
      vcm[v] += 1;
    } else {
      return;
    }
  } else {
    vcm.emplace(v, 1);
  }

  visited->emplace(v, true);

  StmtType stmt_type = pkb_->GetStmtType(v);

  if (IsModifyingType(stmt_type, is_bip)) {
    // assert only 1 modified_var
    VarIndexList modified_vars = pkb_->GetModifiedVarS(v);

    if (stmt_type == StmtType::kAssign) {
      VarIndex modified_var = modified_vars.front();
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
      for (auto& modified_var : modified_vars) {
        if (lmm.count(modified_var)) {
          lmm.erase(modified_var);
        }
      }
    }
  }

  // dfs neighbours
  VertexSet neighbours = cfg->GetNeighboursSet(v);
  for (auto& neighbour : neighbours) {
    DfsSetAffectsTables(neighbour, at, abt, visited, lmm, vcm, cfg, is_bip);
  }
}

bool AffectsExtractor::IsModifyingType(StmtType stmt_type, bool is_bip) {
  if (is_bip) {
    return stmt_type == StmtType::kRead || stmt_type == StmtType::kAssign;
  }
  return stmt_type == StmtType::kCall || stmt_type == StmtType::kRead ||
         stmt_type == StmtType::kAssign;
}
