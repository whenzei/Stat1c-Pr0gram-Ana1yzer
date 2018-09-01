#include "var_table.h"

#include <string>

using std::string;

VarTable::VarTable() {
	num_var = 0;
}

VarTable::~VarTable() {
	delete[] * var_table;
}

int VarTable::InsertVar(string var_name) {
	*var_table[num_var] = var_name;
	num_var++;
	return num_var;
}

string* VarTable::GetAllVar() {
  return *var_table;
}