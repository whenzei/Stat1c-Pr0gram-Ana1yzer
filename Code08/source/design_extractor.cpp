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
}

void DesignExtractor::CheckCyclicCalls() {
  if (pkb_->GetCallGraph()->HasCycle()) {
    throw SemanticErrorException("Cyclic call statements found, terminating.");
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
  CFG program_cfg = CFG(*pkb_->GetCombinedCFG());
  // program cfg will always have 1 as root
  program_cfg.SetRoot(1);
  DfsConnect(1, &program_cfg);
  pkb_->SetProgramCFG(program_cfg);
}

void DesignExtractor::UpdateCFGRoots() {
  ProcNameList all_procs = pkb_->GetAllProcNames();
  for (auto& proc : all_procs) {
    CFG* cfg = pkb_->GetCFG(proc);
    VertexSet all_vertices = cfg->GetAllVertices();
    int min = INT_MAX;
    for (auto& vertex : all_vertices) {
      if (vertex < min) {
        min = vertex;
      }
    }
    cfg->SetRoot(min);
  }
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

void DesignExtractor::DfsConnect(const Vertex vertex, CFG* cfg) {
  VisitedMap visited;
  for (auto& v : cfg->GetAllVertices()) {
    visited[v] = false;
  }

  DfsConnect(vertex, cfg, &visited);
}

void DesignExtractor::DfsConnect(const Vertex v, CFG* cfg,
                                 VisitedMap* visited) {
  if ((*visited)[v]) {
    return;
  }

  (*visited)[v] = true;

  VertexList terminal_nodes;

  // get neighbours before adding the root as neighbour
  VertexList neighbours = cfg->GetNeighboursList(v);

  StmtType stmt_type = pkb_->GetStmtType(v);
  if (stmt_type == StmtType::kCall) {
    ProcName proc_name = pkb_->GetCalledProcedure(v);
    CFG* called_cfg = pkb_->GetCFG(proc_name);
    // add root of the procedure's cfg as neighbour of call statement
    cfg->AddEdge(v, called_cfg->GetRoot());
    // get terminal nodes of the called cfg
    terminal_nodes = called_cfg->GetTerminalNodes();
  }

  for (auto& neighbour : neighbours) {
    if (stmt_type == StmtType::kCall) {
      // remove all neighbours of the call statement
      cfg->RemoveEdge(v, neighbour);

      // add the neighbours to the terminal nodes of the procedure
      for (auto& terminal_node : terminal_nodes) {
        cfg->AddEdge(terminal_node, neighbour);
      }
    }
    DfsConnect(neighbour, cfg, visited);
  }
}