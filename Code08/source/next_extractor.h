#pragma once

#ifndef NEXT_EXTRACTOR_H
#define NEXT_EXTRACTOR_H

#include "pkb.h"

using NextTTable = Graph;
using NextTMap = unordered_map<int, unordered_set<int>>;

class NextExtractor {
  PKB* pkb_;
  NextTTable next_t_table_;
  NextTTable previous_t_table_;

  void SetNextTTables();
  void BFSSetNextTTables(StmtNum start);

  NextTMap GetTypedNextTMap(StmtType type);

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
  StmtNumList GetNextT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(n, stmt_num)
  StmtNumList GetPreviousT(StmtNum stmt_num);

  // Get the NextT mapping of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all nextT StmtNums
  NextTMap GetNextTMap();

  /* Helper API for PQLEvaluator to call specific typed NextT table */
  NextTMap GetAssignNextTMap();
  NextTMap GetWhileNextTMap();
  NextTMap GetIfNextTMap();
  NextTMap GetCallNextTMap();
  NextTMap GetReadNextTMap();
  NextTMap GetPrintNextTMap();
};

#endif  // !NEXT_EXTRACTOR_H
