#pragma once

#ifndef PQL_EXTRACTOR
#define PQL_EXTRACTOR

#include "pql_global.h"
#include "pkb.h"

// Helper class to extract information from PKB to evaluate:
// Next*, Affects and Affects*
class PqlExtractor {
 private:
  PKB pkb_;

 public:
  PqlExtractor(PKB pkb);

  //**************** Next* *********************************

  // @returns true if Next*(previous_stmt, next_stmt) holds
  bool IsNextT(StmtNum previous_stmt, StmtNum next_stmt);

  // @returns true if Next*(_, stmt_num) holds
  bool IsNextT(StmtNum stmt_num);

  // @returns true if Next*(stmt_num, _) holds
  bool IsPreviousT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(stmt_num, n)
  StmtNumList GetNextT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(n, stmt_num)
  StmtNumList GetPreviousT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(_, n)
  StmtNumList GetAllNextT();

  // @returns a list of all n's that satisfy Next*(n, _)
  StmtNumList GetAllPreviousT();

  // @returns a list of all pairs of <n1, n2> that satisfy Next*(n1, n2)
  StmtNumPairList GetAllNextTPairs();
  
  //*********************************************************

};

#endif