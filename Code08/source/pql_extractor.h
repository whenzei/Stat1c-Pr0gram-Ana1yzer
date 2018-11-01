#pragma once

#ifndef PQL_EXTRACTOR
#define PQL_EXTRACTOR

#include <stack>

#include "pkb.h"
#include "pql_global.h"

using std::stack;
using VarIndexSet = unordered_set<VarIndex>;
using AffectsTable = unordered_map<StmtNum, StmtNumSet>;
using LastModMap = unordered_map<Vertex, VarIndex>;
using WhileLastModMap = unordered_map<Vertex, LastModMap>;

// Helper class to extract information from PKB to evaluate:
// Next*, Affects and Affects*
class PqlExtractor {
 private:
  PKB pkb_;
  CFG* curr_affects_cfg_;
  VisitedMap curr_visited_;

  // Helper method
  //@params start is the StmtNum that should be in the LHS of all pairs
  //        res_list is passed by reference
  void FormPairBFS(StmtNum start, StmtNumPairList* res_list);

  // @params: curr the current vertex
  // @params: target the target vertex to reach
  // @returns true if the current vertex can reach the target vertex without
  // being modified by the given affects_var, false otherwise
  bool DfsAffects(Vertex curr, Vertex target, VarIndex affects_var);

  // @params: curr the current vertex
  // @params: affects_var the LHS of an assignment statement to check if it
  // affects other statements
  // @params: res_list the list of all StmtNum that is affected by affects_var
  // @return: there is no return value as pass by reference is used for res_list
  void DfsAffects(Vertex curr, VarIndex affects_var, StmtNumList* res_list);

  // @params: curr the current vertex
  // @params: rhs_vars the set of variables to be affected (contains variables
  // used by the statement of concern)
  // @params: affected_rhs_vars the set of variables already affected in the
  // current path
  // @params: res_list the list of all StmtNum that is affecting the rhs_vars
  void DfsAffects(Vertex curr, VarIndexSet rhs_vars,
                  VarIndexSet affected_rhs_vars, StmtNumList* res_list);

  // Helper to populate the AffectsTable using DFS
  // @params: Vertex the vertex to start from
  // @params: AffectsTable* the table to populate
  // @params: LastModMap the map to keep track of where each variable was last
  // modified
  // @params: WhileLastModMap the map to keep track of each while statement's
  // LastModMap
  void DfsAllAffects(Vertex v, AffectsTable* affects_table,
                     LastModMap last_mod_map,
                     WhileLastModMap while_last_mod_map);

  bool IsModifyingType(StmtType stmt_type);

  void ClearAffectsGlobals();

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

  //****************** Affects *******************************

  // @returns true if Affects(stmt_1, stmt_2) holds, else false
  bool IsAffects(StmtNum stmt_1, StmtNum stmt_2);

  // @returns a list of n that Affects(stmt_1, n) holds true
  StmtNumList GetAffects(StmtNum stmt_1);

  // @returns a list of n that Affects(n, stmt_num) holds true
  StmtNumList GetAffectedBy(StmtNum stmt_num);

  // Get the AffectsTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affected StmtNums
  AffectsTable GetAffectsTable();
  //*********************************************************

  //****************** AffectsBip *******************************
  AffectsTable PqlExtractor::GetAffectsBipTable();
};

#endif