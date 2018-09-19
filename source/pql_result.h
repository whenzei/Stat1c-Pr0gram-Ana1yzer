#pragma once

#ifndef PQL_RESULT_H
#define PQL_RESULT_H

#include <string>
#include <unordered_map>
#include <vector>
using std::string;
using std::unordered_map;
using std::vector;

#include "pql_enum.h"

using ColumnHeader = unordered_map<string, int>;
using ResultTable = vector<vector<string>>;

/*
This class stores the PQL result as a table
*/
class PqlResult {
 private:
  /* a map that maps the column header name to its column number in the table*/
  ColumnHeader column_header_;
  /* the table which contains all the results from user query*/
  ResultTable result_table_;
  int column_count_;

 public:
  /* Constructor */
  PqlResult();

  /* Setters */
  void AddColumnHeader(string var_name, int column_num);
  void SetResultTable(ResultTable);
  void SetColumnCount(int);

  /* Getters */
  ColumnHeader GetColumnHeader();
  ResultTable GetResultTable();
  int GetColumnCount();
};

#endif  // !PQL_RESULT_H
