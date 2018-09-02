#include "var_table.h"

#include <string>

using std::string;

VarTable::VarTable() {
  num_var_ = 0;
}

VarTable::~VarTable() {
  delete[] * var_table_;
}

int VarTable::InsertVar(string var_name) {
  *var_table_[num_var_] = var_name;
  num_var_++;
  return (num_var_ -1);
}

string* VarTable::GetAllVar() {
  if (num_var_ == 0) { return NULL; }
  else { return *var_table_; }
}