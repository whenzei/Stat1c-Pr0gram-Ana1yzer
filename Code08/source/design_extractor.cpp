#pragma once

#include "design_extractor.h"
#include "semantic_error_exception.h"

DesignExtractor::DesignExtractor(PKB* pkb) { pkb_ = pkb; }

void DesignExtractor::UpdatePkb() {
  UpdateCFGRoots();
  PopulateAllNextPairs();
  UpdateParentT();
  UpdateCallT();
  UpdateUsesAndModifiesWithCallGraph();
  PopulateDominates();
  PopulateProgramCFG();
}

void DesignExtractor::CheckCyclicCalls() {
  if (pkb_->GetCallGraph()->HasCycle()) {
    throw SemanticErrorException("Cyclic call statements found.");
  }
}

void DesignExtractor::PopulateAllNextPairs() {
  CFG* cfg = pkb_->GetCombinedCFG();
  StmtNumPairList next_pair_list;
  for (auto entry : cfg->GetAdjList()) {
    int prev_stmt = entry.first;
    for (int& next_stmt : entry.second) {
      next_pair_list.push_back(std::make_pair(prev_stmt, next_stmt));
    }
  }
  pkb_->SetAllNextPairs(next_pair_list);
}

void DesignExtractor::UpdateParentT() {
  StmtNumList parent_stmts = pkb_->GetAllParent();

  for (auto& parent : parent_stmts) {
    StmtNumList direct_child_stmts = pkb_->GetChild(parent);
    for (auto& direct_child : direct_child_stmts) {
      DescentForChild(parent, direct_child);
    }
  }
}

void DesignExtractor::UpdateUsesAndModifiesWithCallGraph() {
  ProcIndexList toposorted_calls = pkb_->GetToposortedCalls();
  for (ProcIndex proc_id : toposorted_calls) {
    // get all call statement numbers that calls the proc_name
    StmtNumSet calling_stmts = pkb_->GetCallingStmts(proc_id);
    for (StmtNum call_stmt : calling_stmts) {
      ProcIndex proc_of_stmt =
          pkb_->GetProcIndex(pkb_->GetProcOfStmt(call_stmt));
      // get all ParentT of call statement
      StmtNumList parent_stmts = pkb_->GetParentT(call_stmt);

      // update uses for call statement AND its Parent(T)
      for (VarIndex used_var : pkb_->GetUsedVarP(proc_id)) {
        pkb_->InsertUsesS(call_stmt, used_var);
        for (StmtNum parent : parent_stmts) {
          pkb_->InsertUsesS(parent, used_var);
        }
        // update the procedure that contains this call statement
        pkb_->InsertUsesP(proc_of_stmt, used_var);
      }

      // update modifies for call statement AND its Parent(T)
      for (VarIndex modified_var : pkb_->GetModifiedVarP(proc_id)) {
        pkb_->InsertModifiesS(call_stmt, modified_var);
        for (StmtNum parent : parent_stmts) {
          pkb_->InsertModifiesS(parent, modified_var);
        }
        // update the procedure that contains this call statement
        pkb_->InsertModifiesP(proc_of_stmt, modified_var);
      }
    }
  }
}

void DesignExtractor::UpdateCallT() {
  ProcNameSet procs = pkb_->GetAllCallerName();
  for (auto& proc : procs) {
    ProcNameSet callee_procs = pkb_->GetCallee(proc);
    for (auto& direct_callee : callee_procs) {
      DescentForCallee(proc, direct_callee);
    }
  }
}

void DesignExtractor::PopulateDominates() {
  ProcNameList all_procs = pkb_->GetAllProcNames();
  for (auto& proc : all_procs) {
    CFG* cfg = pkb_->GetCFG(proc);
    // we know a vertex dominates another if the vertex is a cut vertex
    VertexSet all_vertices = cfg->GetAllVertices();

    for (Vertex v : all_vertices) {
      VertexSet unreachables = cfg->GetUnreachableVertices(v);
      pkb_->InsertDominates(v, unreachables);
    }
  }
}

void DesignExtractor::PopulateProgramCFG() {
  CFG* combined_cfg = pkb_->GetCombinedCFG();
  CFG* reversed_combined_cfg = pkb_->GetReverseCombinedCFG();
  pair<CFG, CFG> cfgs = ConnectProgramCFG(combined_cfg, reversed_combined_cfg);
  pkb_->SetProgramCFG(cfgs.first);
  pkb_->SetReverseProgramCFG(cfgs.second);
}

pair<CFG, CFG> DesignExtractor::ConnectProgramCFG(CFG* combined_cfg, CFG* rev_combined_cfg) {
  // must clone, or the combined cfg will be mutated
  CFG program_cfg = CFG(*combined_cfg);
  CFG rev_program_cfg = CFG(*rev_combined_cfg);
  VisitedMap visited = VisitedMap();

  for (auto v : program_cfg.GetAllVertices()) {
    // not visited yet
    if (!visited.count(v)) {
        DfsConnect(v, &program_cfg, &rev_program_cfg, &visited);
    }
  }

  return make_pair(program_cfg, rev_program_cfg);
}


void DesignExtractor::DfsConnect(const Vertex v, CFG* cfg, CFG* rev_cfg,
                                 VisitedMap* visited) {
  if (visited->count(v)) {
    return;
  }

  visited->emplace(v, true);

  VertexList terminal_nodes;

  StmtType stmt_type = pkb_->GetStmtType(v);
  if (stmt_type == StmtType::kCall) {
    // get neighbours before adding the root as neighbour
    VertexList neighbours = cfg->GetNeighboursList(v);
    ProcName proc_name = pkb_->GetProcName(pkb_->GetCalledProcedure(v));
    CFG* called_cfg = pkb_->GetCFG(proc_name);
    Vertex called_cfg_root = called_cfg->GetRoot();
    // add root of the procedure's cfg as neighbour of call statement's
    // previouses
    VertexList previouses = pkb_->GetPrevious(v);
    for (auto& previous : previouses) {
      cfg->AddEdge(previous, called_cfg_root);
      rev_cfg->AddEdge(called_cfg_root, previous);
    }

    // remove the whole call node
    // must be done before obtaining terminal nodes in case the call statement
    // is a terminal node
    cfg->RemoveNode(v);
    rev_cfg->RemoveNode(v);

    // get terminal nodes of the called cfg
    terminal_nodes = called_cfg->GetTerminalNodes();

    for (auto& neighbour : neighbours) {
      // add the neighbours to the terminal nodes of the procedure
      for (auto& terminal_node : terminal_nodes) {
        cfg->AddEdge(terminal_node, neighbour);
        rev_cfg->AddEdge(neighbour, terminal_node);
      }
    }

    // get all the neighbours of the previous node again and dfs
    for (auto& previous : previouses) {
      VertexList prev_neighbours = cfg->GetNeighboursList(previous);
      for (auto& prev_neighbour : prev_neighbours) {
        DfsConnect(prev_neighbour, cfg, rev_cfg, visited);
      }
    }
  } else {
    // Default path for non calls: Get the neighbours
    VertexList neighbours = cfg->GetNeighboursList(v);
    for (auto& neighbour : neighbours) {
      DfsConnect(neighbour, cfg, rev_cfg, visited);
    }
  }
}

void DesignExtractor::UpdateCFGRoots() {
  ProcNameList all_procs = pkb_->GetAllProcNames();
  for (auto& proc : all_procs) {
    CFG* cfg = pkb_->GetCFG(proc);
    Vertex min_vertex = GetMinVertex(cfg);
    cfg->SetRoot(min_vertex);
  }
}

int DesignExtractor::GetMinVertex(CFG* cfg) {
  VertexSet all_vertices = cfg->GetAllVertices();
  int min = INT_MAX;
  for (auto& vertex : all_vertices) {
    if (vertex < min) {
      min = vertex;
    }
  }
  return min;
}

void DesignExtractor::DescentForChild(StmtNum true_parent, StmtNum curr_stmt) {
  StmtNumList child_stmts = pkb_->GetChild(curr_stmt);
  for (auto& child : child_stmts) {
    pkb_->InsertParentT(true_parent, child);
    DescentForChild(true_parent, child);
  }
}

void DesignExtractor::DescentForCallee(ProcName true_caller,
                                       ProcName curr_callee) {
  ProcNameSet callees = pkb_->GetCallee(curr_callee);
  for (auto& callee : callees) {
    pkb_->InsertIndirectCallRelationship(true_caller, callee);
    DescentForCallee(true_caller, callee);
  }
}
