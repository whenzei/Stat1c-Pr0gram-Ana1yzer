#pragma once

#include <map>
#include <string>
#include <list>

using std::map;
using std::string;
using std::list;

typedef int StmtNum;
typedef string Stmt;
typedef list<string> StmtList;

#ifndef SPA_ASSIGN_TABLE_H
#define SPA_ASSIGN_TABLE_H

/**
 * The assignment table class for the PKB component
 * Used to store assign statements that are passed into PKB from the parser
 */
class AssignTable {
  
  map<StmtNum, Stmt> assign_map_;
  StmtList assign_vector_;

 public:
  /**
   * inserts the given assign statement into the assignment table
   * @param statement_number the statement number of the assign statement
   * @param assign_statement the assign statement to be inserted
   * @returns 0 if the statement number cannot be found in the table and the
   * assign statement is successfully inserted
   * @returns -1 if the assign statement is empty
   * @returns -1 if the statement number was already in the table
   */
  int InsertAssignStmt(StmtNum statement_number, Stmt assign_statement);

  /**
   * get statement numbers for all assign statements stored inside assignment
   * table
   * @returns the vector of statement numbers(can be empty)
   */
  StmtList GetAssignTable();
};

#endif  // !SPA_ASSIGN_TABLE_H
