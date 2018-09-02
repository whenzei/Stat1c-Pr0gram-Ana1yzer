#pragma once

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "assign_table.h"

using std::map;
using std::string;
using std::stringstream;
using std::vector;

int AssignTable::Insert(int statement_number, string assign_statement) {
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

vector<string> AssignTable::List() { return assign_vector_; }
