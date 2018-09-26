#pragma once

#ifndef PQL_EVALUATOR_H
#define PQL_EVALUATOR_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "pkb.h"
#include "pql_enum.h"
#include "pql_evaluator.h"
#include "pql_query.h"
#include "pql_result.h"

using std::list;
using std::string;
using std::unordered_map;
using std::vector;
using FinalResult = list<string>;
using QueryResultList = vector<string>;
using QueryResultPairList = vector<pair<string, string>>;

/*A class to evaluate user query and return result to user*/
class PqlEvaluator {
 private:
  PqlDeclarationEntity select_type_;  // the entity type of the select-variable
  PKB pkb_;                           // pkb database
  bool clause_flag_;                  // to determine if clauses are true/false
  bool merge_flag_;       // to determine if result table is empty after a merge
  PqlQuery pql_query_;    // the object where user query is stored
  PqlResult pql_result_;  // the object where results are stored

 public:
  /* Contructor */
  PqlEvaluator();

  /* Setter */
  void SetQuery(PqlQuery);
  void SetSelectType(PqlDeclarationEntity);
  void SetPKB(PKB);
  void SetClauseFlag(bool);
  void SetMergeFlag(bool);
  void SetPqlResult(PqlResult);

  /* Getter */
  PqlQuery GetQuery();
  PqlDeclarationEntity GetSelectType();
  PKB GetPKB();
  bool IsValidClause();
  bool IsMergeTableEmpty();
  PqlResult GetPqlResult();

  /**
   * Called by the GUI. Use the Query provided by user
   * and return a list of results based on the query
   * @param the query object and pkb
   * @returns a list<string> if there is result,
   * or an empty list otherwise
   */
  FinalResult GetResultFromQuery(PqlQuery* query, PKB pkb);

  /**
   * Get the final result based on the var name that user is 'selecting'
   * @param select-variable name
   * @returns a list<string> if there is result,
   * or an empty list otherwise
   */
  FinalResult GetFinalResultFromTable(string select_var);

  /**
   * Return a list of all the result of a certain type
   * @param declaration entity type
   * @returns a vector<string> if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetSelectAllResult(PqlDeclarationEntity select_type);

  /**
   * The main method for all suchthat-type queries
   * Method will determine which type of such that clause is to be evaluated, it
   * will also determine the arrangement of clause arguments
   * @param such that clause in the Query
   */
  void GetSuchThatResult(PqlSuchthat suchthat);

  /**
   * The main method for all pattern-type queries
   * Method will determine which type of pattern clause is to be evaluated
   * @param pattern clause in the Query
   */
  void GetPatternResult(PqlPattern pattern);

  /**
   * Evaluate assign pattern and store result in PqlResult table
   * @param pattern clause in the Query
   */
  void EvaluateAssignPattern(PqlPattern pattern);

  /**
   * Evaluate follows clause and store result in PqlResult table
   * @param follows clause in the Query and arrangement of clause arguments
   */
  void EvaluateFollows(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate follows* clause and store result in PqlResult table
   * @param follows* clause in the Query and arrangement of clause arguments
   */
  void EvaluateFollowsT(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate parent clause and store result in PqlResult table
   * @param parent clause in the Query and arrangement of clause arguments
   */
  void EvaluateParent(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate parent* clause and store result in PqlResult table
   * @param parent* clause in the Query and arrangement of clause arguments
   */
  void EvaluateParentT(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate uses (stmt) clause and store result in PqlResult table
   * @param uses clause in the Query and arrangement of clause arguments
   */
  void EvaluateUsesS(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate uses (procedure) clause and store result in PqlResult table
   * @param uses clause in the Query and arrangement of clause arguments
   */
  void EvaluateUsesP(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate modifies (stmt) clause and store result in PqlResult table
   * @param modifies clause in the Query and arrangement of clause arguments
   */
  void EvaluateModifiesS(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate modifies (procedure) clause and store result in PqlResult table
   * @param modifies clause in the Query and arrangement of clause arguments
   */
  void EvaluateModifiesP(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Determine the declaration type of the select variable
   * @param select-variable name
   * @returns entity type of the select-variable
   */
  PqlDeclarationEntity CheckSelectDeclarationType(string select_var_name);

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
   * @returns vector<string> list that only contains result of a certain entity
   * type
   */
  QueryResultList FilterResult(vector<string> unfiltered_result,
                               PqlDeclarationEntity entity_type);

  /**
   * Filter the pkb list based on the variable entity type
   * @param unfiltered list and variable entity type (e.g constant)
   * @returns vector<string> list that only contains result of a certain entity
   * type
   */
  QueryResultList FilterVariableResult(vector<string> unfiltered_result,
                                       PqlDeclarationEntity variable_type);

  /**
   * Filter the pkb list of pairs based on the declaration entity type
   * @param the filter type, unfiltered list and declaration entity type
   * for the left and right of the pair
   * @returns vector<string> of result pair that only contains result of a
   * certain entity type
   */
  QueryResultPairList FilterPairResult(
      PqlResultFilterType filter_type,
      vector<pair<string, string>> unfiltered_pair_result,
      PqlDeclarationEntity left_type, PqlDeclarationEntity right_type);

  /**
   * Get all the result from left of a pair and return it as a list
   * @param the vector<string> of pairs
   * @returns vector<string> of result in the left of the pair
   */
  QueryResultList GetAllLeftOfPair(QueryResultPairList filtered_list);

  /**
   * Get all the result from right of a pair and return it as a list
   * @param the vector<string> of pairs
   * @returns vector<string> of result in the right of the pair
   */
  QueryResultList GetAllRightOfPair(QueryResultPairList filtered_list);

  /**
   * Stores the list of results into the PqlResult table
   * @param the vector<string> of results and synonym-variable name
   */
  void StoreClauseResultInTable(QueryResultList, string);

  /**
   * Stores the list of pair into the PqlResult table
   * @param the vector<string> of pairs and the pair's synonym-variable name
   */
  void StoreClauseResultInTable(QueryResultPairList, string, string);
};

#endif  // !QUERY_EVALUATOR_H