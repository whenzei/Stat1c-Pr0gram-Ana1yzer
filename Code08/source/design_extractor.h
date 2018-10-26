#pragma once

#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include "pkb.h"

class DesignExtractor {
 private:
  PKB* pkb_;

  void UpdateParentT();
  void UpdateUsesAndModifiesWithCallGraph();
  void UpdateCallT();
  void PopulateDominates();

  //******** Helper Methods **************

  // Search for child of current stmt_num recursively
  // and updates PKB follows* relation
  //
  // Used in UpdateParentT()
  void DescentForChild(StmtNum true_parent, StmtNum curr_stmt);

  // Search for callee of current proc recursively
  // and updates PKB calls* relation
  //
  // Used in UpdateCallT()
  void DescentForCallee(ProcName true_caller, ProcName curr_callee);

  //**************************************
 public:
  DesignExtractor(PKB* pkb);

  // extract data from PKB and updates various relations such as ParentT and
  // Calls' Modifies and Uses
  void UpdatePkb();

  // Check if call graph has cyclic calls,
  // @throws SemanticException if cycle exists.
  void CheckCyclicCalls();
};

#endif