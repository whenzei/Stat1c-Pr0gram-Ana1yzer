#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "pql_enum.h"
#include "pql_result.h"

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

PqlResult::PqlResult() {}

void PqlResult::MergeResults(QueryResultList result_list,
                             PqlResultTableConflict conflict_type,
                             int conflict_column_num) {
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
  }
  // This column already exist, merge with comparison
  else {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& result_column : result_list) {
        // If the column value in table matches the value in result list
        if (row[conflict_column_num].compare(result_column) == 0) {
          // Row has passed the comparison
          new_table.push_back(row);
          break;
        }
      }
    }
    SetResultTable(new_table);
  }
}

void PqlResult::MergeResults(QueryResultPairList result_pair_list,
                             PqlResultTableConflict conflict_type,
                             int conflict_left_pair, int conflict_right_pair) {
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
  }
  // Left has conflict
  else if (conflict_type == kOneConflictLeft) {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& result_column_pair : result_pair_list) {
        // If the column value in table matches the value in result list
        if (row[conflict_left_pair].compare(result_column_pair.first) == 0) {
          // Row has passed the comparison
          // Merging
          ResultRow new_row = row;
          new_row.push_back(result_column_pair.second);
          new_table.push_back(new_row);
        }
      }
    }
    SetResultTable(new_table);
  }
  // Right has conflict
  else if (conflict_type == kOneConflictRight) {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& result_column_pair : result_pair_list) {
        // If the column value in table matches the value in result list
        if (row[conflict_right_pair].compare(result_column_pair.second) == 0) {
          // Row has passed the comparison
          // Merging
          ResultRow new_row = row;
          new_row.push_back(result_column_pair.first);
          new_table.push_back(new_row);
        }
      }
    }
    SetResultTable(new_table);
  }
  // Two conflict
  else if (conflict_type == kTwoConflict) {
    // Iterating through the row
    for (auto& row : result_table) {
      // Iterating through the result list to be merged
      for (auto& result_column_pair : result_pair_list) {
        // If the column value in table matches the value in result list
        if ((row[conflict_left_pair].compare(result_column_pair.first) == 0) &&
            (row[conflict_right_pair].compare(result_column_pair.second) ==
             0)) {
          // Row has passed the comparison
          new_table.push_back(row);
          break;
        }
      }
    }
    SetResultTable(new_table);
  }
}

void PqlResult::InitTable(QueryResultList result_list) {
  for (auto& iter : result_list) {
    ResultRow new_row;
    // Add one new column
    new_row.push_back(iter);
    // Add row to result table
    this->result_table_.push_back(new_row);
  }
}

void PqlResult::InitTable(QueryResultPairList result_pair_list) {
  for (auto& iter : result_pair_list) {
    ResultRow new_row;
    // Add two new column
    new_row.push_back(iter.first);
    new_row.push_back(iter.second);
    // Add row to result table
    this->result_table_.push_back(new_row);
  }
}

/* Getter and Setter */

void PqlResult::AddColumnHeader(string var_name, int column_num) {
  column_header_.insert(std::make_pair(var_name, column_num));
}

void PqlResult::SetResultTable(ResultTable result_table) {
  this->result_table_ = result_table;
}

void PqlResult::SetColumnCount(int column_count) {
  this->column_count_ = column_count;
}

ColumnHeader PqlResult::GetColumnHeader() { return column_header_; }

ResultTable PqlResult::GetResultTable() { return result_table_; }

int PqlResult::GetColumnCount() { return column_count_; }
