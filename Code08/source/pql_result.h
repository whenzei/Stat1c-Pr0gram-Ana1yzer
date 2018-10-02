#pragma once

#ifndef PQL_RESULT_H
#define PQL_RESULT_H

#include <string>
#include <unordered_map>
#include <vector>

#include "pql_global.h"

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;
using ColumnHeader = unordered_map<string, int>;
using ResultTable = vector<vector<string>>;
using ResultRow = vector<string>;
using QueryResultList = vector<string>;
using QueryResultPairList = vector<pair<string, string>>;

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

  void InitTable(QueryResultList, string);

  void InitTable(QueryResultPairList, string, string);

  void MergeResults(QueryResultList, PqlResultTableConflict, int, string);

  void MergeResults(QueryResultPairList, PqlResultTableConflict, int, int, string, string);

  /* Setters */
  void AddColumnHeader(string var_name, int column_num);
  void ClearColumnHeader();
  void SetResultTable(ResultTable);
  void SetColumnCount(int);

  /* Getters */
  ColumnHeader GetColumnHeader();
  ResultTable GetResultTable();
  int GetColumnCount();
};

#endif  // !PQL_RESULT_H
