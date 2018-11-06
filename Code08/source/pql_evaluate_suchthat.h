#pragma once

#ifndef PQL_EVALUATE_SUCHTHAT_H
#define PQL_EVALUATE_SUCHTHAT_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"
#include "pql_query.h"
#include "pql_extractor.h"

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
class PqlEvaluateSuchthat {
 private:
  PKB* pkb_;          // pkb database
  bool clause_flag_;  // to determine if clauses are true/false
  PqlExtractor* pqle_;

 public:
  /* Contructor */
  PqlEvaluateSuchthat();

  /* Setter */
  void SetPKB(PKB*);
  void SetPqlExtractor(PqlExtractor*);
  void SetClauseFlag(bool);

  /* Getter */
  bool IsValidClause();

  /**
   * The main method for all suchthat-type queries
   * Method will determine which type of such that clause is to be evaluated, it
   * will also determine the arrangement of clause arguments
   * @param The evaluator, pkb and such that clause in the Query
   * @return boolean of whether the clause is true/false
   */
  bool EvaluateSuchthatClause(PqlEvaluator* pql_eval, PKB* pkb,
                              PqlSuchthat suchthat, PqlExtractor*);

  /**
   * Return a list of all the result of a certain type
   * @param declaration entity type
   * @returns a vector<int> if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetSelectAllResult(PqlDeclarationEntity select_type);

  /**
   * Evaluate follows clause and store result in PqlResult table
   * @param The evaluator and follows clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateFollows(PqlEvaluator*, PqlSuchthat suchthat,
                       SuchthatParamType arrangement);

  /**
   * Evaluate follows* clause and store result in PqlResult table
   * @param The evaluator and follows* clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateFollowsT(PqlEvaluator*, PqlSuchthat suchthat,
                        SuchthatParamType arrangement);

  /**
   * Evaluate parent clause and store result in PqlResult table
   * @param The evaluator and parent clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateParent(PqlEvaluator*, PqlSuchthat suchthat,
                      SuchthatParamType arrangement);

  /**
   * Evaluate parent* clause and store result in PqlResult table
   * @param The evaluator and parent* clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateParentT(PqlEvaluator*, PqlSuchthat suchthat,
                       SuchthatParamType arrangement);

  /**
   * Evaluate uses (stmt) clause and store result in PqlResult table
   * @param The evaluator and uses clause in the Query and arrangement of clause
   * arguments
   */
  void EvaluateUsesS(PqlEvaluator*, PqlSuchthat suchthat,
                     SuchthatParamType arrangement);

  /**
   * Evaluate uses (procedure) clause and store result in PqlResult table
   * @param The evaluator and uses clause in the Query and arrangement of clause
   * arguments
   */
  void EvaluateUsesP(PqlEvaluator*, PqlSuchthat suchthat,
                     SuchthatParamType arrangement);

  /**
   * Evaluate modifies (stmt) clause and store result in PqlResult table
   * @param The evaluator and modifies clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateModifiesS(PqlEvaluator*, PqlSuchthat suchthat,
                         SuchthatParamType arrangement);

  /**
   * Evaluate modifies (procedure) clause and store result in PqlResult table
   * @param The evaluator and modifies clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateModifiesP(PqlEvaluator*, PqlSuchthat suchthat,
                         SuchthatParamType arrangement);

  /**
   * Evaluate Call clause and store result in PqlResult table
   * @param The evaluator and Call clause in the Query and arrangement of clause
   * arguments
   */
  void EvaluateCalls(PqlEvaluator*, PqlSuchthat suchthat,
                     SuchthatParamType arrangement);

  /**
   * Evaluate Call* clause and store result in PqlResult table
   * @param The evaluator and Call* clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateCallsT(PqlEvaluator*, PqlSuchthat suchthat,
                      SuchthatParamType arrangement);

  /**
   * Evaluate Next clause and store result in PqlResult table
   * @param The evaluator and Next clause in the Query and arrangement of clause
   * arguments
   */
  void EvaluateNext(PqlEvaluator*, PqlSuchthat suchthat,
                    SuchthatParamType arrangement);

  /**
   * Evaluate Next* clause and store result in PqlResult table
   * @param The evaluator and Next* clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateNextT(PqlEvaluator*, PqlSuchthat suchthat,
                     SuchthatParamType arrangement);

  /**
   * Evaluate Affects clause and store result in PqlResult table
   * @param The evaluator and affects clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateAffects(PqlEvaluator*, PqlSuchthat suchthat,
                       SuchthatParamType arrangement);

  /**
   * Evaluate Affects* clause and store result in PqlResult table
   * @param The evaluator and affects* clause in the Query and arrangement of
   * clause arguments
   */
  void EvaluateAffectsT(PqlEvaluator*, PqlSuchthat suchthat,
                        SuchthatParamType arrangement);

  /**
   * Determine the number of synonym in the such that param (e.g such that
   * Follows(a,b) => 2 synonyms)
   * @param parameter/arguments of the such that clause
   * @returns arangement of arguments in the form of SuchthatParamType enum
   */
  SuchthatParamType CheckSuchthatParamType(Parameters such_that_param);

  /**
   * Filter the pkb list based on the declaration entity type
   * @param unfiltered list and declaration entity type (e.g assign)
   * @returns vector<int> list that only contains result of a certain entity
   * type
   */
  QueryResultList FilterResult(QueryResultList unfiltered_result,
                               PqlDeclarationEntity entity_type);

  /**
   * Filter the pkb list based on the variable entity type
   * @param unfiltered list and variable entity type (e.g constant)
   * @returns vector<int> list that only contains result of a certain entity
   * type
   */
  QueryResultList FilterVariableResult(QueryResultList unfiltered_result,
                                       PqlDeclarationEntity variable_type);

  /**
   * Filter the pkb list of pairs based on the declaration entity type
   * @param the filter type, unfiltered list and declaration entity type
   * for the left and right of the pair
   * @returns vector<int> of result pair that only contains result of a
   * certain entity type
   */
  QueryResultPairList FilterPairResult(
      PqlResultFilterType filter_type,
      QueryResultPairList unfiltered_pair_result,
      PqlDeclarationEntity left_type, PqlDeclarationEntity right_type);
};

#endif  // !PQL_EVALUATE_SUCHTHAT_H