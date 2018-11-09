#pragma once

#ifndef PQL_PROJECTOR_H
#define PQL_PROJECTOR_H

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_query.h"
#include "pql_result.h"

using std::to_string;

using ResultTableList = vector<ResultTable>;
// first int represents the result table, second int represents the column
using ResultTableColumnHeader = unordered_map<string, pair<int, int>>;
using SelectionGroupTable = unordered_map<int, vector<VarName>>;
using SynonymList = vector<Synonym>;

// This class generates the final result from intermediate result tables
class PqlProjector {
 private:
  // one result table for each group
  ResultTableList intermediate_result_tables_;
  ResultTableColumnHeader intermediate_column_header_;
  SynonymList selections_list_;
  // eliminate duplicates
  VarNameSet selections_set_;
  PKB* pkb_;
  SelectionGroupTable selection_group_table_;
  ResultTable final_result_table_;
  ColumnHeader final_column_header_;
  FinalResult final_result_;
  unordered_set<int> selected_groups_;
  int column_count_;
  IndexVarMap index_to_var_map_;
  IndexProcMap index_to_proc_map_;
 
  // sort selected synonyms into groups (according to which intermediate result table they belong to)
  void SortSelections();

  // keep only the columns that are selected, remove unselected columns
  ResultTable* TrimIntermediateResultTable(int group);

  // store the cross product of intermediate result tables as final_result_table
  void MergeIntermediateResultTables();

  // generate final_result from final_result_table
  void GenerateFinalResult();

  // @returns true if the result for select all select_type is not empty and is
  // added to intermediate_result_tables_ used when a selected synonym does not
  // appear in any clause
  bool AddSelectAllResult(Synonym selected_syn);

  // @returns the corresponding variable name
  VarName GetVarName(VarIndex var_id);

  // @returns the corresponding procedure name
  ProcName GetProcName(ProcIndex proc_id);

 public:
  // @returns the final result of the query
  FinalResult GetFinalResult(ResultTableList intermediate_result_tables,
                             ResultTableColumnHeader intermediate_column_header,
                             SynonymList selections, PKB* pkb, bool bool_result_so_far);
};

#endif  // !PQL_PROJECTOR_H