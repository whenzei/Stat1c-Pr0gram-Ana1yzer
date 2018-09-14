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

 public:
  /* Contructor */
  PqlEvaluator();

  /* Setter */
  void setDeclarations(unordered_map<string, PqlDeclarationEntity>);
  void setSuchthat(vector<PqlSuchthat>);
  void setSelectVar(string);
  void setSelectType(PqlDeclarationEntity);
  void setPKB(PKB);

  /* Getter */
  unordered_map<string, PqlDeclarationEntity> getDeclarations();
  vector<PqlSuchthat> getSuchthat();
  string getSelectVar();
  PqlDeclarationEntity getSelectType();
  PKB getPKB();

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
  list<string> GetResultFromSelectAllQuery();

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are "such that" clause.
   * @input declaration entities of select variable, such that clause provided
   * by user and pkb
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  list<string> GetResultFromQueryWithSuchThat();

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
};

#endif  // !QUERY_EVALUATOR_H