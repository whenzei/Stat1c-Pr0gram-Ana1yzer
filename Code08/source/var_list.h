#pragma once

#ifndef SPA_VAR_LIST_H
#define SPA_VAR_LIST_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using VarName = string;
using VarNameList = vector<string>;
using VarNameSet = unordered_set<string>;
using VarNamePairList = vector<pair<string, string>>;
using VarIndexMap = unordered_map<string, int>;
using IndexVarMap = unordered_map<int, string>;

// The variable list class for the PKB component
// Used to store variable names that are passed into PKB from the parser
class VarList {
  VarNameList var_name_list_;
  VarNameSet var_name_set_;
  VarNamePairList var_name_twin_list_;
  // TODO: Depending on PQL's preferences, might have to add below
  // a list of Proc Indices and just return the entire list.
  VarIndexMap var_index_map_;
  IndexVarMap index_var_map_;

 private: 
  // Temporary
  int var_count = 0;

 public:
  // Returns a list of all variable names in the VarList.
  VarNameList GetAllVarName();

  // Returns a list of all variable names in pairs (in each pair, the same var_name is repeated)
  VarNamePairList GetAllVarNameTwin();

  // Inserts var_name into VarList
  void InsertVarName(VarName var_name);

  // @return true if var_name is in the var list
  bool IsVarName(VarName var_name);

  // @returns the integer index mapped to the Variable name
  // @returns -1 if there is no such variable name in the list
  int GetIndexForVar(VarName var_name);

  // @returns the Variable name index mapped to the index
  VarName GetVarForIndex(int index);
};

#endif !SPA_VAR_LIST_H