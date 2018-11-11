#pragma once

#ifndef SPA_PKB_H
#define SPA_PKB_H

// forward declaration due to circular dependencies
class AssignStmtData;
class WhileStmtData;
class ReadStmtData;
class PrintStmtData;
class IfStmtData;
class CallStmtData;
class StatementData;

#include "call_table.h"
#include "const_list.h"
#include "dominates_table.h"
#include "follows_table.h"
#include "graph.h"
#include "modifies_table.h"
#include "next_table.h"
#include "parent_table.h"
#include "pattern_table.h"
#include "pql_global.h"
#include "proc_list.h"
#include "stmt_table.h"
#include "stmt_type_list.h"
#include "uses_table.h"
#include "var_list.h"
// statement_data.h must be at the end due to forward declaration
#include "statement_data.h"

using CallGraph = Graph;

class PKB {
  ProcList proc_list_;
  VarList var_list_;
  ConstList const_list_;
  StmtTable stmt_table_;
  StmtTypeList stmt_type_list_;
  FollowsTable follows_table_;
  ParentTable parent_table_;
  PatternTable pattern_table_;
  ModifiesTable modifies_table_;
  UsesTable uses_table_;
  NextTable next_table_;
  CallGraph call_graph_;
  CallTable call_table_;
  DominatesTable dominates_table_;

 private:
  bool HandleInsertStatement(StatementData* stmt_data, StmtType stmt_type);
  void HandleInsertVariables(VarName variable, VarNameSet var_set);
  void HandleInsertVariables(VarNameSet var_set);
  void HandleInsertVariable(VarName variable, StmtType stmt_type, StmtNum stmt_num);
  void HandleInsertConstants(ConstValueSet constants);
  void HandleInsertPattern(StmtType stmt_type, void* stmt_data);

 public:
  // inserts the given procedure name into the procedure list
  // @param proc_name the procedure name to be inserted
  void InsertProcName(ProcName proc_name);

  // get all procedure names stored inside procedure list
  // @returns the list of procedure names (can be empty)
  ProcNameList GetAllProcNames();

  // get all procedure indices stored inside procedure list
  // @returns the list of procedure indices (can be empty)
  ProcIndexList GetAllProcIndices();

  // @returns the list of <p_id, p_id> (same proc index repeated twice)
  ProcIndexPairList GetAllProcIndexTwin();

  // @returns the list of <p_name, p_name> (same proc name repeated twice)
  ProcNamePairList GetAllProcNameTwin();

  // @return the procedure name-index map
  ProcIndexMap GetProcToIndexMapping();

  // @return the index-procedure name map
  IndexProcMap GetIndexToProcMapping();

  // @returns the corresponding procedure name
  ProcName GetProcName(ProcIndex index);

  // @returns the corresponding procedure name index
  ProcIndex GetProcIndex(ProcName proc_name);

  // get all variable indices stored inside variable list
  // @returns the list of variable indices (can be empty)
  VarIndexList GetAllVarIndices();

  // @returns the list of <v, v> (same v repeated twice)
  VarIndexPairList GetAllVarIndexTwin();

  // @return the variable name-index map
  VarIndexMap GetVarToIndexMapping();

  // @return the index-variable name map
  IndexVarMap GetIndexToVarMapping();

  // @returns the corresponding var name
  VarName GetVarName(VarIndex index);

  // @returns the corresponding var name index
  VarIndex GetVarIndex(VarName var_name);

  // @returns the variable modified in stmt_num if it is a read stmt
  VarIndex GetReadVar(StmtNum stmt_num);

  // @returns the variable used in stmt_num if it is a print stmt
  VarIndex GetPrintVar(StmtNum stmt_num);

  // @returns a list of read statements that modifies the variable
  StmtNumList GetReadStmt(VarName var_name);

  // @returns a list of print statements that uses the variable
  StmtNumList GetPrintStmt(VarName var_name);

  // @returns true if var_name is modified in any read stmt
  bool IsReadVar(VarName var_name);

  // @returns true if var_name is used in any print stmt
  bool IsPrintVar(VarName var_name);

  // @returns a list of all variables modified in some read stmt
  VarIndexList GetAllReadVar();

  // @returns a list of all variables used in some print stmt
  VarIndexList GetAllPrintVar();

  // @returns a list of all variables modified in some read stmt (repeat each
  // var_id to form a pair)
  VarIndexPairList GetAllReadVarTwin();

  // @returns a list of all variables used in some print stmt (repeat each
  // var_id to form a pair)
  VarIndexPairList GetAllPrintVarTwin();

  // get all constant values stored inside constant list
  // @returns the list of constant values (can be empty)
  ConstValueList GetAllConstValue();

  // @returns the list of <c, c> (same c repeated twice)
  ConstValuePairList GetAllConstValueTwin();

  // @returns a PqlDeclarationEntity enum to represent the statement type
  // (assign/while/if/read/print)
  StmtType GetStmtType(StmtNum stmt_num);

  // @returns the procedure that the statement belongs to
  ProcName GetProcOfStmt(StmtNum stmt_num);

  // @returns pointer to the call graph of the program
  CallGraph* GetCallGraph();

  // inserts an edge from the node referring to given curr_proc_name to the node
  // referring to given called_proc_name in the call graph
  void InsertEdgeInCallGraph(ProcName curr_proc_name,
                             ProcName called_proc_name);

  // @returns toposorted order of procedure calls in a ProcNameList
  vector<int> GetToposortedCalls();

  // inserts the given assign statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param AssignStmtData the encapsulation of statement data for PKB use
  void InsertAssignStmt(AssignStmtData*);

  // inserts the given while statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by WhileStmtData
  // structure
  void InsertWhileStmt(WhileStmtData*);

  // inserts the given if statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by IfStmtData structure
  void InsertIfStmt(IfStmtData* stmt_data);

  // inserts the given read statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by ReadStmtData
  // structure
  void InsertReadStmt(ReadStmtData* stmt_data);

  // inserts the given print statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by PrintStmtData
  // structure
  void InsertPrintStmt(PrintStmtData* stmt_data);

  // Inserts the given call statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the stmt data as encapsulated by the CallStmtData
  // structure
  void InsertCallStmt(CallStmtData* stmt_data);

  // Inserts a follows relationship between followee and follower
  void InsertFollows(StmtNum followee, StmtNum follower);

  // Inserts a modifies relationship between modifying_stmt and modified_var
  void InsertModifiesS(StmtNum modifying_stmt, VarName modified_var);

  void InsertModifiesS(StmtNum modifying_stmt, VarIndex modified_var_id);

  // Inserts a modifies relationship between modifying_proc and modified_var
  void InsertModifiesP(ProcName modifying_proc, VarName modified_var);

  void InsertModifiesP(ProcIndex modifying_proc_id, VarIndex modified_var_id);

  // Inserts a uses relationship between using_stmt and used_var
  void InsertUsesS(StmtNum using_stmt, VarName used_var);

  void InsertUsesS(StmtNum using_stmt, VarIndex used_var_id);

  // Inserts a uses relationship between using_proc and used_var
  void InsertUsesP(ProcName using_proc, VarName used_var);

  void InsertUsesP(ProcIndex using_proc_id, VarIndex used_var_id);

  // Inserts a direct parent relationship between parent_stmt_num and
  // child_stmt_num
  void InsertParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // Inserts an indirect parent relationship between parent_stmt_num and
  // child_stmt_num
  void InsertParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // Inserts a next relationship between previous_stmt and next_stmt
  void InsertNext(ProcName proc_name, StmtNum previous_stmt, StmtNum next_stmt);

  void SetAllNextPairs(StmtNumPairList next_pair_list);

  // get statement numbers for all statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllStmt();

  // @returns the list of <s, s> (same s repeated twice)
  StmtNumPairList GetAllStmtTwin();

  // get statement numbers for all assign statements stored inside stmt type
  // list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllAssignStmt();

  // @returns the list of <a, a> (same a repeated twice)
  StmtNumPairList GetAllAssignStmtTwin();

  // get statement numbers for all while statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllWhileStmt();

  // @returns the list of <w, w> (same w repeated twice)
  StmtNumPairList GetAllWhileStmtTwin();

  // get statement numbers for all if statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllIfStmt();

  // @returns the list of <ifs, ifs> (same ifs repeated twice)
  StmtNumPairList GetAllIfStmtTwin();

  // get statement numbers for all read statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllReadStmt();

  // @returns the list of <r, r> (same r repeated twice)
  StmtNumPairList GetAllReadStmtTwin();

  // get statement numbers for all print statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllPrintStmt();

  // @returns the list of <pr, pr> (same pr repeated twice)
  StmtNumPairList GetAllPrintStmtTwin();

  // get stmt numbers for all call stmts inside the stmt type list
  // @returns the list of stmt numbers
  StmtNumList GetAllCallStmt();

  // @returns the list of <call, call> (same call repeated twice)
  StmtNumPairList GetAllCallStmtTwin();

  /***********************
   * Is-A Functions *
   ***********************/
  // @returns true if var_id is a variable in the variable list
  bool IsVarIndex(VarIndex var_id);

  // @returns true if var_id is a variable in the variable list
  bool IsVarName(VarName var_name);

  // @returns true if stmt_num is a statement in the statement list
  bool IsStmtNum(StmtNum stmt_num);

  // @returns true if proc_name is a procedure in the procedure list
  bool IsProcName(ProcName proc_name);

  // @returns true if proc_id is a procedure in the procedure list
  bool IsProcIndex(ProcIndex proc_id);

  // @returns true if const_value is a constant in the constant list
  bool IsConstValue(ConstValue const_value);

  // @returns true if the given stmt_num is an assign statement
  bool IsAssignStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a while statement
  bool IsWhileStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is an if statement
  bool IsIfStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a read statement
  bool IsReadStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a print statement
  bool IsPrintStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a call statement
  bool IsCallStmt(StmtNum stmt_num);

  /***********************
   * Follows Table Functions *
   ***********************/

  // @returns true if Follows*(followee, follower) holds
  bool IsFollowsT(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns true if Follows(followee, follower) holds
  bool IsFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns a list of all n's that satisfy Follows*(stmt_num, n)
  StmtNumList GetFollowsT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(stmt_num, n)
  StmtNumList GetFollows(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(_, n)
  StmtNumList GetAllFollows();

  // @returns a list of all n's that satisfy Follows*(n, stmt_num)
  StmtNumList GetFollowedByT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(n, stmt_num)
  StmtNumList GetFollowedBy(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows*(n, _)
  StmtNumList GetAllFollowedBy();

  // @returns true if Follows(_, _) holds
  bool HasFollowsRelationship();

  // @returns list of all pairs of <s1, s2> that satisfy Follows*(s1, s2)
  StmtNumPairList GetAllFollowsTPair();

  // @returns list of all pairs of <s1, s2> that satisfy Follows(s1, s2)
  StmtNumPairList GetAllFollowsPair();

  /***********************
   * Parent Table Functions *
   ***********************/

  // @returns true if Parent(parent_stmt_num, child_stmt_num) holds
  bool IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns true if Parent*(parent_stmt_num, child_stmt_num) holds
  bool IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns a list of all n's that satisfy Parent(n, stmt_num)
  // only one element
  StmtNumList GetParent(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent*(n, stmt_num)
  StmtNumList GetParentT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent(n, _)/Parent*(n, _)
  StmtNumList GetAllParent();

  // @returns a list of all n's that satisfy Parent(stmt_num, n)
  StmtNumList GetChild(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent*(stmt_num, n)
  StmtNumList GetChildT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent(_, n)
  StmtNumList GetAllChild();

  // @returns true if Parent(_, _) holds
  bool HasParentRelationship();

  // @returns list of all pairs of <s1, s2> that satisfy Parent(s1, s2)
  StmtNumPairList GetAllParentPair();

  // @returns list of all pairs of <s1, s2> that satisfy Parent*(s1, s2)
  StmtNumPairList GetAllParentTPair();

  /***********************
   * Modifies Table Functions *
   ***********************/

  // @returns true if Modifies(stmt_num, var_name) holds
  bool IsModifiedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if Modifies(stmt_num, var_index) holds
  bool IsModifiedByS(StmtNum stmt_num, VarIndex var_index);

  // @returns true if Modifies(proc_name, var_name) holds
  bool IsModifiedByP(ProcName proc_name, VarName var_name);

  // @returns a list of all n's that satisfy Modifies(stmt_num, n)
  VarIndexList GetModifiedVarS(StmtNum stmt_num);

  // @params proc_name the name of the procedure to check Uses(proc_name, n)
  // @returns a list of all n's that satisfy Modifies(proc_name, n)
  VarIndexList GetModifiedVarP(ProcName proc_name);

  // @params proc_id the id of the proc_name to check Uses(proc_name, n)
  // @returns a list of all n's that satisfy Modifies(proc_name, n)
  VarIndexList GetModifiedVarP(ProcIndex proc_id);

  // @returns a list of all stmt_num's that satisfy Modifies(stmt_num, var_name)
  StmtNumList GetModifyingS(VarName var_name);

  // @returns a list of all indices of procedures that satisfy
  // Modifies(proc_name, var_name)
  ProcIndexList GetModifyingP(VarName var_name);

  // @returns a list of all stmt_num's that satisfy Modifies(stmt_num, _)
  StmtNumList GetAllModifyingS();

  // @returns a list of all proc_name's that satisfy Modifies(proc_name,
  // _)
  ProcIndexList GetAllModifyingP();

  // @returns a list of all pairs of <stmt_num, var_name> that satisfy
  // Modifies(stmt_num, var_name)
  StmtVarIndexPairList GetAllModifiesPairS();

  // @returns a list of all pairs of <proc_name, var_name> that satisfy
  // Modifies(proc_name, var_name)
  ProcVarPairList GetAllModifiesPairP();

  /***********************
   * Uses Table Functions *
   ***********************/

  // @returns a list of all n's that satisfy Uses(stmt_num, n)
  VarIndexList GetUsedVarS(StmtNum stmt_num);

  // @params proc_name the name of the procedure to check Uses(proc_name, n)
  // @returns a list of all n's that satisfy Uses(proc_name, n)
  VarIndexList GetUsedVarP(ProcName proc_name);

  // @params proc_id the id of the proc_name to check Uses(proc_name, n)
  // @returns a list of all n's that satisfy Uses(proc_name, n)
  VarIndexList GetUsedVarP(ProcIndex proc_id);

  // @returns a list of all stmt_num's that satisfy Uses(stmt_num, _)
  StmtNumList GetAllUsingStmt();

  // @returns a list of all proc_name's that satisfy Uses(proc_name, _)
  ProcIndexList GetAllUsingProc();

  // @returns a list of all stmt_num's that satisfy Uses(stmt_num, var_name)
  StmtNumList GetUsingStmt(VarName var_name);

  // @returns a list of all proc_name's that satisfy Uses(proc_name, var_name)
  ProcIndexList GetUsingProc(VarName var_name);

  // @returns true if Uses(stmt_num, var_name) holds
  bool IsUsedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if Uses(stmt_num, var_index) holds
  bool IsUsedByS(StmtNum stmt_num, VarIndex var_index);

  // @returns true if Uses(proc_name, var_name) holds
  bool IsUsedByP(ProcName proc_name, VarName var_name);

  // @returns a list of all pairs of <stmt_num, var_name> that satisfy
  // Uses(stmt_num, var_name)
  StmtVarIndexPairList GetAllUsesPairS();

  // @returns a list of all pairs of <proc_name, var_name> that satisfy
  // Uses(proc_name, var_name)
  ProcVarPairList GetAllUsesPairP();

  /***************************
   * Pattern Table Functions *
   ***************************/

  // @returns a list of a's that satisfy pattern a(var_name, _sub_expr_)
  // var_name can be an empty string, sub_expr can be an empty TokenList (to
  // represent underscore)
  StmtNumList GetAssignWithPattern(VarName var_name, TokenList sub_expr);

  // @returns a list of a's that satisfy pattern a(var_name, exact_expr)
  // var_name can be an empty string (to represent underscore)
  StmtNumList GetAssignWithExactPattern(VarName var_name, TokenList exact_expr);

  // @returns a list of all pairs of <a, v> that satisfy pattern a(v,
  // _sub_expr_)
  StmtVarPairList GetAllAssignPatternPair(TokenList sub_expr);

  // @returns a list of all pairs of <a, v> that satisfy pattern a(v,
  // exact_expr)
  StmtVarPairList GetAllAssignExactPatternPair(TokenList exact_expr);

  // @returns a list of w's that satisfy pattern w(var_name, _)
  // var_name can be an empty string to represent underscore
  StmtNumList GetWhileWithPattern(VarName var_name);

  // @returns a list of all pairs of <w, v> that satisfy pattern w(v, _)
  StmtVarPairList GetAllWhilePatternPair();

  // @returns a list of ifs' that satisfy pattern ifs(var_name, _)
  // var_name can be an empty string to represent underscore
  StmtNumList GetIfWithPattern(VarName var_name);

  // @returns a list of all pairs of <ifs, v> that satisfy pattern ifs(v, _)
  StmtVarPairList GetAllIfPatternPair();

  /***********************
   * Call Table Functions *
   ***********************/

  // Inserts an indirect caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false otherwise
  // @params caller procedure name and callee procedure name
  bool InsertIndirectCallRelationship(ProcName caller_proc,
                                      ProcName callee_proc);

  // Inserts a direct caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false otherwise
  // @params caller procedure name and callee procedure name
  bool InsertDirectCallRelationship(ProcName caller_proc, ProcName callee_proc);

  // Inserts a calls relationship to the call table.
  // @params stmt num of statement
  // @params proc name of the procedure *being called*
  void InsertCalls(StmtNum stmt_num, ProcName callee_proc);

  // Finds and returns a list of statement numbers calling the given proc.
  // @params proc index of the procedure being called
  // @returns a list of stmt numbers (can be empty)
  StmtNumList GetCallingStmts(ProcIndex callee_proc_id);

  // Finds and returns a list of pairs of all stmt numbers
  // and procedures called by that stmt number.
  // @returns a list of <stmt_num, proc> pairs
  StmtNumProcPairList GetAllCallingStmtPairs();

  // Finds and returns all indices of direct callees for given procedure.
  // @params caller procedure index
  // @returns a list containing all direct callees' indices for given proc (can
  // be empty)
  ProcIndexList GetCallee(ProcIndex caller_proc_id);

  // Finds and returns all names of direct callees for given procedure.
  // @params caller procedure name
  // @returns a list containing all direct callees' names for given proc (can be
  // empty)
  ProcNameList GetCallee(ProcName caller_proc);

  // Finds and returns all indices of callees for given procedure.
  // @params caller procedure index
  // @returns a list containing all callees' indices for given proc (can be
  // empty)
  ProcIndexList GetCalleeT(ProcIndex caller_proc_id);

    // Finds and returns all indices of callees for given procedure.
  // @params caller procedure name
  // @returns a list containing all callees' indices for given proc (can be
  // empty)
  ProcIndexList GetCalleeT(ProcName caller_proc);


  // Finds and returns all indices of direct callers for given procedure.
  // @params callee procedure index
  // @returns a list containing all direct callers' indices for given proc (can
  // be empty)
  ProcIndexList GetCaller(ProcIndex callee_proc_id);

  // Finds and returns all indices of direct callers for given procedure.
  // @params callee procedure name
  // @returns a list containing all direct callers' indices for given proc (can
  // be empty)
  ProcIndexList GetCaller(ProcName callee_proc);

  // Finds and returns all indices of callers for given procedure.
  // @params callee procedure index
  // @returns a list containing all callers' indices for given proc (can be
  // empty)
  ProcIndexList GetCallerT(ProcIndex callee_proc_id);

  // Finds and returns all indices of callers for given procedure.
  // @params callee procedure name
  // @returns a list containing all callers' indices for given proc (can be
  // empty)
  ProcIndexList GetCallerT(ProcName callee_proc);

  // @returns all procedures calling some other proc (can be empty)
  ProcIndexList GetAllCaller();

  // Special method to retrieve all caller procs in String format.
  // To be only used by DE when populating PKB.
  // @returns a list of all procedures calling some other proc, in string form
  ProcNameList GetAllCallerName();

  // @returns all procedures being called by some other proc (can be empty)
  ProcIndexList GetAllCallee();

  // Special method to retrieve all caller procs in String format.
  // To be only used by DE when populating PKB.
  // @returns a list of all procedures calling some other proc, in string form
  ProcNameList GetAllCalleeName();

  // @returns all procedures being called by some other proc (in pairs)
  ProcIndexPairList GetAllCalleeTwin();

  // @returns a list of all <caller, direct callee> pairs
  ProcIndexPairList GetAllCallPairs();

  // @returns a list of all <caller, callee> pairs
  ProcIndexPairList GetAllCallTPairs();

  // @returns true if Call(caller, callee) is true
  bool IsCall(ProcName caller_proc, ProcName callee_proc);

  // @returns true if Call*(caller, callee) is true
  bool IsCallT(ProcName caller_proc, ProcName callee_proc);

  // @returns true if callee is called by any other proc
  bool IsCalledProc(ProcName callee_proc);
  bool IsCalledProc(ProcIndex callee_proc_id);

  // @returns true if Call Table has any calls relationships
  // false if otherwise
  bool HasCallsRelationship();

  // @returns the index of procedure called at given statement number if exists, empty
  // string otherwise
  ProcIndex GetCalledProcedure(StmtNum stmt_num);

  /************************
   * Next Table Functions *
   ************************/

  // @returns true if Next(previous_stmt, next_stmt) holds
  bool IsNext(StmtNum previous_stmt, StmtNum next_stmt);

  // @returns true if Next(_, stmt_num) holds
  bool IsNext(StmtNum stmt_num);

  // @returns true if Next(stmt_num, _) holds
  bool IsPrevious(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next(stmt_num, n)
  StmtNumList GetNext(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next(n, stmt_num)
  StmtNumList GetPrevious(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next(_, n)
  StmtNumList GetAllNext();

  // @returns a list of all n's that satisfy Next(n, _)
  StmtNumList GetAllPrevious();

  // @returns a list of all pairs of <n1, n2> that satisfy Next(n1, n2)
  StmtNumPairList GetAllNextPairs();

  // @returns true if Next(_, _) holds
  bool HasNextRelationship();

  // @returns the cfg belonging to the given procedure
  CFG* GetCFG(ProcName proc_name);

  // @returns the reverse cfg belonging to the given procedure
  CFG* GetReverseCFG(ProcName proc_name);

  // @returns the combined cfg of the program
  CFG* GetCombinedCFG();

  // @returns the reversed combined cfg of the program
  CFG* GetReverseCombinedCFG();

  // Parser calls this method to notify pkb end of parse.
  // PKB will proceed with design extraction
  void NotifyParseEnd();

  /*****************************
   * AffectsBip Table Functions *
   *****************************/
  // Set the given cfg as the program CFG
  // Solely used by the Design Extractor class
  void SetProgramCFG(const CFG& program_cfg);

  // Set the given cfg as the reversed program CFG
  // Solely used by the Design Extractor class
  void SetReverseProgramCFG(const CFG& reversed_program_cfg);

  // Get the program CFG, with call statements removed and the call
  // statements' previous statements connected to the called procedure's root,
  // and the terminal nodes of the called procedure connected to the neighbours
  // of the (removed) call statement node
  // @returns the program CFG
  CFG* GetProgramCFG();

  // Similar to GetProgramCFG, with the only difference being the direction of
  // the edges are reversed
  // @returns the reversed program CFG
  CFG* GetReverseProgramCFG();

  /*****************************
   * Dominates Table Functions *
   *****************************/

  // inserts dominates relationships between dominating_vertex and
  // dominated_vertices
  void InsertDominates(Vertex dominating_vertex, VertexSet dominated_vertices);

  // @returns true if Dominates(dominating_stmt, dominated_stmt) holds
  bool IsDominates(StmtNum dominating_stmt, StmtNum dominated_stmt);

  // @returns true if Dominates(stmt_num, _) holds
  bool IsDominating(StmtNum stmt_num);

  // @returns true if Dominates(_, stmt_num) holds
  bool IsDominated(StmtNum stmt_num);

  // @returns a list of all s's that satisfy Dominates(s, stmt_num)
  StmtNumList GetDominating(StmtNum stmt_num);

  // @returns a list of all s's that satisfy Dominates(stmt_num, s)
  StmtNumList GetDominated(StmtNum stmt_num);

  // @returns a list of all s's that satisfy Dominates(s, _)
  StmtNumList GetAllDominating();

  // @returns a list of all s's that satisfy Dominates(_, s)
  StmtNumList GetAllDominated();

  // @returns a list of all pairs of <s1, s1> that satisfy Dominates(s1, s2)
  StmtNumPairList GetAllDominatesPairs();

  // @returns true if Dominates(_, _) holds
  bool HasDominatesRelationship();
};

#endif  // !SPA_PKB_H
