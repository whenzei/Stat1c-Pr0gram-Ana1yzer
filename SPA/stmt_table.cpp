#include "stmt_table.h"

#include <string>

using std::string;

const int kTempArraySize = 10;

StmtTable::StmtTable() {
	stmt_table = new string[kTempArraySize];
}

int StmtTable::InsertStmt(string stmt) {
  return 0;
}

string StmtTable::GetStmt(int stmt_num) {
	return stmt_table[stmt_num];
}

string* StmtTable::GetAllStmt() {
	return stmt_table;
}