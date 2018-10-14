#pragma once

#include "design_extractor.h"

DesignExtractor::DesignExtractor(PKB* pkb) { pkb_ = pkb; }

void DesignExtractor::UpdatePkb() {
  UpdateParentT();
  UpdateCallUsesAndModifies();
  UpdateCallT();
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

void DesignExtractor::UpdateCallUsesAndModifies() {
  vector<string> toposorted_calls = pkb_->GetToposortedCalls();
  for (auto proc_name : toposorted_calls) {
    // get all used and modified vars by procedure name
    VarNameList used_vars = pkb_->GetUsedVarP(proc_name);
    VarNameList modified_vars = pkb_->GetModifiedVarP(proc_name);
    // get all call statement numbers that calls the proc_name
    StmtNumList calling_stmts = pkb_->GetCallingStmts(proc_name);
    for (auto stmt : calling_stmts) {
      // get the procedures that has this call statement
      ProcNameList caller_procs = pkb_->GetCaller(proc_name);
      for (auto used_var : used_vars) {
        pkb_->InsertUsesS(stmt, used_var);
        for (auto caller : caller_procs) {
          pkb_->InsertUsesP(caller, used_var);
        }
      }
      for (auto modified_var : modified_vars) {
        pkb_->InsertModifiesS(stmt, modified_var);
        for (auto caller : caller_procs) {
          pkb_->InsertModifiesP(caller, modified_var);
        }
      }
    }
  }
}

void DesignExtractor::UpdateCallT() {
  ProcNameList procs = pkb_->GetAllCaller();
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