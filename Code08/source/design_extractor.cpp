#pragma once

#include "design_extractor.h"
#include "semantic_error_exception.h"

DesignExtractor::DesignExtractor(PKB* pkb) { pkb_ = pkb; }

void DesignExtractor::UpdatePkb() {
  UpdateParentT();
  UpdateUsesAndModifiesWithCallGraph();
  UpdateCallT();
}

void DesignExtractor::CheckCyclicCalls() {
  if (pkb_->GetCallGraph()->HasCycle()) {
    throw SemanticErrorException("Cyclic call statements found, terminating.");
  }
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
  vector<string> toposorted_calls = pkb_->GetToposortedCalls();
  for (auto proc_name : toposorted_calls) {
    // get all call statement numbers that calls the proc_name
    StmtNumList calling_stmts = pkb_->GetCallingStmts(proc_name);
    for (auto call_stmt : calling_stmts) {
      // get the procedures that has this call statement
      ProcIndexList caller_procs = pkb_->GetCaller(proc_name);
      // get all ParentT of call statement
      StmtNumList parent_stmts = pkb_->GetParentT(call_stmt);

      // update uses for call statement AND its Parent(T)
      for (auto used_var : pkb_->GetUsedVarP(proc_name)) {
        pkb_->InsertUsesS(call_stmt, used_var);
        for (auto parent : parent_stmts) {
          pkb_->InsertUsesS(parent, used_var);
        }

        // update the procedure that contains this call statement
        for (auto caller : caller_procs) {
          pkb_->InsertUsesP(caller, used_var);
        }
      }

      // update modifies for call statement AND its Parent(T)
      for (auto modified_var : pkb_->GetModifiedVarP(proc_name)) {
        pkb_->InsertModifiesS(call_stmt, modified_var);
        for (auto parent : parent_stmts) {
          pkb_->InsertUsesS(parent, modified_var);
        }
        // update the procedure that contains this call statement
        for (auto caller : caller_procs) {
          pkb_->InsertModifiesP(caller, modified_var);
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