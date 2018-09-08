#pragma once

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include "const_list.h"
#include "proc_list.h"
#include "stmt_table.h"
#include "stmt_type_list.h"
#include "stmtlist_table.h"
#include "var_list.h"

using StmtNumInt = int;

class PKB {
  ProcList proc_list_;
  VarList
      var_list_;  // TODO: insert vars to var list in all InsertXxStmt functions
  ConstList const_list_;
  StmtTable stmt_table_;  // TODO: insert stmt to stmt table in all InsertXxStmt
                          // functions
  StmtListTable stmtlist_table_;  // TODO: insert stmt to stmtlist table in all
                                  // InsertXxStmt functions
  StmtTypeList stmt_type_list_;   // TODO: insert stmt to stmt type list in all
                                  // InsertXxStmt functions

 public:
  // inserts the given procedure name into the procedure list
  // @param proc_name the procedure name to be inserted
  // @returns true if the procedure name cannot be found in the list and is
  // successfully inserted, false if the procedure name is empty, false if the
  // procedure name was already inside the list
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

  // inserts the given assign statement into the StmtTable, StmtTypeList and StmtListTable
  // @param stmt_num_int the statement number of the assign statement
  // @param stmtlist_index the statement list index the assign statement belongs to
  // @param lfs_var_name the variable name on the left hand side of the assign statement
  // @param rfs_var_name_list the list of variable names on the right hand side of the assign
  // statement
  // @returns true if the statement number cannot be found in the table and the assign 
  // statement is successfully inserted, false otherwise
  bool InsertAssignStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index, VarName lfs_var_name, VarNameList rhs_var_name_list);

  // inserts the given while statement into the StmtTable, StmtTypeList and StmtListTable
  // @param stmt_num_int the statement number of the while statement
  // @param stmtlist_index the statement list index the while statement belongs to
  // @param control_var_name_list the list of control variables of the while statement
  // @returns the StmtListIndex of the child stmtlist if the while statement is inserted
  // successfully, -1 if insertion fails (while statement was already inside StmtTable)
  StmtListIndex InsertWhileStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index, VarNameList control_var_name_list);

  // get statement numbers for all statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllStmt();

  // get statement numbers for all assign statements stored inside stmt type list
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

 private:
  StmtNum ToString(int stmt_num_int);
};

#endif  // !SPA_PKB_H