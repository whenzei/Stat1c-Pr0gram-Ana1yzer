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
 public:
  /* Contructor */
  PqlEvaluator();

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
   * @param declaration entities of select variable and pkb
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromSelectAllQuery(PqlDeclarationEntity select_type,
                                           PKB pkb);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are "such that" clause.
   * @param declaration entities of select variable, such that clause provided
   * by user and pkb
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromQueryWithSuchThat(PqlDeclarationEntity select_type,
                                              PqlSuchthat suchthat, PKB pkb);

  /**
   * Determine the declaration type of the select variable
   * @param select clause and declaration entities provided by user
   * @returns PqlDeclarationEntity of the select variable
   */
  PqlDeclarationEntity CheckSelectDeclarationType(
      string select_var_name,
      unordered_map<string, PqlDeclarationEntity> declarations);

  /**
   * Determine the number of synonym in the such that param (e.g such that
   * Follows(a,b) => 2 entity) also determines whether the synonym in the param
   * is a select variable (e.g Select a)
   * @param left and right param of the such that clause
   * @returns properties of synonym in the form of
   * PqlPropertyOfEntityInSuchthatParam
   */
  PqlArrangementOfSynonymInSuchthatParam
  CheckArrangementOfSynonymInSuchthatParam(
      string select_var_name, pair<pair<string, PqlDeclarationEntity>,
                                   pair<string, PqlDeclarationEntity>>
                                  such_that_param);
};

#endif  // !QUERY_EVALUATOR_H