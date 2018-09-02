#include "stmt_table.h"
#include "statement.h"

#include <string>

using std::string;

StmtTable::StmtTable() {
  num_stmt = 0;
}

StmtTable::~StmtTable() {
  delete[] * stmt_table;
}

// Prerequisite: The line number and statement are valid and non-null.
int StmtTable::InsertStmt(int line_num, string stmt) {
  Statement newStmt(line_num, stmt);
  *stmt_table[num_stmt] = newStmt;
  num_stmt++;
  return (num_stmt-1);
}

int* StmtTable::GetAllStmtNums() {
  int stmt_nums[kTempArraySize];
	
  if (num_stmt == 0) { return NULL;  }
  for (int i = 0; i < num_stmt; i++) {
    stmt_nums[i] = stmt_table[i]->GetLineNum();
  }

  return stmt_nums;
}