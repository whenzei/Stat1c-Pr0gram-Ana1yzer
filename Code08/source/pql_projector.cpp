#include "pql_projector.h"

void PqlProjector::SortSelections() {
  for (auto& syn : selections_set_) {
    int group = intermediate_column_header_[syn].first;
    selection_group_table_[group].push_back(syn);
    selected_groups_.insert(group);
  }
}

void PqlProjector::TrimIntermediateResultTables() {
  for (int group : selected_groups_) {
    ResultTable* result_table = &intermediate_result_tables_[group];
    vector<VarName> selected_synonyms = selection_group_table_[group];
	// trim result table if not all columns are selected
    if (selected_synonyms.size() != (*result_table).front().size()) {
      vector<int> selected_columns;
      for (VarName& syn : selected_synonyms) {
        selected_columns.push_back(intermediate_column_header_[syn].second);
      }
      for (auto& row : (*result_table)) {
        ResultRow new_row;
        // only keep columns that are selected
        for (int column : selected_columns) {
          new_row.push_back(row[column]);
        }
        row = new_row;
      }
    } else {
      // if all columns are selected, changed the order of synonyms in
      // selection_group_table_[group] to follow the column order in result
      // table
      for (VarName& syn : selected_synonyms) {
        int column = intermediate_column_header_[syn].second;
        selection_group_table_[group][column] = syn;
      }
    }
  }
}

void PqlProjector::MergeIntermediateResultTables() {
  while (!selected_groups_.empty()) {
    int group = *selected_groups_.begin();
    ResultTable* result_table = &intermediate_result_tables_[group];
    if (final_result_table_.empty()) {
      final_result_table_ = *result_table;
    } else {
      // cross product
      ResultTable new_table;
      for (auto& row_1 : final_result_table_) {
        for (auto& row_2 : *result_table) {
          ResultRow new_row = row_1;
          new_row.insert(new_row.end(), row_2.begin(), row_2.end());
          new_table.push_back(new_row);
        }
      }
      final_result_table_ = new_table;
    }
    // add column header
    for (VarName& syn : selection_group_table_[group]) {
      final_column_header_[syn] = column_count_;
      column_count_++;
    }
    selected_groups_.erase(group);
  }
}

void PqlProjector::GenerateFinalResult() {
  for (auto& row : final_result_table_) {
    string result;
    for (Synonym& syn : selections_list_) {
      int column = final_column_header_[syn.first];
      switch (syn.second) {
        case PqlDeclarationEntity::kCallName:
        case PqlDeclarationEntity::kProcedure:
          result += GetProcName(row[column]) + " ";
          break;
        case PqlDeclarationEntity::kVariable:
          result += GetVarName(row[column]) + " ";
          break;
        default:
          result += to_string(row[column]) + " ";
          break;
      }
    }
    final_result_.push_back(result.substr(0, result.size() - 1));
  }
}

ResultTable PqlProjector::GetSelectAllResult(Synonym selected_syn) {
  QueryResultList query_result_list;
  PqlResult pql_result;

  switch (selected_syn.second) {
    case PqlDeclarationEntity::kProcedure:
      // Get all procedures name from PKB and store into
      // query_result_list
      query_result_list = pkb_.GetAllProcIndices();
      break;
    case PqlDeclarationEntity::kVariable:
      // Get all variable name from PKB and store into
      // query_result_list 
      query_result_list = pkb_.GetAllVarIndices();
      break;
    case PqlDeclarationEntity::kAssign:
      // Get all statement number of statement which
      // contains assignment from PKB and store into query_result_list
      query_result_list = pkb_.GetAllAssignStmt();
      break;
    case PqlDeclarationEntity::kStmt:
      // Get all stmt number from PKB and store into query_result_list
      query_result_list = pkb_.GetAllStmt();
      break;
    case PqlDeclarationEntity::kRead:
      // Get all read stmt from PKB and store into query_result_list
      query_result_list = pkb_.GetAllReadStmt();
      break;
    case PqlDeclarationEntity::kPrint:
      // Get all print stmt from PKB and store into query_result_list
      query_result_list = pkb_.GetAllPrintStmt();
      break;
    case PqlDeclarationEntity::kCall:
      // Get all call stmt from PKB and store into query_result_list
      query_result_list = pkb_.GetAllCallStmt();
      break;
    case PqlDeclarationEntity::kCallName:
      // Get all call stmt from PKB and store into query_result_list
      query_result_list = pkb_.GetAllCallee();
      break;
    case PqlDeclarationEntity::kWhile:
      // Get all while stmt from PKB and store into query_result_list
      query_result_list = pkb_.GetAllWhileStmt();
      break;
    case PqlDeclarationEntity::kIf:
      // Get all if stmt from PKB and store into query_result_list
      query_result_list = pkb_.GetAllIfStmt();
      break;
    case PqlDeclarationEntity::kConstant:
      // Get all constant from PKB and store into query_result_list
      query_result_list = pkb_.GetAllConstValue();
      break;
    case PqlDeclarationEntity::kProgline:
      // Get all program line from PKB and store into
      // query_result_list
      query_result_list = pkb_.GetAllStmt();
      break;
  }

  pql_result.InitTable(query_result_list, selected_syn.first);
  return pql_result.GetResultTable();
}

VarName PqlProjector::GetVarName(VarIndex var_id) { return index_to_var_map_[var_id]; }

ProcName PqlProjector::GetProcName(ProcIndex proc_id) { return index_to_proc_map_[proc_id]; }

FinalResult PqlProjector::GetFinalResult(
    ResultTableList intermediate_result_tables,
    ResultTableColumnHeader intermediate_column_header, SynonymList selections,
    PKB pkb) {
  intermediate_result_tables_ = intermediate_result_tables;
  intermediate_column_header_ = intermediate_column_header;
  selections_list_ = selections;
  pkb_ = pkb;
  column_count_ = 0;
  index_to_var_map_ = pkb_.GetIndexToVarMapping();
  index_to_proc_map_ = pkb_.GetIndexToProcMapping();

  for (Synonym& syn : selections_list_) {
    // use a set to eliminate duplicates
    if (selections_set_.insert(syn.first).second) {
      // if the selected synonym is not in any group, store the result of
      // GetAll[synonym_type] as a new group
      if (intermediate_column_header_.count(syn.first) == 0) {
        intermediate_result_tables_.push_back(GetSelectAllResult(syn));
        intermediate_column_header_[syn.first] =
            make_pair(intermediate_result_tables_.size() - 1, 0);
      }
    }
  }

  SortSelections();
  TrimIntermediateResultTables();
  MergeIntermediateResultTables();
  GenerateFinalResult();

  return final_result_;
}

FinalResult PqlProjector::GetFinalResult(bool result) {
  result ? final_result_.push_back("true") : final_result_.push_back("false");
  return final_result_;
}
