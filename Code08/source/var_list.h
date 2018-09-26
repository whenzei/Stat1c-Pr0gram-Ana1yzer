#pragma once

#ifndef SPA_VAR_LIST_H
#define SPA_VAR_LIST_H

#include <string>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_set;
using std::vector;

using VarName = string;
using VarNameList = vector<string>;
using VarNameSet = unordered_set<string>;


// The variable list class for the PKB component
// Used to store variable names that are passed into PKB from the parser
class VarList {

  VarNameList var_name_list_;
  VarNameSet var_name_set_;

 public:
  // Returns a list of all variable names in the VarList.
  VarNameList GetAllVarName();

  // Inserts var_name into VarList
  void InsertVarName(VarName var_name);
};

#endif !SPA_VAR_LIST_H