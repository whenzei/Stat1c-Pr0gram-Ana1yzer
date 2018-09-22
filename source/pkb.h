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
#include "parent_table.h"
#include "pql_enum.h"
#include "proc_list.h"
#include "statement_data.h"
#include "stmt_table.h"
#include "stmt_type_list.h"
#include "stmtlist_table.h"
#include "uses_table.h"
#include "var_list.h"

using StmtNumInt = int;
using StmtNumPairList = vector<pair<string, string>>;

class PKB {
  ProcList proc_list_;
  VarList var_list_;
  ConstList const_list_;
  StmtTable stmt_table_;  // TODO: insert stmt to stmt table in all InsertXxStmt
                          // functions
  StmtListTable stmtlist_table_;  // TODO: insert stmt to stmtlist table in all
                                  // InsertXxStmt functions
  StmtTypeList stmt_type_list_;   // TODO: insert stmt to stmt type list in all
                                  // InsertXxStmt functions
  FollowsTable follows_table_;
  ParentTable parent_table_;
  ModifiesTable modifies_table_;
  UsesTable uses_table_;

 public:
  // inserts the given procedure name into the procedure list
  // @param proc_name the procedure name to be inserted
  // @returns true if the procedure name cannot be found in the list and is
  // successfully inserted, false if the procedure name was already inside the
  // list
  bool InsertProcName(ProcName proc_name);

  // get all procedure names stored inside procedure list
  // @returns the list of procedure names (can be empty)
  ProcNameList GetAllProcName();

  // get all variable names stored inside variable list
  // @returns the list of variable names (can be empty)
  VarNameList GetAllVarName();

  // inserts the given constant value into the constant list
  // @param const_value the constatnt value to be inserted
  // @returns true if the constant value cannot be found in the list and is
  // successfully inserted, false if the value was already in the list
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

  // Expose follows_table InsertFollows method to Parser
  void InsertFollows(StmtNum followee, StmtNum follower);

  // Expose modifies_table InsertModifies method to Parser
  // TODO: add InsertModifies(proc_name, modified_var) for iter 2
  void InsertModifies(StmtNum modifying_stmt, VarName modified_var);

  // Expose uses_table InsertUses method to Parser
  // TODO: add InsertUses(proc_name, modified_var) for iter 2
  void InsertUses(StmtNum using_stmt, VarName used_var);

  // Expose parent_table InsertParent method to Parser
  void InsertParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // Expose parent_table InsertParentT method to Parser
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

  // @returns true if FollowsT(followee, follower) holds
  bool IsFollowsT(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns true if Follows(followee, follower) holds
  bool IsFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns stmt number of statement a's that satisfies FollowsT(stmt_num_int,
  // a)
  StmtList GetFollowsT(StmtNum stmt_num);

  // @returns stmt number containing statement a that satisfies
  // Follows(stmt_num_int, a)
  StmtList GetFollows(StmtNum stmt_num);

  // @returns a list of all statements that follow some other statement
  StmtList GetAllFollows();

  // @returns list of stmt numbers of a's that satisfies FollowsT(a,
  // stmt_num_int)
  StmtList GetFollowedByT(StmtNum stmt_num);

  // @returns list containing statement a that satisfies Follows(a,
  // stmt_num_int)
  StmtList GetFollowedBy(StmtNum stmt_num);

  // @returns a list of all statements that are followed by some other statment
  StmtList GetAllFollowedBy();

  // @returns true if follows table has any follows relationships
  bool HasFollowsRelationship();

  // @returns list of pairs of FollowsT relationships
  StmtNumPairList GetAllFollowsTPair();

  // @returns list of pairs of Follows relationships
  StmtNumPairList GetAllFollowsPair();

  // @returns true if the statement specified by parent_stmt_num is the direct
  // parent of the statement specified by child_stmt_num
  bool IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns true if the statement specified by parent_stmt_num is a parent
  // (direct or indirect) of the statement specified by child_stmt_num
  bool IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns a list of the statement number (only one element) of the direct
  // parent
  StmtNumList GetParent(StmtNum stmt_num);

  // @returns a list of statement numbers of the parents (direct + indirect)
  StmtNumList GetParentT(StmtNum stmt_num);

  // @returns a list of statement numbers of statements that are parents to some
  // statement
  StmtNumList GetAllParent();

  // @returns a list of statement numbers of the direct children
  StmtNumList GetChild(StmtNum stmt_num);

  // @returns a list of statement numbers of the children (direct + indirect)
  StmtNumList GetChildT(StmtNum stmt_num);

  // @returns a list of statement numbers of statements that are children to
  // some statement
  StmtNumList GetAllChild();

  // @returns true if there exists any parent-child relationship
  bool HasParentRelationship();

  // returns a list of parent-child pairs (direct)
  StmtNumPairList GetAllParentPair();

  // returns a list of parent-child pairs (direct + indirect)
  StmtNumPairList GetAllParentTPair();

  // @returns true modifies(stmt_num_int, var_name) holds
  bool IsModifiedByS(StmtNum stmt_num, VarName var_name);

  // @returns true modifies(proc_name, var_name) holds
  bool IsModifiedByP(ProcName proc_name, VarName var_name);

  // @returns a list of variables that are modified at the given statement
  VarNameList GetModifiedVarS(StmtNum stmt_num);

  // @returns a list of variables that are modified in the given procedure
  VarNameList GetModifiedVarP(ProcName proc_name);

  // @returns a list of variables that are modified in any statement/procedure
  VarNameList GetAllModifiedVar();

  // @returns a list of statements that the given variable is modified in
  StmtNumList GetModifyingS(VarName var_name);

  // @returns a list of procedures that the given variable is modified in
  ProcNameList GetModifyingP(VarName var_name);

  // @returns a list of all statements that modify some variable
  StmtNumList GetAllModifyingS();

  // @returns a list of all procedures that modify some variable
  ProcNameList GetAllModifyingP();

  // @returns true if there exists any modifies relationship
  bool HasModifiesRelationship();

  // @returns a list of all pairs of <modifying_stmt_num, modified_var_name>
  StmtVarPairList GetAllModifiesPairS();

  // @returns a list of all pairs of <modifying_proc_name, modified_var_name>
  ProcVarPairList GetAllModifiesPairP();

  // @returns a list of all variables that are used by any proc and/or stmt
  VarNameList GetAllUsedVar();

  // @returns a list of variables used in the statement identified by stmt_num
  VarNameList GetUsedVarS(StmtNum stmt_num);

  // @returns a list of variables used in the procedure identified by proc_name
  VarNameList GetUsedVarP(ProcName proc_name);

  // @returns a list of all statements that use any variable
  StmtNumList GetAllUsingStmt();

  // @returns a list of all procedures that use any variable
  ProcNameList GetAllUsingProc();

  // @returns a list of all statements that use the variable identified by
  // var_name
  StmtNumList GetUsingStmt(VarName var_name);

  // @returns s a list of all procedures that use the variable identified by
  // var_name
  ProcNameList GetUsingProc(VarName var_name);

  // @returns true if var_name is used in stmt_num, false if otherwise
  bool IsUsedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if var_name is used in proc_name, false if otherwise
  bool IsUsedByP(ProcName proc_name, VarName var_name);

  // @returns true if Uses Table contains any uses relationships, false if
  // otherwise
  bool HasUsesRelationship();

  // @returns a list of all <stmt_num, var_name> pairs of statements using a
  // variable and the variable name
  StmtVarPairList GetAllUsesPairS();

  // @returns a list of all <proc_name, var_name> pairs of procedures using a
  // variable and the variable name
  ProcVarPairList GetAllUsesPairP();



 private:
  bool HandleInsertStatement(StatementData* stmt_data, StmtType stmt_type);
  void HandleInsertVariables(VarName variable, VarNameSet var_set);
  void HandleInsertVariables(VarNameSet var_set);
  void HandleInsertVariable(VarName variable);
  void HandleInsertConstants(ConstValueSet constants);
};

#endif  // !SPA_PKB_H