#include <string>
#include <unordered_map>
#include <vector>

#include "debug.h"
#include "pql_global.h"
#include "pql_result.h"

using std::string;
using std::unordered_map;
using std::vector;

PqlResult::PqlResult() { SetColumnCount(0); }

void PqlResult::MergeResults(QueryResultList result_list,
                             PqlResultTableConflict conflict_type,
                             int conflict_column_num, string header_name) {
  ResultTable result_table = GetResultTable();
  ResultTable new_table;

  // Not found, no conflict, proceed to merge
  if (conflict_type == kNoConflict) {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& result_column : result_list) {
        // Merging
        ResultRow new_row = row;
        new_row.push_back(result_column);
        new_table.push_back(new_row);
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_name, col_num);
    SetColumnCount(++col_num);
  }
  // This column already exist, merge with comparison
  else {
    SetupMergeSet(result_list);
    MergeSet merge_set = GetMergeSet();
    // Iterating through the row
    for (auto& row : result_table) {
      // If the column value in table matches the value in result list
      if (merge_set.count(std::to_string(row[conflict_column_num]))) {
        // Row has passed the comparison
        new_table.push_back(row);
      }
    }
    SetResultTable(new_table);
  }

  // If table is empty
  if (new_table.empty()) {
    // Remove all the column header
    SetColumnCount(0);
    ClearColumnHeader();
  }
}

void PqlResult::MergeResults(QueryResultSet result_set,
                             PqlResultTableConflict conflict_type,
                             int conflict_column_num, string header_name) {
  ResultTable result_table = GetResultTable();
  ResultTable new_table;

  // Not found, no conflict, proceed to merge
  if (conflict_type == kNoConflict) {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& result_column : result_set) {
        // Merging
        ResultRow new_row = row;
        new_row.push_back(result_column);
        new_table.push_back(new_row);
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_name, col_num);
    SetColumnCount(++col_num);
  }
  // This column already exist, merge with comparison
  else {
    SetupMergeSet(result_set);
    MergeSet merge_set = GetMergeSet();
    // Iterating through the row
    for (auto& row : result_table) {
      // If the column value in table matches the value in result list
      if (merge_set.count(std::to_string(row[conflict_column_num]))) {
        // Row has passed the comparison
        new_table.push_back(row);
      }
    }
    SetResultTable(new_table);
  }

  // If table is empty
  if (new_table.empty()) {
    // Remove all the column header
    SetColumnCount(0);
    ClearColumnHeader();
  }
}

void PqlResult::MergeResults(QueryResultPairList result_pair_list,
                             PqlResultTableConflict conflict_type,
                             int conflict_left_pair, int conflict_right_pair,
                             string header_left, string header_right) {
  ResultTable result_table = GetResultTable();
  ResultTable new_table;

  // Not found, no conflict, proceed to merge
  if (conflict_type == kNoConflict) {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& result_column_pair : result_pair_list) {
        // Merging
        ResultRow new_row = row;
        new_row.push_back(result_column_pair.first);
        new_row.push_back(result_column_pair.second);
        new_table.push_back(new_row);
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_left, col_num);
    AddColumnHeader(header_right, ++col_num);
    SetColumnCount(++col_num);
  }
  // Left has conflict
  else if (conflict_type == kOneConflictLeft) {
    Debug::PrintLn(Debug::kLog, "Merging result tables: Conflict left");
    SetupMergeMap(result_pair_list, conflict_type);
    MergeMap merge_map = GetMergeMap();
    // Iterating through the row
    for (auto& row : result_table) {
      MergeMap::iterator merge_iter = merge_map.find(row[conflict_left_pair]);
      // If this col value is found in merge map
      if (merge_iter != merge_map.end()) {
        // Row has passed the comparison, Merging
        for (auto& merge_item : merge_iter->second) {
          ResultRow new_row = row;
          new_row.push_back(merge_item);
          new_table.push_back(new_row);
        }
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_right, col_num);
    SetColumnCount(++col_num);
  }
  // Right has conflict
  else if (conflict_type == kOneConflictRight) {
    SetupMergeMap(result_pair_list, conflict_type);
    MergeMap merge_map = GetMergeMap();
    // Iterating through the row
    for (auto& row : result_table) {
      MergeMap::iterator merge_iter = merge_map.find(row[conflict_right_pair]);
      // If this col value is found in merge map
      if (merge_iter != merge_map.end()) {
        // Row has passed the comparison, Merging
        for (auto& merge_item : merge_iter->second) {
          ResultRow new_row = row;
          new_row.push_back(merge_item);
          new_table.push_back(new_row);
        }
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_left, col_num);
    SetColumnCount(++col_num);
  }
  // Two conflict
  else if (conflict_type == kTwoConflict) {
    SetupMergeSet(result_pair_list);
    MergeSet merge_set = GetMergeSet();
    // Iterating through the row
    for (auto& row : result_table) {
      string unique_key = std::to_string(row[conflict_left_pair]) + "-" +
                          std::to_string(row[conflict_right_pair]);

      Debug::PrintLn(Debug::kLog,
                     "Merging result tables, unique key: " + unique_key);
      // If the column value in table matches the value in result list
      if (merge_set.count(unique_key)) {
        // Row has passed the comparison
        new_table.push_back(row);
      }
    }
    SetResultTable(new_table);
  }

  // If table is empty
  if (new_table.empty()) {
    // Remove all the column header
    SetColumnCount(0);
    ClearColumnHeader();
  }
}

void PqlResult::MergeResults(AffectsMap affects_table,
                             PqlResultTableConflict conflict_type,
                             int conflict_left_pair, int conflict_right_pair,
                             string header_left, string header_right) {
  ResultTable result_table = GetResultTable();
  ResultTable new_table;

  // Not found, no conflict, proceed to merge
  if (conflict_type == kNoConflict) {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& affect : affects_table) {
        for (auto& affected : affect.second) {
          // Merging
          ResultRow new_row = row;
          new_row.push_back(affect.first);
          new_row.push_back(affected);
          new_table.push_back(new_row);
        }
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_left, col_num);
    AddColumnHeader(header_right, ++col_num);
    SetColumnCount(++col_num);
  }
  // Left has conflict
  else if (conflict_type == kOneConflictLeft) {
    SetupMergeMap(affects_table, conflict_type);
    MergeMap merge_map = GetMergeMap();
    // Iterating through the row
    for (auto& row : result_table) {
      MergeMap::iterator merge_iter = merge_map.find(row[conflict_left_pair]);
      // If this col value is found in merge map
      if (merge_iter != merge_map.end()) {
        // Row has passed the comparison, Merging
        for (auto& merge_item : merge_iter->second) {
          ResultRow new_row = row;
          new_row.push_back(merge_item);
          new_table.push_back(new_row);
        }
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_right, col_num);
    SetColumnCount(++col_num);
  }
  // Right has conflict
  else if (conflict_type == kOneConflictRight) {
    SetupMergeMap(affects_table, conflict_type);
    MergeMap merge_map = GetMergeMap();
    // Iterating through the row
    for (auto& row : result_table) {
      MergeMap::iterator merge_iter = merge_map.find(row[conflict_right_pair]);
      // If this col value is found in merge map
      if (merge_iter != merge_map.end()) {
        // Row has passed the comparison, Merging
        for (auto& merge_item : merge_iter->second) {
          ResultRow new_row = row;
          new_row.push_back(merge_item);
          new_table.push_back(new_row);
        }
      }
    }
    SetResultTable(new_table);
    // Manage column header
    int col_num = GetColumnCount();
    AddColumnHeader(header_left, col_num);
    SetColumnCount(++col_num);
  }
  // Two conflict
  else if (conflict_type == kTwoConflict) {
    SetupMergeSet(affects_table);
    MergeSet merge_set = GetMergeSet();
    // Iterating through the row
    for (auto& row : result_table) {
      string unique_key = std::to_string(row[conflict_left_pair]) + "-" +
                          std::to_string(row[conflict_right_pair]);

      // If the column value in table matches the value in result list
      if (merge_set.count(unique_key)) {
        // Row has passed the comparison
        new_table.push_back(row);
      }
    }
    SetResultTable(new_table);
  }

  // If table is empty
  if (new_table.empty()) {
    // Remove all the column header
    SetColumnCount(0);
    ClearColumnHeader();
  }
}

void PqlResult::InitTable(QueryResultList result_list, string header_name) {
  for (auto& iter : result_list) {
    ResultRow new_row;
    // Add one new column
    new_row.push_back(iter);
    // Add row to result table
    this->result_table_.push_back(new_row);
  }
  // Manage column header
  int col_num = GetColumnCount();
  AddColumnHeader(header_name, col_num);
  SetColumnCount(++col_num);
}

void PqlResult::InitTable(QueryResultSet result_set, string header_name) {
  for (auto& iter : result_set) {
    ResultRow new_row;
    // Add one new column
    new_row.push_back(iter);
    // Add row to result table
    this->result_table_.push_back(new_row);
  }
  // Manage column header
  int col_num = GetColumnCount();
  AddColumnHeader(header_name, col_num);
  SetColumnCount(++col_num);
}

void PqlResult::InitTable(QueryResultPairList result_pair_list,
                          string header_left, string header_right) {
  for (auto& iter : result_pair_list) {
    ResultRow new_row;
    // Add two new column
    new_row.push_back(iter.first);
    new_row.push_back(iter.second);
    // Add row to result table
    this->result_table_.push_back(new_row);
  }
  // Manage column header
  int col_num = GetColumnCount();
  AddColumnHeader(header_left, col_num);
  AddColumnHeader(header_right, ++col_num);
  SetColumnCount(++col_num);
}

void PqlResult::InitTable(AffectsMap affects_table, string header_left,
                          string header_right) {
  for (auto& affect : affects_table) {
    for (auto& affected : affect.second) {
      ResultRow new_row;
      // Add two new column
      new_row.push_back(affect.first);
      new_row.push_back(affected);
      // Add row to result table
      this->result_table_.push_back(new_row);
    }
  }
  // Manage column header
  int col_num = GetColumnCount();
  AddColumnHeader(header_left, col_num);
  AddColumnHeader(header_right, ++col_num);
  SetColumnCount(++col_num);
}

void PqlResult::SetupMergeSet(QueryResultList result_list) {
  ClearMergeSet();

  for (auto& iter : result_list) {
    AddMergeSet(std::to_string(iter));
  }
}

void PqlResult::SetupMergeSet(QueryResultSet result_set) {
  ClearMergeSet();

  for (auto& iter : result_set) {
    AddMergeSet(std::to_string(iter));
  }
}

void PqlResult::SetupMergeSet(QueryResultPairList result_pair_list) {
  ClearMergeSet();

  for (auto& iter : result_pair_list) {
    AddMergeSet(std::to_string(iter.first) + "-" + std::to_string(iter.second));
  }
}

void PqlResult::SetupMergeSet(AffectsMap affects_table) {
  ClearMergeSet();

  for (auto& affect : affects_table) {
    for (auto& affected : affect.second) {
      AddMergeSet(std::to_string(affect.first) + "-" +
                  std::to_string(affected));
    }
  }
}

void PqlResult::SetupMergeMap(QueryResultPairList result_pair_list,
                              PqlResultTableConflict conflict_type) {
  ClearMergeMap();

  switch (conflict_type) {
    case kOneConflictLeft:
      for (auto& iter : result_pair_list) {
        AddMergeMap(iter.first, iter.second);
      }
      return;
    case kOneConflictRight:
      for (auto& iter : result_pair_list) {
        AddMergeMap(iter.second, iter.first);
      }
      return;
  }
}

void PqlResult::SetupMergeMap(AffectsMap affects_table,
                              PqlResultTableConflict conflict_type) {
  ClearMergeMap();

  switch (conflict_type) {
    case kOneConflictLeft:
      for (auto& affect : affects_table) {
        for (auto& affected : affect.second) {
          AddMergeMap(affect.first, affected);
        }
      }
      return;
    case kOneConflictRight:
      for (auto& affect : affects_table) {
        for (auto& affected : affect.second) {
          AddMergeMap(affected, affect.first);
        }
      }
      return;
  }
}

/* Getter and Setter */

void PqlResult::ClearColumnHeader() { this->column_header_.clear(); }

void PqlResult::AddColumnHeader(string var_name, int column_num) {
  this->column_header_.insert(std::make_pair(var_name, column_num));
}

void PqlResult::SetResultTable(ResultTable result_table) {
  this->result_table_ = result_table;
}

void PqlResult::SetColumnCount(int column_count) {
  this->column_count_ = column_count;
}

void PqlResult::AddMergeSet(string key) { this->merge_set_.insert(key); }

void PqlResult::AddMergeMap(int key, int value) {
  MergeMap::iterator iter = merge_map_.find(key);

  if (iter != merge_map_.end()) {
    iter->second.push_back(value);
  } else {
    // Create new list
    vector<int> merge_map_list;
    merge_map_list.push_back(value);
    this->merge_map_.insert(std::make_pair(key, merge_map_list));
  }
}

void PqlResult::ClearMergeMap() { this->merge_map_.clear(); }

void PqlResult::ClearMergeSet() { this->merge_set_.clear(); }

ColumnHeader PqlResult::GetColumnHeader() { return column_header_; }

ResultTable PqlResult::GetResultTable() { return result_table_; }

int PqlResult::GetColumnCount() { return column_count_; }

MergeMap PqlResult::GetMergeMap() { return merge_map_; }

MergeSet PqlResult::GetMergeSet() { return merge_set_; }