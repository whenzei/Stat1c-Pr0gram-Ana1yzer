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

  // Update the root node of all the CFGs to be the smallest vertex number in
  // the CFG
  // Rationale: When parser inserts a statement, it inserts the child
  // statements before its parents due to recursive descent. Current graph root
  // is set to the first node inserted, and not the smallest vertex number. This
  // method solves that by getting the minimum of all vertex numbers and setting
  // that as the root
  void UpdateCFGRoots();

  //******** Helper Methods **************
  // @returns the minimum vertex number found in the given CFG
  int GetMinVertex(CFG* cfg);

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

  // @params: CFG* pointer to cfg to run DfsConnect on
  // @returns the connected CFG
  pair<CFG, CFG> ConnectProgramCFG(CFG* cfg, CFG* rev_cfg);

  void DfsConnect(const Vertex v, CFG* cfg, CFG* rev_cfg, VisitedMap* visited);

  //**************************************
 public:
  DesignExtractor(PKB* pkb);

  // extract data from PKB and updates various relations such as ParentT and
  // Calls' Modifies and Uses
  void UpdatePkb();

  // Check if call graph has cyclic calls,
  // @throws SemanticException if cycle exists.
  void CheckCyclicCalls();

  void PopulateAllNextPairs();

  // Insert both reversed and normal program CFG into the PKB for AffectsBip
  // relation
  void PopulateProgramCFG();
};

#endif