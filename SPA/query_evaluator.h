#pragma once

#include <list>
#include <string>
#include <vector>

#include "declaration.h"
#include "query.h"

#ifndef QUERY_EVALUATOR_H
#define QUERY_EVALUATOR_H

/*A class to evaluate user query and return result to user*/
class QueryEvaluator {
 private:
  DeclarationKeyword select_type;

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
  std::list<std::string> GetResultFromQuery(Query query);

  /**
   * Use the results from @GetResultFromQuery
   * and format it into a single string to display to the user
   * @param results the list of string containing result of the query
   * @returns a string to be displayed to the user
   */
  std::string FormatResultString(std::list<std::string> results);
};

#endif  // !QUERY_EVALUATOR_H