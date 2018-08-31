#pragma once

#include <string>
#include <vector>
#include "query.h"

#ifndef QUERY_EVALUATOR_H
#define QUERY_EVALUATOR_H

class QueryEvaluator {
 public:
  /* Contructor */
  QueryEvaluator();

  /**
   * Use the Query provided by user
   * and return a result string based on the query
   * @param query the query by the user
   * @returns result as a string if there is a result, or "none"
   * otherwise
   */
  std::string GetResultFromQuery(Query query);
};

#endif  // !QUERY_EVALUATOR_H