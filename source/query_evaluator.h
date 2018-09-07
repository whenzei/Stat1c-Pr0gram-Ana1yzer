#pragma once

#ifndef QUERY_EVALUATOR_H
#define QUERY_EVALUATOR_H

#include <list>
#include <string>

#include "pkb.h"
#include "pql_query.h"
#include "query_evaluator.h"

using std::list;
using std::string;

/*A class to evaluate user query and return result to user*/
class QueryEvaluator {
 public:
  /* Contructor */
  QueryEvaluator();

  /**
   * Use the @Query provided by user
   * and return a list of results based on the query
   * @param query the query by the user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromQuery(PqlQuery* query, PKB pkb);

  /**
   * Use the @Query provided by user
   * and return a list of results based on the query. This method will only be
   * used when there are no "such that" or "pattern" clause.
   * @param select clause and declaration entities provided by user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromSelectAllQuery(
      string select_var_name, map<string, PqlDeclarationEntity> declarations,
      PKB pkb);

  /**
   * Use the results from @GetResultFromQuery
   * and format it into a single string to display to the user
   * @param results the list of string containing result of the query
   * @returns a string to be displayed to the user
   */
  string FormatResultString(list<string> results);
};

#endif  // !QUERY_EVALUATOR_H