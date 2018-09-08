#pragma once

#include "assign_table.h"
#include "proc_list.h"
#include "var_list.h"
#include "const_list.h"
#include "stmt_table.h"
#include "stmtlist_table.h"

#ifndef SPA_PKB_H
#define SPA_PKB_H

class PKB {
 private:
  ProcList proc_list_;
  AssignTable assign_table_;
  VarList var_list_; //TODO: insert vars to var list in all InsertXxStmt functions
  ConstList const_list_;
  StmtTable stmt_table_; //TODO: insert stmt to stmt table in all InsertXxStmt functions
  StmtListTable stmtlist_table_; //TODO: insert stmt to stmtlist table in all InsertXxStmt functions

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

  /**
   * inserts the given assign statement into the assignment table
   * @param statement_number the statement number of the assign statement
   * @param assign_statement the assign statement to be inserted
   * @returns 0 if the statement number cannot be found in the table and the
   * assign statement is successfully inserted
   * @returns -1 if the assign statement is empty
   * @returns -1 if the statement number was already in the table
   */
  int InsertAssignStmt(StmtNumInt statement_number, Stmt assign_statement);

  /**
   * get statement numbers for all assign statements stored inside assignment
   * table
   * @returns the vector of statement numbers(can be empty)
   */
  StmtList GetAllAssignStmt();
};

#endif  // !SPA_PKB_H