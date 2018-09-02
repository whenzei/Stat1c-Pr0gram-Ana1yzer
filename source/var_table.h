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
  
  // Takes in the variable name and inserts a new variable in the VarTable.
  // Increments variable count. 
  // Returns the index of the variable just added.
  int InsertVar(string var_name);

  // Returns pointer to an array of all var_name in the VarTable. 
  // Returns null if VarTable is empty.
  string* VarTable::GetAllVar();
};

#endif  !VAR_TABLE_H