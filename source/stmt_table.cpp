#include "stmt_table.h"
#include "statement.h"

const int kSuccess = 0;
const int kFail = -1;

StmtTable::StmtTable() {
  num_stmt_ = 0;
}

int StmtTable::InsertStmt(int line_num, string stmt) {
  for (int i = 0; i < num_stmt_; i++) {
    if ((stmt_table_[i]).GetLineNum()==line_num) { return kFail; }
  }

  Statement newStmt(line_num, stmt);
  stmt_table_[num_stmt_] = newStmt;
  num_stmt_++;
  return kSuccess;
}

std::vector<int> StmtTable::GetAllStmtNums() {
  int stmt_nums[kTempArraySize];
  std::vector<int> stmt_num;
	
  for (int i = 0; i < num_stmt_; i++) {
    stmt_num.push_back((stmt_table_[i]).GetLineNum());
    stmt_nums[i] = (stmt_table_[i]).GetLineNum();
  }

  return stmt_num;
}