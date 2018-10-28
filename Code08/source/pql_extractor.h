#pragma once

#ifndef PQL_EXTRACTOR
#define PQL_EXTRACTOR

#include <stack>

#include "pkb.h"
#include "pql_global.h"

using std::stack;
using LastModifiedMap = unordered_map<VarIndex, StmtNum>;

// Helper class to extract information from PKB to evaluate:
// Next*, Affects and Affects*
class PqlExtractor {
 private:
  PKB pkb_;
  stack<LastModifiedMap*> modified_vars_stack_;
  CFG* curr_affects_cfg_;
  VisitedMap curr_visited_;

  // Helper method
  //@params start is the StmtNum that should be in the LHS of all pairs
  //        res_list is passed by reference
  void FormPairBFS(StmtNum start, StmtNumPairList* res_list);

  bool DfsAffects(Vertex curr, Vertex target, VarName affects_var);

  void DfsAffects(Vertex curr, VarName affects_var, StmtNumList* res_list);

  bool IsModifyingType(StmtType stmt_type);

  void ClearVisitedMap();

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

  // @returns a list of all pairs of <n1, n2> that satisfy Next*(n1, n2)
  StmtNumPairList GetAllNextTPairs();
  
  //****************** Affects* *******************************

  // @returns true if Affects(stmt_1, stmt_2) holds, else false
  bool IsAffects(StmtNum stmt_1, StmtNum stmt_2);

  // @returns a list of n that Affects(stmt_1, n) holds true
  StmtNumList GetAffects(StmtNum stmt_1);
  //*********************************************************
};

#endif