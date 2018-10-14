#pragma once

#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include "pkb.h"

class DesignExtractor {
 private:
  PKB* pkb_;

  void UpdateParentT();
  void UpdateUsesAndModifiesWithCallGraph();

  //******** Helper Methods **************

  // Search for child of current stmt_num recursively
  // and updates PKB follows* relation
  //
  // Used in UpdateParentT()
  void DescentForChild(StmtNum true_parent, StmtNum curr_stmt);

  //**************************************
 public:
  DesignExtractor(PKB* pkb);
  void UpdatePkb();
};

#endif