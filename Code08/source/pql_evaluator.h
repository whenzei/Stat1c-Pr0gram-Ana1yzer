#pragma once

#ifndef PQL_EVALUATOR_H
#define PQL_EVALUATOR_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"
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
  PKB pkb_;               // pkb database
  bool clause_flag_;      // to determine if clauses are true/false
  PqlQuery pql_query_;    // the object where user query is stored
  PqlResult pql_result_;  // the object where results are stored

 public:
  /* Contructor */
  PqlEvaluator();

  /* Setter */
  void SetQuery(PqlQuery);
  void SetPKB(PKB);
  void SetClauseFlag(bool);
  void SetPqlResult(PqlResult);

  /* Getter */
  PqlQuery GetQuery();
  PKB GetPKB();
  bool IsValidClause();
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
   * Get the results from table
   * @param the select synonym
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetResultFromTable(Synonym);

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
   * The main method for all with-type queries
   * Method will determine which type of with clause is to be evaluated, it
   * will also determine the arrangement of clause arguments
   * @param with clause in the Query
   */
  void GetWithResult(PqlWith with);

  /**
   * Evaluate with clause (1 synonym) and store result in PqlResult table
   * @param the synonym in with clause and the value for comparison
   */
  void EvaluateWithOneSynonym(Synonym with_syn, string comparison_val);

  /**
   * Evaluate with clause (2 synonym) and store result in PqlResult table
   * @param the synonyms in with clause
   */
  void EvaluateWithTwoSynonym(Synonym left_param, Synonym right_param);

  /**
   * Evaluate assign pattern and store result in PqlResult table
   * @param pattern clause in the Query
   */
  void EvaluateAssignPattern(PqlPattern pattern);

  /**
   * Evaluate while pattern and store result in PqlResult table
   * @param pattern clause in the Query
   */
  void EvaluateWhilePattern(PqlPattern pattern);

  /**
   * Evaluate if pattern and store result in PqlResult table
   * @param pattern clause in the Query
   */
  void EvaluateIfPattern(PqlPattern pattern);

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
   * Determine the number of synonym in the such that param (e.g such that
   * Follows(a,b) => 2 synonyms)
   * @param parameter/arguments of the such that clause
   * @returns arangement of arguments in the form of SuchthatParamType enum
   */
  SuchthatParamType CheckSuchthatParamType(Parameters such_that_param);

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
  QueryResultList FilterWithResult(QueryResultList unfiltered_result,
                                   PqlDeclarationEntity entity_type);

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