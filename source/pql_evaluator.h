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
  PqlDeclarationEntity
      select_type_;  // the type of the variable in 'Select' statement
  PKB pkb_;          // pkb database
  bool clause_flag_;
  bool merge_flag_;
  PqlQuery pql_query_;
  PqlResult pql_result_;

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
   * Get the final result based on the var name that user is 'selecting'
   * @input result table and select var name
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  FinalResult GetFinalResultFromTable(string select_var);

  /**
   * Use the @Query provided by user
   * and return a list of results based on the query
   * @input query the query by the user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  FinalResult GetResultFromQuery(PqlQuery* query, PKB pkb);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are no "such that" or "pattern" clause.
   * @input declaration entities of select variable and pkb
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetSelectAllResult(PqlDeclarationEntity select_type);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are "such that" clause.
   * @input such that clause provided by user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  void GetSuchThatResult(PqlSuchthat suchthat);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are "pattern" clause.
   * @input pattern clause provided by user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  void GetPatternResult(PqlPattern pattern);

  /**
   * Evaluate assign pattern
   * @input pattern clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateAssignPattern(PqlPattern pattern);

  /**
   * Evaluate such that follows
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateFollows(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate such that follows*
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateFollowsT(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate such that parent
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateParent(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate such that parent*
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateParentT(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate such that uses stmt
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateUsesS(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate such that uses procedure
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateUsesP(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate such that modify stmt
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateModifiesS(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Evaluate such that modify procedure
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  void EvaluateModifiesP(PqlSuchthat suchthat, SuchthatParamType arrangement);

  /**
   * Determine the declaration type of the select variable
   * @input select clause and declaration entities provided by user
   * @returns PqlDeclarationEntity of the select variable
   */
  PqlDeclarationEntity CheckSelectDeclarationType(string select_var_name);

  /**
   * Determine the number of synonym in the such that param (e.g such that
   * Follows(a,b) => 2 entity) also determines whether the synonym in the param
   * is a select variable (e.g Select a)
   * @input left and right param of the such that clause
   * @returns properties of synonym in the form of
   * PqlPropertyOfEntityInSuchthatParam
   */
  SuchthatParamType CheckSuchthatParamType(Parameters such_that_param);

  /**
   * Filter the result list based on the selection entity type
   * @input unfiltered result list and selection entity type (e.g assign)
   * @returns result list that only contains result of a certain entity type
   */
  QueryResultList FilterResult(vector<string> unfiltered_result,
                               PqlDeclarationEntity entity_type);

  /**
   * Filter the result list based on the variable entity type
   * @input unfiltered result list and variable entity type (e.g constant)
   * @returns result list that only contains result of a certain entity type
   */
  QueryResultList FilterVariableResult(vector<string> unfiltered_result,
                                       PqlDeclarationEntity variable_type);

  /**
   * Filter the result pair list based on the selection entity type and filter
   * type
   * @input the filter type, unfiltered result list and selection entity type
   * (e.g assign) for left and right of the pair
   * @returns result pair list that only contains result of a certain entity
   * type
   */
  QueryResultPairList FilterPairResult(
      PqlResultFilterType filter_type,
      vector<pair<string, string>> unfiltered_pair_result,
      PqlDeclarationEntity left_type, PqlDeclarationEntity right_type);

  /**
   * Get all the result from left of the pair and return it in a list
   * @input the list of pairs
   * @returns list of result in the left of the pair
   */
  QueryResultList GetAllLeftOfPair(QueryResultPairList filtered_list);

  /**
   * Get all the result from right of the pair and return it in a list
   * @input the list of pairs
   * @returns list of result in the right of the pair
   */
  QueryResultList GetAllRightOfPair(QueryResultPairList filtered_list);

  void StoreClauseResultInTable(QueryResultList, string);

  void StoreClauseResultInTable(QueryResultPairList, string, string);
};

#endif  // !QUERY_EVALUATOR_H