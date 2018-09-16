#pragma once

#ifndef QUERY_EVALUATOR_H
#define QUERY_EVALUATOR_H

#include <list>
#include <string>
#include <unordered_map>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_query.h"

using std::list;
using std::string;
using std::unordered_map;

/*A class to evaluate user query and return result to user*/
class PqlEvaluator {
 private:
  unordered_map<string, PqlDeclarationEntity>
      declarations_;        // the list of declarations
  string select_var_name_;  // the variable name of the 'Select' statement
  vector<PqlSuchthat> suchthats_;  // collection of such that clauses
  PqlDeclarationEntity
      select_type_;  // the type of the variable in 'Select' statement
  PKB pkb_;          // pkb database
  bool clauseFlag_;

 public:
  /* Contructor */
  PqlEvaluator();

  /* Setter */
  void setDeclarations(unordered_map<string, PqlDeclarationEntity>);
  void setSuchthat(vector<PqlSuchthat>);
  void setSelectVar(string);
  void setSelectType(PqlDeclarationEntity);
  void setPKB(PKB);
  void setClauseFlag(bool);

  /* Getter */
  unordered_map<string, PqlDeclarationEntity> getDeclarations();
  vector<PqlSuchthat> getSuchthat();
  string getSelectVar();
  PqlDeclarationEntity getSelectType();
  PKB getPKB();
  bool getClauseFlag();

  /**
   * Use the @Query provided by user
   * and return a list of results based on the query
   * @param query the query by the user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromQuery(PqlQuery* query, PKB pkb);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are no "such that" or "pattern" clause.
   * @input declaration entities of select variable and pkb
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromSelectAllQuery(PqlDeclarationEntity select_type);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are "such that" clause.
   * @input such that clause provided by user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromQueryWithSuchThat(PqlSuchthat suchthat);

  /**
   * Evaluate such that follows
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  list<string> EvaluateFollows(
      PqlDeclarationEntity select_type, PqlSuchthat suchthat,
      PqlArrangementOfSynonymInSuchthatParam arrangement);

  /**
   * Evaluate such that follows*
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  list<string> EvaluateFollowsT(
      PqlDeclarationEntity select_type, PqlSuchthat suchthat,
      PqlArrangementOfSynonymInSuchthatParam arrangement);

  /**
   * Evaluate such that parent
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  list<string> EvaluateParent(
      PqlDeclarationEntity select_type, PqlSuchthat suchthat,
      PqlArrangementOfSynonymInSuchthatParam arrangement);

  /**
   * Evaluate such that parent*
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  list<string> EvaluateParentT(
      PqlDeclarationEntity select_type, PqlSuchthat suchthat,
      PqlArrangementOfSynonymInSuchthatParam arrangement);

  /**
   * Evaluate such that uses stmt
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  list<string> EvaluateUsesS(
      PqlDeclarationEntity select_type, PqlSuchthat suchthat,
      PqlArrangementOfSynonymInSuchthatParam arrangement);

  /**
   * Evaluate such that modify stmt
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  list<string> EvaluateModifiesS(
      PqlDeclarationEntity select_type, PqlSuchthat suchthat,
      PqlArrangementOfSynonymInSuchthatParam arrangement);

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
  PqlArrangementOfSynonymInSuchthatParam
  CheckArrangementOfSynonymInSuchthatParam(
      string select_var_name, pair<pair<string, PqlDeclarationEntity>,
                                   pair<string, PqlDeclarationEntity>>
                                  such_that_param);

  /**
   * Filter the result list based on the selection entity type
   * @input unfiltered result list and selection entity type (e.g assign)
   * @returns result list that only contains result of a certain entity type
   */
  list<string> FilterResult(list<string> unfiltered_result,
                            PqlDeclarationEntity select_type);

  /**
   * Filter the result list based on the selection entity type and filter type
   * @input the filter type, unfiltered result list and selection entity type (e.g assign)
   * @returns result list that only contains result of a certain entity type
   */
  list<string> FilterPairResult(PqlResultFilterType filter_type,
                                list<pair<string,string>> unfiltered_pair_result,
                                PqlDeclarationEntity select_type);
};

#endif  // !QUERY_EVALUATOR_H