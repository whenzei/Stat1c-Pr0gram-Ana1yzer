#pragma once

#ifndef NEXT_EXTRACTOR_H
#define NEXT_EXTRACTOR_H

#include "pkb.h"

class NextExtractor {
  PKB* pkb_;

  //@params: start is the StmtNum that should be in the LHS of all pairs
  //@params: res_list is passed by reference
  void FormPairBFS(StmtNum start, StmtNumPairSet* res_list);

 public:
  NextExtractor();
  NextExtractor(PKB* pkb);

  // @returns true if Next*(previous_stmt, next_stmt) holds
  bool IsNextT(StmtNum previous_stmt, StmtNum next_stmt);

  // @returns true if Next*(_, stmt_num) holds
  bool IsNextT(StmtNum stmt_num);

  // @returns true if Next*(stmt_num, _) holds
  bool IsPreviousT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(stmt_num, n)
  StmtNumSet GetNextT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(n, stmt_num)
  StmtNumSet GetPreviousT(StmtNum stmt_num);

  // @returns a list of all pairs of <n1, n2> that satisfy Next*(n1, n2)
  StmtNumPairSet GetAllNextTPairs();
};

#endif  // !NEXT_EXTRACTOR_H
