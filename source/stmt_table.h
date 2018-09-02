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
	
  // Takes in the statement in string format and inserts it in the StmtTable.
  // Increments statement count.
  // Returns the index of the statement just added.
  int InsertStmt(int line_num, string stmt);

  // Returns a pointer to a list of all line numbers of all statements in the StmtTable.
  // Returns null if StmtTable is empty. 
  int* GetAllStmtNums();
};

#endif !STMT_TABLE_H