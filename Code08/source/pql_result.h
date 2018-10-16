#pragma once

#ifndef PQL_RESULT_H
#define PQL_RESULT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pql_global.h"

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using ColumnHeader = unordered_map<string, int>;
using MergeMap = unordered_map<string, vector<string>>;
using MergeSet = unordered_set<string>;
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
  MergeMap merge_map_;
  MergeSet merge_set_;

 public:
  /* Constructor */
  PqlResult();

  void InitTable(QueryResultList, string);

  void InitTable(QueryResultPairList, string, string);

  void MergeResults(QueryResultList, PqlResultTableConflict, int, string);

  void MergeResults(QueryResultPairList, PqlResultTableConflict, int, int,
                    string, string);

  void SetupMergeMap(QueryResultPairList, PqlResultTableConflict);

  void SetupMergeSet(QueryResultList);

  void SetupMergeSet(QueryResultPairList);

  /* Setters */
  void AddColumnHeader(string var_name, int column_num);
  void ClearColumnHeader();
  void SetResultTable(ResultTable);
  void SetColumnCount(int);
  void AddMergeMap(string key, string value);
  void AddMergeSet(string key);
  void ClearMergeMap();
  void ClearMergeSet();

  /* Getters */
  ColumnHeader GetColumnHeader();
  MergeMap GetMergeMap();
  MergeSet GetMergeSet();
  ResultTable GetResultTable();
  int GetColumnCount();
};

#endif  // !PQL_RESULT_H
