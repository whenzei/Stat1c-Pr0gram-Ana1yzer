#include "var_table.h"

#include <string>

using std::string;

const int kSuccess = 0;
const int kFail = -1;

VarTable::VarTable() {
  num_var_ = 0;
}

VarTable::~VarTable() {
  delete[] * var_table_;
}

int VarTable::InsertVar(string var_name) {
  for (int i = 0; i < num_var_; i++) {
    if (*var_table_[i] == var_name) { return kFail; }
  }

  *var_table_[num_var_] = var_name;
  num_var_++;
  return kSuccess;
}

string* VarTable::GetAllVar() {
  if (num_var_ == 0) { return NULL; }
  else { return *var_table_; }
}