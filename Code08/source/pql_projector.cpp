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
          result += pkb_.GetProcName(row[column]) + " ";
          break;
        case PqlDeclarationEntity::kProcedure:
          result += pkb_.GetProcName(row[column]) + " ";
          break;
        case PqlDeclarationEntity::kVariable:
          result += pkb_.GetVarName(row[column]) + " ";
          break;
        default:
          result += to_string(row[column]) + " ";
          break;
      }
    }
    final_result_.push_back(result.substr(0, result.size() - 1));
  }
}

FinalResult PqlProjector::GetFinalResult(PqlResultList intermediate_results,
                                         SynonymList selections, PKB pkb) {
  selections_list_ = selections;
  pkb_ = pkb;
  column_count_ = 0;

  for (int i = 0; i < intermediate_results.size(); i++) {
    intermediate_result_tables_.push_back(
        intermediate_results[i].GetResultTable());
    ColumnHeader column_header = intermediate_results[i].GetColumnHeader();
    for (auto& entry : column_header) {
      intermediate_column_header_[entry.first] = make_pair(i, entry.second);
    }
  }

  for (Synonym& syn : selections_list_) {
    selections_set_.insert(syn.first);
  }
  
  SortSelections();
  TrimIntermediateResultTables();
  MergeIntermediateResultTables();
  GenerateFinalResult();

  return final_result_;
}
