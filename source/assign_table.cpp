#pragma once

#include "assign_table.h"
#include <sstream>

using std::stringstream;

int AssignTable::InsertAssignStmt(StmtNumInt statement_number, Stmt assign_statement) {
  if (assign_statement.compare("") == 0) {
    return -1;
  } else if (assign_map_.find(statement_number) != assign_map_.end()) {
    return -1;
  } else {
    assign_map_[statement_number] = assign_statement;
    stringstream statement_number_ss;
    statement_number_ss << statement_number;
    assign_vector_.push_back(statement_number_ss.str());
    return 0;
  }
}

StmtList AssignTable::GetAssignTable() { return assign_vector_; }
