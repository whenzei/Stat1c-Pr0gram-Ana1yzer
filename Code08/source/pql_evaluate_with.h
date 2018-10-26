#pragma once

#ifndef PQL_EVALUATE_WITH_H
#define PQL_EVALUATE_WITH_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"

using std::list;
using std::string;
using std::unordered_map;
using std::vector;
using FinalResult = list<string>;
using QueryResultList = vector<int>;
using QueryResultPairList = vector<pair<int, int>>;
using VarProcToIndexMap = unordered_map<string, int>;
using IndexToVarProcMap = unordered_map<int, string>;

/*A class to evaluate user query and return result to user*/
class PqlEvaluateWith {
 private:
  PKB pkb_;               // pkb database
  bool clause_flag_;      // to determine if clauses are true/false
  VarProcToIndexMap var_to_index;
  VarProcToIndexMap proc_to_index;

 public:
  /* Contructor */
  PqlEvaluateWith();

  /* Setter */
  void SetPKB(PKB);
  void SetClauseFlag(bool);

  /* Getter */
  PKB GetPKB();
  bool IsValidClause();

  bool EvaluateWithClause(PqlEvaluator*, PKB, PqlWith);

  /**
   * Return a list of all the result of a certain type
   * @param declaration entity type
   * @returns a vector<string> if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetSelectAllResult(PqlDeclarationEntity select_type);

  /**
   * Return a list of all the result in pairs of a certain type
   * @param declaration entity type
   * @returns a vector<pair<string>> if there is result,
   * or an empty list otherwise
   */
  QueryResultPairList GetSelectAllTwinResult(PqlDeclarationEntity select_type);

  /**
   * Evaluate with clause (1 synonym) and store result in PqlResult table
   * @param the synonym in with clause and the value for comparison
   */
  void EvaluateWithOneSynonym(PqlEvaluator*, Synonym with_syn,
                              string comparison_val);

  /**
   * Evaluate with clause (2 synonym) and store result in PqlResult table
   * @param the synonyms in with clause
   */
  void EvaluateWithTwoSynonym(PqlEvaluator*, Synonym left_param,
                              Synonym right_param);

  /**
   * Determine the number of synonym in the with param (e.g with
   * a.stmt# = b.stmt# => 2 synonyms)
   * @param parameter/arguments of the with clause
   * @returns arangement of arguments in the form of WithParamType enum
   */
  WithParamType CheckWithParamType(Parameters with_param);

  /**
   * Filter the result list based on the declaration entity type
   * @param unfiltered list and declaration entity type (e.g assign)
   * @returns vector<string> list that only contains result of a certain entity
   * type
   */
  QueryResultPairList FilterWithResult(QueryResultList unfiltered_result,
                                   PqlDeclarationEntity entity_type);
};

#endif  // !PQL_EVALUATE_WITH_H