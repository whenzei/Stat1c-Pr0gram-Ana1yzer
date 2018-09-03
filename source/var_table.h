#pragma once

#include <string>

#ifndef VAR_TABLE_H
#define VAR_TABLE_H

using std::string;

const int kTempArraySize = 10;

class VarTable {
private:
  string *var_table_[kTempArraySize];
  int num_var_;

public:
  // Constructor
  VarTable();

  // Destructor
  ~VarTable();
  
  // Takes in the variable name in string format.
  // Prerequisite: The variable name is non-null.
  // Checks if the same variable name already exists in the VarTable, then adds variable to the VarTable if not.
  // Increments variable count. 
  // Returns 0 if the same variable already exists in the VarTable.
  // Returns 1 if variable is successfully added to the VarTable.
  int InsertVar(string var_name);

  // Returns pointer to an array of all var_name in the VarTable. 
  // Returns null if VarTable is empty.
  string* VarTable::GetAllVar();
};

#endif  !VAR_TABLE_H