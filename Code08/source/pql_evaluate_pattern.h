#pragma once

#ifndef PQL_EVALUATE_PATTERN_H
#define PQL_EVALUATE_PATTERN_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"
#include "pql_query.h"

using std::list;
using std::string;
using std::unordered_map;
using std::vector;
using QueryResultList = vector<int>;
using QueryResultPairList = vector<pair<int, int>>;

/*A class to evaluate the pql pattern clause*/
class PqlEvaluatePattern {
 private:
  PKB pkb_;           // pkb database
  bool clause_flag_;  // to determine if clauses are true/false

 public:
  /* Contructor */
  PqlEvaluatePattern();

  /* Setter */
  void SetPKB(PKB);
  void SetClauseFlag(bool);

  /* Getter */
  bool IsValidClause();

  /**
   * The main method for all pattern-type queries
   * Method will determine which type of pattern clause is to be evaluated
   * @param The evaluator, pkb and pattern clause in the Query
   * @return boolean of whether the clause is true/false
   */
  bool EvaluatePatternClause(PqlEvaluator*, PKB, PqlPattern);

  /**
   * Evaluate assign pattern and store result in PqlResult table
   * @param The evaluator and pattern clause in the Query
   */
  void EvaluateAssignPattern(PqlEvaluator*, PqlPattern pattern);

  /**
   * Evaluate while pattern and store result in PqlResult table
   * @param The evaluator and pattern clause in the Query
   */
  void EvaluateWhilePattern(PqlEvaluator*, PqlPattern pattern);

  /**
   * Evaluate if pattern and store result in PqlResult table
   * @param The evaluator and pattern clause in the Query
   */
  void EvaluateIfPattern(PqlEvaluator*, PqlPattern pattern);
};

#endif  // !PQL_EVALUATE_PATTERN_H