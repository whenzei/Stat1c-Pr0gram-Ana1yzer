#include "var_table.h"

#include <string>

using std::string;

const int kTempArraySize = 10;

VarTable::VarTable() {
  var_table = new string[kTempArraySize];
}

int VarTable::InsertVar(string var_name) {
  return 0;
}

string* VarTable::GetAllVar() {
  return var_table;
}