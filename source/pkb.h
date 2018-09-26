#pragma once

#ifndef SPA_PKB_H
#define SPA_PKB_H

// forward declaration due to circular dependencies
class AssignStmtData;
class WhileStmtData;
class ReadStmtData;
class PrintStmtData;
class IfStmtData;
class StatementData;

#include "const_list.h"
#include "follows_table.h"
#include "modifies_table.h"
#include "pattern_table.h"
#include "parent_table.h"
#include "pql_enum.h"
#include "proc_list.h"
#include "stmt_table.h"
#include "stmt_type_list.h"
#include "stmtlist_table.h"
#include "uses_table.h"
#include "var_list.h"
#include "statement_data.h"

using StmtNumInt = int;

class PKB {
  ProcList proc_list_;
  VarList var_list_;
  ConstList const_list_;
  StmtTable stmt_table_;
  StmtListTable stmtlist_table_;
  StmtTypeList stmt_type_list_;
  FollowsTable follows_table_;
  ParentTable parent_table_;
  PatternTable pattern_table_;
  ModifiesTable modifies_table_;
  UsesTable uses_table_;

 public:
  // inserts the given procedure name into the procedure list
  // @param proc_name the procedure name to be inserted
  bool InsertProcName(ProcName proc_name);

  // get all procedure names stored inside procedure list
  // @returns the list of procedure names (can be empty)
  ProcNameList GetAllProcName();

  // get all variable names stored inside variable list
  // @returns the list of variable names (can be empty)
  VarNameList GetAllVarName();

  // inserts the given constant value into the constant list
  // @param const_value the constatnt value to be inserted
  bool InsertConstValue(ConstValue const_value);

  // get all constant values stored inside constant list
  // @returns the list of constant values (can be empty)
  ConstValueList GetAllConstValue();

  // return a PqlDeclarationEntity enum to represent the statement type
  // (assign/while/if/read/print)
  StmtType GetStmtType(StmtNum stmt_num);

  // inserts the given assign statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param AssignStmtData the encapsulation of statement data for PKB use
  void InsertAssignStmt(AssignStmtData*);

  // inserts the given while statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by WhileStmtData structure
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

  // Inserts a follows relationship between followee and follower
  void InsertFollows(StmtNum followee, StmtNum follower);

  // Inserts a modifies relationship between modifying_stmt and modified_var
  void InsertModifiesS(StmtNum modifying_stmt, VarName modified_var);

  // Inserts a modifies relationship between modifying_proc and modified_var
  void InsertModifiesP(ProcName modifying_proc, VarName modified_var);

  // Inserts a uses relationship between using_stmt and used_var
  void InsertUsesS(StmtNum using_stmt, VarName used_var);

  // Inserts a uses relationship between using_proc and used_var
  void InsertUsesP(ProcName using_proc, VarName used_var);

  // Inserts a direct parent relationship between parent_stmt_num and
  // child_stmt_num
  void InsertParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // Inserts an indirect parent relationship between parent_stmt_num and
  // child_stmt_num
  void InsertParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // get statement numbers for all statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllStmt();

  // get statement numbers for all assign statements stored inside stmt type
  // list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllAssignStmt();

  // get statement numbers for all while statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllWhileStmt();

  // get statement numbers for all if statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllIfStmt();

  // get statement numbers for all read statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllReadStmt();

  // get statement numbers for all print statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllPrintStmt();

  // Follows table public functions

  // @returns true if Follows*(followee, follower) holds
  bool IsFollowsT(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns true if Follows(followee, follower) holds
  bool IsFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns a list of all n's that satisfy Follows*(stmt_num, n)
  StmtList GetFollowsT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(stmt_num, n)
  StmtList GetFollows(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(_, n)
  StmtList GetAllFollows();

  // @returns a list of all n's that satisfy Follows*(n, stmt_num)
  StmtList GetFollowedByT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(n, stmt_num)
  StmtList GetFollowedBy(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows*(n, _)
  StmtList GetAllFollowedBy();

  // @returns true if Follows(_, _) holds
  bool HasFollowsRelationship();

  // @returns list of all pairs of <s1, s2> that satisfy Follows*(s1, s2)
  StmtNumPairList GetAllFollowsTPair();

  // @returns list of all pairs of <s1, s2> that satisfy Follows(s1, s2)
  StmtNumPairList GetAllFollowsPair();

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

  // @returns true if Modifies(stmt_num, var_name) holds
  bool IsModifiedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if Modifies(proc_name, var_name) holds
  bool IsModifiedByP(ProcName proc_name, VarName var_name);

  // @returns a list of all n's that satisfy Modifies(stmt_num, n)
  VarNameList GetModifiedVarS(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Modifies(proc_name, n)
  VarNameList GetModifiedVarP(ProcName proc_name);

  // @returns a list of all stmt_num's that satisfy Modifies(stmt_num, var_name)
  StmtNumList GetModifyingS(VarName var_name);

  // @returns a list of all proc_name's that satisfy Modifies(proc_name,
  // var_name)
  ProcNameList GetModifyingP(VarName var_name);

  // @returns a list of all stmt_num's that satisfy Modifies(stmt_num, _)
  StmtNumList GetAllModifyingS();

  // @returns a list of all proc_name's that satisfy Modifies(proc_name,
  // _)
  ProcNameList GetAllModifyingP();

  // @returns a list of all pairs of <stmt_num, var_name> that satisfy
  // Modifies(stmt_num, var_name)
  StmtVarPairList GetAllModifiesPairS();

  // @returns a list of all pairs of <proc_name, var_name> that satisfy
  // Modifies(proc_name, var_name)
  ProcVarPairList GetAllModifiesPairP();

  // @returns a list of all n's that satisfy Uses(stmt_num, n)
  VarNameList GetUsedVarS(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Uses(proc_name, n)
  VarNameList GetUsedVarP(ProcName proc_name);

  // @returns a list of all stmt_num's that satisfy Uses(stmt_num, _)
  StmtNumList GetAllUsingStmt();

  // @returns a list of all proc_name's that satisfy Uses(proc_name, _)
  ProcNameList GetAllUsingProc();

  // @returns a list of all stmt_num's that satisfy Uses(stmt_num, var_name)
  StmtNumList GetUsingStmt(VarName var_name);

  // @returns a list of all proc_name's that satisfy Uses(proc_name, var_name)
  ProcNameList GetUsingProc(VarName var_name);

  // @returns true if Uses(stmt_num, var_name) holds
  bool IsUsedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if Uses(proc_name, var_name) holds
  bool IsUsedByP(ProcName proc_name, VarName var_name);

  // @returns a list of all pairs of <stmt_num, var_name> that satisfy
  // Uses(stmt_num, var_name)
  StmtVarPairList GetAllUsesPairS();

  // @returns a list of all pairs of <proc_name, var_name> that satisfy
  // Uses(proc_name, var_name)
  ProcVarPairList GetAllUsesPairP();

  // @returns a list of a's that satisfy pattern a(var_name, _sub_expr_)
  // var_name can be an empty string, sub_expr can be an empty TokenList (to
  // represent underscore)
  StmtNumList GetAssignWithPattern(VarName var_name, TokenList sub_expr);

  // @returns a list of a's that satisfy pattern a(var_name, exact_expr)
  // var_name can be an empty string (to represent underscore)
  StmtNumList GetAssignWithExactPattern(VarName var_name,
                                        TokenList exact_expr);

  // @returns a list of all pairs of <a, v> that satisfy pattern a(v, _sub_expr_)
  StmtVarPairList GetAllAssignPatternPair(TokenList sub_expr);

  // @returns a list of all pairs of <a, v> that satisfy pattern a(v, exact_expr)
  StmtVarPairList GetAllAssignExactPatternPair(TokenList exact_expr);

  // Parser calls this method to notify pkb end of parse.
  // PKB will proceed with design extraction
  void NotifyParseEnd();

 private:
  bool HandleInsertStatement(StatementData* stmt_data, StmtType stmt_type);
  void HandleInsertVariables(VarName variable, VarNameSet var_set);
  void HandleInsertVariables(VarNameSet var_set);
  void HandleInsertVariable(VarName variable);
  void HandleInsertConstants(ConstValueSet constants);
  void HandleInsertPattern(StmtType stmt_type, void* stmt_data);
};

#endif  // !SPA_PKB_H