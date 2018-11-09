#pragma once

#ifndef PQL_RESULT_H
#define PQL_RESULT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "result_hasher.h"
#include "pql_extractor.h"
#include "pql_global.h"

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using ColumnHeader = unordered_map<string, int>;
using MergeMap = unordered_map<int, vector<int>>;
using MergeSet = unordered_set<string>;
using ResultTable = unordered_set<vector<int>>;
using ResultRow = vector<int>;
using QueryResultSet = unordered_set<int>;
using QueryResultPairSet = unordered_set<pair<int, int>,ResultHasher>;

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

  void InitTable(QueryResultSet, string);

  void InitTable(QueryResultPairSet, string, string);

  void InitTable(AffectsMap, string, string);

  void MergeResults(QueryResultSet, PqlResultTableConflict, int, string);

  void MergeResults(QueryResultPairSet, PqlResultTableConflict, int, int,
                    string, string);

  void MergeResults(AffectsMap, PqlResultTableConflict, int, int, string,
                    string);

  void SetupMergeMap(QueryResultPairSet, PqlResultTableConflict);

  void SetupMergeMap(AffectsMap, PqlResultTableConflict);

  void SetupMergeSet(QueryResultSet);

  void SetupMergeSet(QueryResultPairSet);

  void SetupMergeSet(AffectsMap);

  /* Setters */
  void AddColumnHeader(string var_name, int column_num);
  void ClearColumnHeader();
  void SetResultTable(ResultTable);
  void SetColumnCount(int);
  void AddMergeMap(int key, int value);
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
