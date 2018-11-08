#pragma once

#ifndef SPA_VAR_LIST_H
#define SPA_VAR_LIST_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "pql_global.h"

using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;

using VarName = string;
using VarIndex = int;
using VarIndexSet = unordered_set<VarIndex>;
using VarIndexPairSet = unordered_set<pair<VarIndex, VarIndex>>;
using VarNameSet = unordered_set<VarName>;
using VarIndexMap = unordered_map<VarName, VarIndex>;
using IndexVarMap = unordered_map<VarIndex, VarName>;
using StmtNum = int;
using StmtNumSet = unordered_set<StmtNum>;
using StmtVarMap = unordered_map<StmtNum, VarIndex>;
using VarStmtMap = unordered_map<VarIndex, unordered_set<StmtNum>>;

// The variable list class for the PKB component
// Used to store variable names that are passed into PKB from the parser
class VarList {
  VarIndexSet var_index_set_;
  VarIndexPairSet var_index_twin_set_;
  VarIndexMap var_index_map_;
  IndexVarMap index_var_map_;
  StmtVarMap read_var_map_;
  VarStmtMap var_read_map_;
  VarIndexPairSet read_var_twin_set_;
  VarIndexSet read_var_set_;
  StmtVarMap print_var_map_;
  VarStmtMap var_print_map_;
  VarIndexSet print_var_set_;
  VarIndexPairSet print_var_twin_set_;

 private:
  // Internal variable to assign new variables' indices
  int var_count_ = 0;

 public:
  // Returns a list of all variable names in the VarList.
  VarIndexSet GetAllVarIndices();

  // Returns a list of all variable names in pairs (in each pair, the same
  // var_name is repeated)
  VarIndexPairSet GetAllVarIndexTwin();

  // Inserts var_name into VarList
  // @return index of the inserted variable if successful
  // @return -1 if variable name already exists in the variable list
  int InsertVarName(VarName var_name);

  void InsertVarName(VarName var_name, PqlDeclarationEntity stmt_type, StmtNum stmt_num);

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

  // @returns the variable modified in stmt_num if it is a read stmt
  VarIndex GetReadVar(StmtNum stmt_num);

  // @returns the variable used in stmt_num if it is a print stmt
  VarIndex GetPrintVar(StmtNum stmt_num);

  // @returns a list of all read stmts that modifies the given variable
  StmtNumSet GetReadStmt(VarIndex var_id);

  // @returns a list of all print stmts that uses the given variable
  StmtNumSet GetPrintStmt(VarIndex var_id);

  // @returns true if var_name is modified in any read stmt
  bool IsReadVar(VarIndex var_id);

  // @returns true if var_name is used in any print stmt
  bool IsPrintVar(VarIndex var_id);

  // @returns a list of all variables modified in some read stmt
  VarIndexSet GetAllReadVar();

  // @returns a list of all variables used in some print stmt
  VarIndexSet GetAllPrintVar();

  // @returns a list of all variables modified in some read stmt (repeat each
  // var_id to form a pair)
  VarIndexPairSet GetAllReadVarTwin();

  // @returns a list of all variables used in some print stmt (repeat each
  // var_id to form a pair)
  VarIndexPairSet GetAllPrintVarTwin();
};

#endif !SPA_VAR_LIST_H