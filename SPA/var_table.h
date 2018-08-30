#pragma once

#include <string>

#ifndef VAR_TABLE_H
#define VAR_TABLE_H

using std::string;

class VarTable {
private:
  string* var_table;

public:
  // Constructor
  VarTable();
  // Takes in the variable name and inserts a new variable in the VarTable.
  // Returns the index of the variable.
  int InsertVar(string var_name);
  // Returns an array of all var_name in the VarTable. 
  // Returns null if VarTable is empty.
  string* VarTable::GetAllVar();
};

#endif  !VAR_TABLE_H