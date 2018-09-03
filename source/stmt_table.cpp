#include "stmt_table.h"
#include "statement.h"

#include <string>

using std::string;

const int kSuccess = 0;
const int kFail = -1;

StmtTable::StmtTable() {
  num_stmt_ = 0;
}

StmtTable::~StmtTable() {
  delete[] * stmt_table_;
}

int StmtTable::InsertStmt(int line_num, string stmt) {
  for (int i = 0; i < num_stmt_; i++) {
    if (stmt_table_[i]->GetLineNum() == line_num) { return kFail; }
  }

  Statement newStmt(line_num, stmt);
  *stmt_table_[num_stmt_] = newStmt;
  num_stmt_++;
  return kSuccess;
}

int* StmtTable::GetAllStmtNums() {
  int stmt_nums[kTempArraySize];
	
  if (num_stmt_ == 0) { return NULL;  }
  for (int i = 0; i < num_stmt_; i++) {
    stmt_nums[i] = stmt_table_[i]->GetLineNum();
  }

  return stmt_nums;
}