#pragma once

#include "design_extractor.h"
#include "semantic_error_exception.h"

DesignExtractor::DesignExtractor(PKB* pkb) { pkb_ = pkb; }

void DesignExtractor::UpdatePkb() {
  UpdateCFGRoots();
  PopulateAllNextPairs();
  UpdateParentT();
  UpdateUsesAndModifiesWithCallGraph();
  UpdateCallT();
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
    StmtNumList calling_stmts = pkb_->GetCallingStmts(proc_id);
    for (StmtNum call_stmt : calling_stmts) {
      // get the procedures that has this call statement
      ProcIndexList caller_procs = pkb_->GetCaller(proc_id);
      // get all ParentT of call statement
      StmtNumList parent_stmts = pkb_->GetParentT(call_stmt);

      // update uses for call statement AND its Parent(T)
      for (VarIndex used_var : pkb_->GetUsedVarP(proc_id)) {
        pkb_->InsertUsesS(call_stmt, used_var);
        for (StmtNum parent : parent_stmts) {
          pkb_->InsertUsesS(parent, used_var);
        }

        // update the procedure that contains this call statement
        for (ProcIndex caller_proc : caller_procs) {
          pkb_->InsertUsesP(caller_proc, used_var);
        }
      }

      // update modifies for call statement AND its Parent(T)
      for (VarIndex modified_var : pkb_->GetModifiedVarP(proc_id)) {
        pkb_->InsertModifiesS(call_stmt, modified_var);
        for (StmtNum parent : parent_stmts) {
          pkb_->InsertUsesS(parent, modified_var);
        }
        // update the procedure that contains this call statement
        for (ProcIndex caller_proc : caller_procs) {
          pkb_->InsertModifiesP(caller_proc, modified_var);
        }
      }
    }
  }
}

void DesignExtractor::UpdateCallT() {
  ProcNameList procs = pkb_->GetAllCallerName();
  for (auto& proc : procs) {
    ProcNameList callee_procs = pkb_->GetCallee(proc);
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
  pkb_->SetProgramCFG(ConnectProgramCFG(combined_cfg));
  pkb_->SetReverseProgramCFG(ConnectProgramCFG(reversed_combined_cfg));
}

CFG DesignExtractor::ConnectProgramCFG(CFG* combined_cfg) {
  // must clone, or the combined cfg will be mutated
  CFG program_cfg = CFG(*combined_cfg);
  // program cfg might not always have 1 as root, such as when a call statement
  // is stmt#1 and removed
  Vertex min_vertex = GetMinVertex(&program_cfg);
  program_cfg.SetRoot(min_vertex);

  VisitedMap visited;
  for (auto& v : program_cfg.GetAllVertices()) {
    visited[v] = false;
  }

  DfsConnect(min_vertex, &program_cfg, &visited);
  return program_cfg;
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
  ProcNameList callees = pkb_->GetCallee(curr_callee);
  for (auto& callee : callees) {
    pkb_->InsertIndirectCallRelationship(true_caller, callee);
    DescentForCallee(true_caller, callee);
  }
}

void DesignExtractor::DfsConnect(const Vertex v, CFG* cfg,
                                 VisitedMap* visited) {
  if ((*visited)[v]) {
    return;
  }

  (*visited)[v] = true;

  VertexList terminal_nodes;

  StmtType stmt_type = pkb_->GetStmtType(v);
  if (stmt_type == StmtType::kCall) {
    // get neighbours before adding the root as neighbour
    VertexList neighbours = cfg->GetNeighboursList(v);
    ProcName proc_name = pkb_->GetCalledProcedure(v);
    CFG* called_cfg = pkb_->GetCFG(proc_name);
    Vertex called_cfg_root = called_cfg->GetRoot();
    // add root of the procedure's cfg as neighbour of call statement's
    // previouses
    VertexList previouses = pkb_->GetPrevious(v);
    for (auto& previous : previouses) {
      cfg->AddEdge(previous, called_cfg_root);
    }

    // remove the whole call node
    // must be done before obtaining terminal nodes in case the call statement
    // is a terminal node
    cfg->RemoveNode(v);

    // get terminal nodes of the called cfg
    terminal_nodes = called_cfg->GetTerminalNodes();

    for (auto& neighbour : neighbours) {
      // add the neighbours to the terminal nodes of the procedure
      for (auto& terminal_node : terminal_nodes) {
        cfg->AddEdge(terminal_node, neighbour);
      }
    }

    // get all the neighbours of the previous node again and dfs
    for (auto& previous : previouses) {
      VertexList prev_neighbours = cfg->GetNeighboursList(previous);
      for (auto& prev_neighbour : prev_neighbours) {
        DfsConnect(prev_neighbour, cfg, visited);
      }
    }
  } else {
    // Default path for non calls: Get the neighbours
    VertexList neighbours = cfg->GetNeighboursList(v);
    for (auto& neighbour : neighbours) {
      DfsConnect(neighbour, cfg, visited);
    }
  }
}