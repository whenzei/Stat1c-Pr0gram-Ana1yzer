#pragma once

#ifndef SPA_VAR_LIST_H
#define SPA_VAR_LIST_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using VarName = string;
using VarIndex = int;
using VarIndexList = vector<VarIndex>;
using VarIndexPairList = vector<pair<VarIndex, VarIndex>>;
using VarNameSet = unordered_set<VarName>;
using VarIndexMap = unordered_map<VarName, VarIndex>;
using IndexVarMap = unordered_map<VarIndex, VarName>;

// The variable list class for the PKB component
// Used to store variable names that are passed into PKB from the parser
class VarList {
  VarIndexList var_index_list_;
  VarIndexPairList var_index_twin_list_;
  // TODO: Depending on PQL's preferences, might have to add below
  // a list of Proc Indices and just return the entire list.
  VarIndexMap var_index_map_;
  IndexVarMap index_var_map_;

 private:
  // Internal variable to assign new variables' indices
  int var_count_ = 0;

 public:
  // Returns a list of all variable names in the VarList.
  VarIndexList GetAllVarIndices();

  // Returns a list of all variable names in pairs (in each pair, the same
  // var_name is repeated)
  VarIndexPairList GetAllVarIndexTwin();

  // Inserts var_name into VarList
  // @return index of the inserted variable if successful
  // @return -1 if variable name already exists in the variable list
  int InsertVarName(VarName var_name);

  // @return true if var_id is in the var list
  bool IsVarIndex(VarIndex var_id);

  // @return true if var_name is in the var list
  bool IsVarName(VarName var_name);

  // @returns an unordered map with Index to Var mapping
  IndexVarMap GetIndexToVarMapping();

  // @returns an unordered map with Var to Index mapping
  VarIndexMap GetVarToIndexMapping();

  // @returns the corresponding var name
  VarName GetVarName(VarIndex index);

  // @returns the corresponding var name index
  VarIndex GetVarIndex(VarName var_name);
};

#endif !SPA_VAR_LIST_H