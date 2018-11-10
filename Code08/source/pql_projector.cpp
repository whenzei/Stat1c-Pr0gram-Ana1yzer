#include "pql_projector.h"

using std::find;

void PqlProjector::SortSelections() {
  for (auto& syn : selections_set_) {
    int group = intermediate_column_header_[syn].first;
    selection_group_table_[group].push_back(syn);
    selected_groups_.insert(group);
  }
}

ResultTable* PqlProjector::TrimIntermediateResultTable(int group) {
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
  return result_table;
}

void PqlProjector::MergeIntermediateResultTables() {
  while (!selected_groups_.empty()) {
    int group = *selected_groups_.begin();
    ResultTable* result_table = &intermediate_result_tables_[group];
    if (final_result_table_.empty()) {
      final_result_table_ = *TrimIntermediateResultTable(group);
    } else {
      // cross product
      ResultTable new_table;
      for (auto& row_1 : final_result_table_) {
        for (auto& row_2 : *result_table) {
          ResultRow new_row = row_1;
          for (string& selected_synonym : selection_group_table_[group]) {
            // only merge the selected columns
            int selected_column =
                intermediate_column_header_[selected_synonym].second;
            new_row.push_back(row_2[selected_column]);
          }
          new_table.push_back(new_row);
        }
      }

      // clean duplicates, take P(nlogn) but may be worth it
      std::sort(new_table.begin(), new_table.end());
      new_table.erase(std::unique(new_table.begin(), new_table.end()), new_table.end());
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
        case PqlDeclarationEntity::kReadName:
        case PqlDeclarationEntity::kPrintName:
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

bool PqlProjector::AddSelectAllResult(Synonym selected_syn) {
  QueryResultList query_result_list;
  PqlResult pql_result;
  bool intermediate_result_tables_modified = false;
  int c = 0;

  switch (selected_syn.second) {
    case PqlDeclarationEntity::kProcedure:
      // Get all procedures name from PKB and store into
      // query_result_list
      query_result_list = pkb_->GetAllProcIndices();
      break;
    case PqlDeclarationEntity::kVariable:
      // Get all variable name from PKB and store into
      // query_result_list
      query_result_list = pkb_->GetAllVarIndices();
      break;
    case PqlDeclarationEntity::kAssign:
      // Get all statement number of statement which
      // contains assignment from PKB and store into query_result_list
      query_result_list = pkb_->GetAllAssignStmt();
      break;
    case PqlDeclarationEntity::kStmt:
      // Get all stmt number from PKB and store into query_result_list
      query_result_list = pkb_->GetAllStmt();
      break;
    case PqlDeclarationEntity::kRead:
      // Get all read stmt from PKB and store into query_result_list
      query_result_list = pkb_->GetAllReadStmt();
      break;
    case PqlDeclarationEntity::kPrint:
      // Get all print stmt from PKB and store into query_result_list
      query_result_list = pkb_->GetAllPrintStmt();
      break;
    case PqlDeclarationEntity::kCall:
      // Get all call stmt from PKB and store into query_result_list
      query_result_list = pkb_->GetAllCallStmt();
      break;
    case PqlDeclarationEntity::kCallName: {
      string call_stmt = selected_syn.first.substr(1);
      Synonym call_syn = make_pair(call_stmt, PqlDeclarationEntity::kCall);
      if (find(selections_list_.begin(), selections_list_.end(), call_syn) !=
          selections_list_.end()) {
        // if call_stmt is also selected, make sure there is a table containing
        // call_stmt first, then add a column of call_var
        if (!intermediate_column_header_.count(call_stmt)) {
          // add an intermediate result table for the corresponding call_stmt if
          // it does not exist in any intermediate result table
          if (!AddSelectAllResult(call_syn)) {
            return false;
          }
        }
        // get the corresponding call.procName for each row and add them as a
        // column
        int result_table_id = intermediate_column_header_[call_stmt].first;
        int column_id = intermediate_column_header_[call_stmt].second;
        ResultTable* result_table =
            &intermediate_result_tables_[result_table_id];
        for (auto& row : *result_table) {
          row.push_back(pkb_->GetCalledProcedure(row[column_id]));
        }
        intermediate_column_header_[selected_syn.first] =
            make_pair(result_table_id, (*result_table).front().size() - 1);
        intermediate_result_tables_modified = true;
      } else {
        query_result_list = pkb_->GetAllCallee();
      }
      break;
    }
    case PqlDeclarationEntity::kWhile:
      // Get all while stmt from PKB and store into query_result_list
      query_result_list = pkb_->GetAllWhileStmt();
      break;
    case PqlDeclarationEntity::kIf:
      // Get all if stmt from PKB and store into query_result_list
      query_result_list = pkb_->GetAllIfStmt();
      break;
    case PqlDeclarationEntity::kConstant:
      // Get all constant from PKB and store into query_result_list
      query_result_list = pkb_->GetAllConstValue();
      break;
    case PqlDeclarationEntity::kProgline:
      // Get all program line from PKB and store into
      // query_result_list
      query_result_list = pkb_->GetAllStmt();
      break;
    case PqlDeclarationEntity::kReadName: {
      string read_stmt = selected_syn.first.substr(1);
      Synonym read_syn = make_pair(read_stmt, PqlDeclarationEntity::kRead);
      if (find(selections_list_.begin(), selections_list_.end(), read_syn) !=
          selections_list_.end()) {
        // if read_stmt is also selected, make sure there is a table containing
        // read_stmt first, then add a column of read_var
        if (!intermediate_column_header_.count(read_stmt)) {
          // add an intermediate result table for the corresponding read_stmt if
          // it does not exist in any intermediate result table
          if (!AddSelectAllResult(read_syn)) {
            return false;
          }
        }
        // get the corresponding read.varName for each row and add them as a
        // column
        int result_table_id = intermediate_column_header_[read_stmt].first;
        int column_id = intermediate_column_header_[read_stmt].second;
        ResultTable* result_table =
            &intermediate_result_tables_[result_table_id];
        for (auto& row : *result_table) {
          row.push_back(pkb_->GetReadVar(row[column_id]));
        }
        intermediate_column_header_[selected_syn.first] =
            make_pair(result_table_id, (*result_table).front().size() - 1);
        intermediate_result_tables_modified = true;
      } else {
        query_result_list = pkb_->GetAllReadVar();
      }
      break;
    }
    case PqlDeclarationEntity::kPrintName: {
      string print_stmt = selected_syn.first.substr(1);
      Synonym print_syn = make_pair(print_stmt, PqlDeclarationEntity::kPrint);
      if (find(selections_list_.begin(), selections_list_.end(), print_syn) !=
          selections_list_.end()) {
        // if print_stmt is also selected, make sure there is a table containing
        // print_stmt first, then add a column of print_var
        if (!intermediate_column_header_.count(print_stmt)) {
          // add an intermediate result table for the corresponding print_stmt
          // if it does not exist in any intermediate result table
          if (!AddSelectAllResult(print_syn)) {
            return false;
          }
        }
        // get the corresponding print.varName for each row and add them as a
        // column
        int result_table_id = intermediate_column_header_[print_stmt].first;
        int column_id = intermediate_column_header_[print_stmt].second;
        ResultTable* result_table =
            &intermediate_result_tables_[result_table_id];
        for (auto& row : *result_table) {
          row.push_back(pkb_->GetPrintVar(row[column_id]));
        }
        intermediate_column_header_[selected_syn.first] =
            make_pair(result_table_id, (*result_table).front().size() - 1);
        intermediate_result_tables_modified = true;
      } else {
        query_result_list = pkb_->GetAllPrintVar();
      }
      break;
    }
  }

  if (intermediate_result_tables_modified) {
    return true;
  } else if (query_result_list.empty()) {
    return false;
  } else {
    pql_result.InitTable(query_result_list, selected_syn.first);
    intermediate_result_tables_.push_back(pql_result.GetResultTable());
    intermediate_column_header_[selected_syn.first] =
        make_pair(intermediate_result_tables_.size() - 1, c);
    return true;
  }
}

VarName PqlProjector::GetVarName(VarIndex var_id) {
  return index_to_var_map_[var_id];
}

ProcName PqlProjector::GetProcName(ProcIndex proc_id) {
  return index_to_proc_map_[proc_id];
}

FinalResult PqlProjector::GetFinalResult(
    ResultTableList intermediate_result_tables,
    ResultTableColumnHeader intermediate_column_header, SynonymList selections,
    PKB* pkb, bool bool_result_so_far) {
  if (!bool_result_so_far) {
    if (selections.empty()) {
      // case 1: select boolean and the query evaluates to false
      final_result_.push_back("FALSE");
    }
    // case 2: select synonym and the query has empty result (return empty final
    // result list)
    return final_result_;
  } else if (selections.empty()) {
    // case 3: select boolean and the query evaluates to true
    final_result_.push_back("TRUE");
    return final_result_;
  }

  // case 4: select synonym and the query has non-empty result

  intermediate_result_tables_ = intermediate_result_tables;
  intermediate_column_header_ = intermediate_column_header;
  selections_list_ = selections;
  pkb_ = pkb;
  column_count_ = 0;
  index_to_var_map_ = pkb_->GetIndexToVarMapping();
  index_to_proc_map_ = pkb_->GetIndexToProcMapping();

  for (Synonym& syn : selections_list_) {
    // use a set to eliminate duplicates
    if (selections_set_.insert(syn.first).second) {
      if (intermediate_column_header_.count(syn.first) == 0) {
        if (!AddSelectAllResult(syn)) {
          // one of the selected synonym does not have any result, return empty
          // final result list
          return final_result_;
        }
      }
    }
  }

  SortSelections();
  MergeIntermediateResultTables();
  GenerateFinalResult();

  return final_result_;
}
