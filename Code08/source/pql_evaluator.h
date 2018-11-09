#pragma once

#ifndef PQL_EVALUATOR_H
#define PQL_EVALUATOR_H

#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_extractor.h"
#include "pql_global.h"
#include "pql_query.h"
#include "pql_result.h"
#include "result_hasher.h"

using std::list;
using std::set;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using FinalResult = list<string>;
using QueryResultSet = set<int>;
using QueryResultPairSet = set<pair<int, int>>;
using ResultTableList = vector<ResultTable>;
using ResultTableColumnHeader = unordered_map<string, pair<int, int>>;

/*A class to evaluate user query and return result to user*/
class PqlEvaluator {
 private:
  bool clause_flag_;      // to determine if clauses are true/false
  PqlQuery pql_query_;    // the object where user query is stored
  PqlResult pql_result_;  // the object where results are stored
  ResultTableList result_t_list_;
  ResultTableColumnHeader result_c_header_;

 public:
  /* Contructor */
  PqlEvaluator();

  /* Setter */
  void SetClauseFlag(bool);
  void SetPqlResult(PqlResult);
  void SetResultTableList(ResultTableList);
  void SetResultTableColumnHeader(ResultTableColumnHeader);

  /* Getter */
  bool IsValidClause();

  /**
   * Called by the GUI. Use the Query provided by user
   * and return a list of results based on the query
   * @param the query object and pkb
   * @returns a list<string> if there is result,
   * or an empty list otherwise
   */
  FinalResult GetResultFromQuery(PqlQuery* query, PKB pkb);

  /**
   * Stores the list of results into the PqlResult table
   * @param the vector<int> of results and synonym-variable name
   */
  void StoreClauseResultInTable(QueryResultSet, string);

  /**
   * Stores the list of pair into the PqlResult table
   * @param the vector<int> of pairs and the pair's synonym-variable name
   */
  void StoreClauseResultInTable(QueryResultPairSet, string, string);

  /**
   * Stores the list of results into the PqlResult table
   * @param the hashmap<int,set> of results and left & right synonym-variable
   * name
   */
  void StoreClauseResultInTable(AffectsMap, string, string);
};

#endif  // !QUERY_EVALUATOR_H