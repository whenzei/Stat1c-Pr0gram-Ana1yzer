#pragma once
#include "statement.h"

#include <string>

using std::string;

const int kTempArraySize = 10;

#ifndef STMT_TABLE_H
#define STMT_TABLE_H

class StmtTable {
private:
  Statement* stmt_table_[kTempArraySize];
  int num_stmt_;

public:
  // Constructor
  StmtTable();

  // Destructor
  ~StmtTable();
	
  // Takes in a statement in string format and its line number.
  // Prerequisite: The line number and statement are valid and non-null.
  // Checks if statement with the same line number already exists in the StmtTable and adds the statement if not.
  // Increments statement count.
  // Returns 0 if a statement with the same line number already exists.
  // Returns 1 if statement is successfully added to the StmtTable.
  int InsertStmt(int line_num, string stmt);

  // Returns a pointer to a list of all line numbers of all statements in the StmtTable.
  // Returns null if StmtTable is empty. 
  int* GetAllStmtNums();
};

#endif !STMT_TABLE_H