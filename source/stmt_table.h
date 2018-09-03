#pragma once

#ifndef STMT_TABLE_H
#define STMT_TABLE_H

#include "statement.h"

#include <string>
#include <vector>

using std::string;

const int kTempArraySize = 10;

class StmtTable {
  Statement stmt_table_[kTempArraySize];
  int num_stmt_ = 0;

public:
  // Constructor
  StmtTable();
	
  // Takes in a statement in string format and its line number.
  // Prerequisite: The line number and statement are valid and non-null.
  // Checks if statement with the same line number already exists in the StmtTable and adds the statement if not.
  // Increments statement count.
  // Returns 0 if a statement with the same line number already exists.
  // Returns 1 if statement is successfully added to the StmtTable.
  int InsertStmt(int line_num, string stmt);

  // Returns a vector of all line numbers of all statements in the StmtTable.
  std::vector<int> GetAllStmtNums();
};

#endif !STMT_TABLE_H