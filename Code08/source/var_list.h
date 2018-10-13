#pragma once

#ifndef SPA_VAR_LIST_H
#define SPA_VAR_LIST_H

#include <string>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_set;
using std::vector;

using VarName = string;
using VarNameList = vector<string>;
using VarNameSet = unordered_set<string>;
using VarNamePairList = vector<pair<string, string>>;

// The variable list class for the PKB component
// Used to store variable names that are passed into PKB from the parser
class VarList {
  VarNameList var_name_list_;
  VarNameSet var_name_set_;
  VarNamePairList var_name_twin_list_;

 public:
  // Returns a list of all variable names in the VarList.
  VarNameList GetAllVarName();

  // Returns a list of all variable names in pairs (in each pair, the same var_name is repeated)
  VarNamePairList GetAllVarNameTwin();

  // Inserts var_name into VarList
  void InsertVarName(VarName var_name);

  // @return true if var_name is in the var list
  bool IsVarName(VarName var_name);
};

#endif !SPA_VAR_LIST_H