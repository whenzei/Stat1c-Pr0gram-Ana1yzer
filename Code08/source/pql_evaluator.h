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
using QueryResultList = vector<int>;
using QueryResultPairList = vector<pair<int, int>>;
using VarProcToIndexMap = unordered_map<string, int>;
using IndexToVarProcMap = unordered_map<int, string>;

/*A class to evaluate user query and return result to user*/
class PqlEvaluator {
 private:
  bool clause_flag_;      // to determine if clauses are true/false
  PqlQuery pql_query_;    // the object where user query is stored
  PqlResult pql_result_;  // the object where results are stored
  IndexToVarProcMap index_to_var;
  IndexToVarProcMap index_to_proc;
  VarProcToIndexMap var_to_index;
  VarProcToIndexMap proc_to_index;

 public:
  /* Contructor */
  PqlEvaluator();

  /* Setter */
  void SetClauseFlag(bool);
  void SetPqlResult(PqlResult);
  void SetIndexToVar(IndexToVarProcMap);
  void SetIndexToProc(IndexToVarProcMap);

  /* Getter */
  bool IsValidClause();
  PqlResult GetPqlResult();
  IndexToVarProcMap GetIndexToVar();
  IndexToVarProcMap GetIndexToProc();

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
   * @returns a vector<int> if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetResultFromTable(Synonym, PKB);

  /**
   * Return a list of all the result of a certain type
   * @param declaration entity type
   * @returns a vector<int> if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetSelectAllResult(PqlDeclarationEntity, PKB);

  /**
   * Take in the list of results and do a cross product for tuple
   * @param List of results of each select clause
   */
  void TupleCrossProduct(FinalResult& final_result, string& temp_result,
                         vector<FinalResult>::iterator curr,
                         vector<FinalResult>::iterator end);

  /**
   * Convert the results from int to string using pkb mapping
   * @param the vector<int> of results and synonym type
   */
  FinalResult ConvertListIntToString(QueryResultList, PqlDeclarationEntity);

  /**
   * Stores the list of results into the PqlResult table
   * @param the vector<int> of results and synonym-variable name
   */
  void StoreClauseResultInTable(QueryResultList, string);

  /**
   * Stores the list of pair into the PqlResult table
   * @param the vector<int> of pairs and the pair's synonym-variable name
   */
  void StoreClauseResultInTable(QueryResultPairList, string, string);

  /* Helper function to trim a string */
  string Trim(const string&);
};

#endif  // !QUERY_EVALUATOR_H