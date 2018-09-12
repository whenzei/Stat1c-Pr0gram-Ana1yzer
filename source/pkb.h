#pragma once

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include "const_list.h"
#include "follows_table.h"
#include "proc_list.h"
#include "stmt_table.h"
#include "stmt_type_list.h"
#include "stmtlist_table.h"
#include "var_list.h"
#include "parent_table.h"

using StmtNumInt = int;

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
  StmtTable stmt_table_;
  StmtListTable stmtlist_table_;
  StmtTypeList stmt_type_list_;
  FollowsTable follows_table_;
  ParentTable parent_table_;

 public:
  // inserts the given procedure name into the procedure list
  // @param proc_name the procedure name to be inserted
  // @returns true if the procedure name cannot be found in the list and is
  // successfully inserted, false if the procedure name was already inside the list
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

  // inserts the given assign statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_num_int the statement number of the assign statement
  // @param stmtlist_index the statement list index the assign statement belongs
  // to
  // @param modified_var_name the variable name on the left hand side of the
  // assign statement
  // @param used_var_name_list the list of variable names on the right hand side
  // of the assign statement
  // @returns true if the statement number cannot be found in the table and the
  // assign statement is successfully inserted, false otherwise
  bool InsertAssignStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                        VarName modified_var_name,
                        VarNameSet used_var_name_set);

  // inserts the given while statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_num_int the statement number of the while statement
  // @param stmtlist_index the statement list index the while statement belongs
  // to
  // @param control_var_name_list the list of control variables of the while
  // statement
  // @returns true if the statement number cannot be found in the table and the
  // while statement is successfully inserted, false otherwise
  bool InsertWhileStmt(StmtNumInt stmt_num_int,
                       StmtListIndex parent_stmtlist_index,
                       StmtListIndex child_stmtlist_index,
                       VarNameSet control_var_name_set);

  // inserts the given if statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_num_int the statement number of the if statement
  // @param stmtlist_index the statement list index the if statement belongs to
  // @param control_var_name_list the list of control variables of the if
  // statement
  // @returns true if the statement number cannot be found in the table and the
  // if statement is successfully inserted, false otherwise
  bool InsertIfStmt(StmtNumInt stmt_num_int,
                    StmtListIndex parent_stmtlist_index,
                    StmtListIndex then_stmtlist_index,
                    StmtListIndex else_stmtlist_index,
                    VarNameSet control_var_name_set);

  // inserts the given read statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_num_int the statement number of the read statement
  // @param stmtlist_index the statement list index the read statement belongs
  // to
  // @param var_name the read variable
  // @returns true if the read statement is inserted successfully, false if
  // insertion fails (read statement was already inside StmtTable)
  bool InsertReadStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                      VarName var_name);

  // inserts the given print statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_num_int the statement number of the print statement
  // @param stmtlist_index the statement list index the print statement belongs
  // to
  // @param var_name the printed variable
  // @returns true if the print statement is inserted successfully, false if
  // insertion fails (print statement was already inside StmtTable)
  bool InsertPrintStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                       VarName var_name);

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
   
  // @returns true if Follows(followee, follower) holds
  bool IsFollows(StmtNumInt followee_stmt_num_int, StmtNumInt follower_stmt_num_int);

  // @returns true if FollowsT(followee, follower) holds
  bool IsFollowsT(StmtNumInt followee_stmt_num_int, StmtNumInt follower_stmt_num_int);

  // @returns stmt number of statement a's that satisfies Follows(stmt_num_int, a)
  StmtList GetFollows(StmtNumInt stmt_num_int);

  // @returns stmt number of statement a that satisfies FollowsT(stmt_num_int, a)
  StmtNum GetFollowsT(StmtNumInt stmt_num_int);

  // @returns list of stmt numbers of a's that satisfies Follows(a, stmt_num_int)
  StmtList GetFollowedBy(StmtNumInt stmt_num_int);

  // @returns stmt number of statement a that satisfies FollowsT(a, stmt_num_int)
  StmtNum GetFollowedByT(StmtNumInt stmt_num_int);
  
  // @returns true if follows table has any follows relationships
  bool HasFollowsRelationship();

  // @returns list of pairs of Follows relationships
  StmtNumPairList GetAllFollowsPair();

  // @returns list of pairs of FollowsT relationships
  StmtNumPairList GetAllFollowsTPair();

  // @returns true if the statement specified by parent_stmt_num is the direct parent
  // of the statement specified by child_stmt_num
  bool IsDirectParent(StmtNumInt parent_stmt_num_int, StmtNumInt child_stmt_num_int);

  // @returns true if the statement specified by parent_stmt_num is a parent (direct or
  // indirect) of the statement specified by child_stmt_num
  bool IsParent(StmtNumInt parent_stmt_num_int, StmtNumInt child_stmt_num_int);

  // @returns a list of the statement number (only one element) of the direct parent
  StmtNumList GetDirectParent(StmtNumInt stmt_num_int);

  // @returns a list of statement numbers of the parents (direct + indirect)
  StmtNumList GetAllParent(StmtNumInt stmt_num_int);

  // @returns a list of statement numbers of the direct children
  StmtNumList GetDirectChild(StmtNumInt stmt_num_int);

  // @returns a list of statement numbers of the children (direct + indirect)
  StmtNumList GetAllChild(StmtNumInt stmt_num_int);

  // @returns true if there exists any parent-child relationship
  bool HasParentRelationship();

 private:
  StmtNum ToString(int stmt_num_int);
};

#endif  // !SPA_PKB_H