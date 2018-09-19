#include "pql_result.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

PqlResult::PqlResult() {}

void PqlResult::AddColumnHeader(string var_name, int column_num) {
  column_header_.insert(std::make_pair(var_name, column_num));
}

void PqlResult::SetResultTable(ResultTable result_table) {
  this->result_table_ = result_table;
}

void PqlResult::SetColumnCount(int column_count) {
  this->column_count_ = column_count;
}

ColumnHeader PqlResult::GetColumnHeader() { return column_header_; }

ResultTable PqlResult::GetResultTable() { return result_table_; }

int PqlResult::GetColumnCount() { return column_count_; }
