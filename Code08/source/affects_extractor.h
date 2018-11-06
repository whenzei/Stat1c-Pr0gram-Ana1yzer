#pragma once

#ifndef AFFECTS_EXTRACTOR_H
#define AFFECTS_EXTRACTOR_H

#include "pkb.h"

using VarIndexSet = unordered_set<VarIndex>;
using AffectsTable = Graph;
using LastModMap = unordered_map<Vertex, VarIndex>;
using WhileLastModMap = unordered_map<Vertex, LastModMap>;

class AffectsExtractor {
 private:
  PKB* pkb_;
  bool has_set_affects_tables_;
  bool has_set_affects_bip_tables_;
  AffectsTable affects_table_;
  AffectsTable affected_by_table_;
  AffectsTable affects_bip_table_;
  AffectsTable affected_by_bip_table_;

  bool EvaluateIsAffects(StmtNum stmt_1, StmtNum stmt_2, bool is_bip);

  bool EvaluateIsAffects(StmtNum stmt, bool is_bip);

  bool EvaluateIsAffected(StmtNum stmt, bool is_bip);

  VertexSet EvaluateGetAffects(StmtNum stmt, bool is_bip);

  VertexSet EvaluateGetAffectedBy(StmtNum stmt, bool is_bip);

  /* SETTER METHODS */
  // Sets the affects_table_ and affected_by_table_ of this class
  void SetAffectsTables();

  // Sets the affects_bip_table_ and affected_by_bip_table_ of this class
  void SetAffectsBipTables();

  /* DFS METHODS */

  // @params: curr the current vertex
  // @params: target the target vertex to reach
  // @params: CFG* the pointer to the CFG to run DFS on
  // @params: VisitedMap* the map to keep track of visited vertices
  // @returns true if the current vertex can reach the target vertex without
  // being modified by the given affects_var, false otherwise
  bool DfsIsAffects(Vertex curr, Vertex target, VarIndex affects_var, CFG* cfg,
                    VisitedMap* visited);

  // @params: curr the current vertex
  // @params: affects_var the variable (belonging to LHS of the assignment
  // statement of concern)
  // @params: CFG* the pointer to the CFG to run DFS on
  // @params: VisitedMap* the map to keep track of visited vertices
  // @returns true if affects_var affects any other assignment statements
  bool DfsIsAffects(Vertex curr, VarIndex affects_var, CFG* cfg,
                    VisitedMap* visited);

  // @params: curr the current vertex
  // @params: used_vars the set of variables to be affected (contains variables
  // used by the statement of concern)
  // @params: affected_used_vars the set of variables already affected in the
  // current path
  // @params: CFG* the pointer to the CFG to run DFS on
  // @params: VisitedMap* the map to keep track of visited vertices
  // @returns true if any of the used_vars is affected (vertex) by any other
  // assignment statement
  bool DfsIsAffected(Vertex curr, VarIndexSet used_vars,
                     VarIndexSet affected_used_vars, CFG* cfg,
                     VisitedMap* visited);

  // @params: curr the current vertex
  // @params: affects_var the LHS of an assignment statement to check if it
  // affects other statements
  // @params: res_list the set of all StmtNum that is affected by affects_var
  // @params: CFG* the pointer to the CFG to run DFS on
  // @params: VisitedMap* the map to keep track of visited vertices
  // @return: there is no return value as pass by reference is used for res_list
  void DfsGetAffects(Vertex curr, VarIndex affects_var, VertexSet* res_list,
                     CFG* cfg, VisitedMap* visited);

  // @params: curr the current vertex
  // @params: used_vars the set of variables to be affected (contains variables
  // used by the statement of concern)
  // @params: affected_used_vars the set of variables already affected in the
  // current path
  // @params: res_list the set of all StmtNum that is affecting the rhs_vars
  // @params: CFG* the pointer to the CFG to run DFS on
  // @params: VisitedMap* the map to keep track of visited vertices
  // @return: there is no return value as pass by reference is used for res_list
  void DfsGetAffectedBy(Vertex curr, VarIndexSet used_vars,
                        VarIndexSet affected_used_vars, VertexSet* res_list,
                        CFG* cfg, VisitedMap* visited);

  // Helper to populate the AffectsTable and AffectedByTable using DFS
  // @params: Vertex the vertex to start from
  // @params: AffectsTable* affects_table the AffectsTable to populate
  // @params: AffectsTable* affected_by_table the AffectedByTable to populate
  // @params: LastModMap the map to keep track of where each variable was last
  // modified
  // @params: WhileLastModMap the map to keep track of each while statement's
  // LastModMap
  // @params: CFG* pointer to cfg to run DFS on
  void DfsSetAffectsTables(Vertex v, AffectsTable* affects_table,
                           AffectsTable* affected_by_table,
                           LastModMap last_mod_map,
                           WhileLastModMap while_last_mod_map,
                           WhileLastModMap prev_while_last_mod_map, CFG* cfg);

  // @returns true if StmtType is either kAssign, kCall, or kRead
  bool IsModifyingType(StmtType stmt_type);

 public:
  AffectsExtractor();
  AffectsExtractor(PKB* pkb);

  /************
  * IsAffects *
  *************/

  // @returns true if Affects(stmt_1, stmt_2) holds, else false
  bool IsAffects(StmtNum stmt_1, StmtNum stmt_2, bool is_bip = false);

  // @returns true if stmt affects any statement
  bool IsAffects(StmtNum stmt, bool is_bip = false);

  // @returns true if stmt is affected by any statement
  bool IsAffected(StmtNum stmt, bool is_bip = false);

  // @returns a list of n that Affects(stmt_1, n) holds true
  VertexSet GetAffects(StmtNum stmt_1, bool is_bip = false);

  // @returns a list of n that Affects(n, stmt_num) holds true
  VertexSet GetAffectedBy(StmtNum stmt_num, bool is_bip = false);

  // @returns set of all statements stmt such that Affects(stmt, _) holds true
  VertexSet GetAllAffects(bool is_bip = false);

  // @returns set of all statements stmt such that Affects(_, stmt) holds true
  VertexSet GetAllAffectedBy(bool is_bip = false);

  // Get the AffectsTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affected StmtNums
  AffectsTable GetAffectsTable();

  // Get the AffectedByTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affecting StmtNums
  AffectsTable GetAffectedByTable();

  /************
  * IsAffects* *
  *************/

  // Checks and returns whether Affects*(stmt_1, stmt_2) is true
  // @returns true if stmt_1 affects stmt_2
  bool IsAffectsT(StmtNum stmt_1, StmtNum stmt_2, bool is_bip = false); 

  // Checks and returns whether Affects*(stmt, n) is true for
  // some arbitrary statement n
  // @returns true if stmt affects any statement
  bool IsAffectsT(StmtNum stmt, bool is_bip = false);

  // Checks and returns whether Affects*(n, stmt) is true for
  // some arbitrary statement n
  // @returns true if stmt is affected by any statement
  bool IsAffectedByT(StmtNum stmt, bool is_bip = false);

  // Returns a set of statements that stmt Affects
  // i.e., where Affects*(stmt, n)
  // @returns a set of statements n
  VertexSet GetAffectsT(StmtNum stmt, bool is_bip = false);

  // Returns a set of statements that stmt is Affected by
  // i.e., where Affects*(n, stmt)
  // @returns a set of statements n
  VertexSet GetAffectedByT(StmtNum stmt, bool is_bip = false);

  // Returns a set of statements n that affects any other statement
  // Logically equivalent to GetAllAffects(is_bip)
  // @returns a set of statements n
  VertexSet GetAllAffectsT(bool is_bip = false);

  // Returns a set of statements n that are affected by any other statement
  // Logically equivalent to GetAllAffectedBy(is_bip);
  // @returns a set of statements n
  VertexSet GetAllAffectedByT(bool is_bip = false);

  /************
  * AffectsBip *
  *************/

  // Get the AffectsBipTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affectedBip StmtNums
  AffectsTable GetAffectsBipTable();

  // Get the AffectedByBipTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affectingBip StmtNums
  AffectsTable GetAffectedByBipTable();

};

#endif  // !AFFECTS_EXTRACTOR_H
