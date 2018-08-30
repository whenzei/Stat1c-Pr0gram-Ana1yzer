#include "var_table.h"
#include <string>

#define TEMP_ARRAY_SIZE 10

VarTable::VarTable() {
	var_table = new std::string[TEMP_ARRAY_SIZE];
}

int VarTable::InsertVar(std::string var_name) {
	return 0;
}

std::string* VarTable::GetAllVar() {
	return var_table;
}