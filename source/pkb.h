#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "proc_table.h"
#include "assign_table.h"

using std::string;
using std::vector;

/* sample code that came with pkb.h
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer
*/

#ifndef SPA_PKB_H
#define SPA_PKB_H

class PKB {
 private:
  ProcTable proc_table_;
  AssignTable assign_table_;

 public:
  /**
   * inserts the given procedure name into the procedure table
   * @param proc_name the procedure name to be inserted
   * @returns 0 if the procedure name cannot be found in the table and is successfully inserted 
   *         -1 if the procedure name is empty 
   *         -1 if the procedure name was already inside the table
   */
  int InsertProc(string proc_name);

  /**
   * get all procedure names stored inside procedure table
   * @returns the vector of procedure names (can be empty)
   */
  vector<string> GetAllProc();

  /**
   * inserts the given assign statement into the assignment table
   * @param statement_number the statement number of the assign statement
   * @param assign_statement the assign statement to be inserted
   * @returns 0 if the statement number cannot be found in the table and the
   * assign statement is successfully inserted
   * @returns -1 if the assign statement is empty
   * @returns -1 if the statement number was already in the table
   */
  int InsertAssign(int statement_number, string assign_statement);

  /**
   * get statement numbers for all assign statements stored inside assignment
   * table
   * @returns the vector of statement numbers(can be empty)
   */
  vector<string> GetAllAssign();

  /* sample code that came with pkb.h
  static VarTable* var_table_;
  static int SetProcToAst(PROC p, TNode* r);
  static TNode* GetRootAst(PROC p);
  */
};

#endif  // !SPA_PKB_H