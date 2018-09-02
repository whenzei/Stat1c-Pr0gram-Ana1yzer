#pragma once

#include <string>
#include <map>
#include <vector>

#include "assign_table.h"

using namespace std;

int AssignTable::Insert(int statement_number, string assign_statement) {
  if (assign_statement.compare("") == 0) {
    return -1;
  } else if (assign_map_.find(statement_number) != assign_map_.end()) {
    return -1;
  } else {
    assign_map_[statement_number] = assign_statement;
	assign_vector_.push_back(to_string(statement_number));
	// to-do: insert to statement table (to be added when Sophie's StmtTable class is ready)
    return 0;
  }
}

vector<string> AssignTable::List() { return assign_vector_; }
