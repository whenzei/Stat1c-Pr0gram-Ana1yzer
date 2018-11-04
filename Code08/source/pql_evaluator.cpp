#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_evaluate_pattern.h"
#include "pql_evaluate_suchthat.h"
#include "pql_evaluate_with.h"
#include "pql_evaluator.h"
#include "pql_global.h"
#include "pql_query.h"

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::vector;

PqlEvaluator::PqlEvaluator() {}

FinalResult PqlEvaluator::GetResultFromQuery(PqlQuery* query, PKB pkb) {
  // Initialise new result table class
  SetPqlResult(PqlResult());
  SetResultTableList(ResultTableList());
  SetResultTableColumnHeader(ResultTableColumnHeader());

  // Default value should be true, until the clause returns a false
  SetClauseFlag(true);
  FinalResult final_results;

  PqlEvaluateWith with_evaluator;
  PqlEvaluatePattern pattern_evaluator;
  PqlEvaluateSuchthat suchthat_evaluator;
  int groupcount = 1;

  if (query->GetGroups().empty()) {
    // Means no extra clause, straight go to projector
  } else {
    // Iterate through the groups
    for (auto& group : query->GetGroups()) {
      // Iterate through the clauses
      for (auto& clause : group) {
        // Code for processing clauses
        switch (clause->GetClauseType()) {
          case PqlClauseType::kSuchthat:
            SetClauseFlag(suchthat_evaluator.EvaluateSuchthatClause(
                this, pkb, *(PqlSuchthat*)clause));
            continue;
          case PqlClauseType::kPattern:
            SetClauseFlag(pattern_evaluator.EvaluatePatternClause(
                this, pkb, *(PqlPattern*)clause));
            continue;
          case PqlClauseType::kWith:
            SetClauseFlag(with_evaluator.EvaluateWithClause(this, pkb,
                                                            *(PqlWith*)clause));
            continue;
        }
        // If the clause is false already, no need to continue evaluating
        if (!IsValidClause()) {
          break;
        }
      }  // end one group iteration

      if (!pql_result_.GetResultTable().empty()) {
        // Get the result table and column info
        result_t_list_.push_back(pql_result_.GetResultTable());
        // Get column headers
        for (auto& header : pql_result_.GetColumnHeader()) {
          this->result_c_header_.emplace(header.first,
                                         make_pair(groupcount, header.second));
        }
        // Reset the result table, clear everything
        ResultTable new_table;
        pql_result_.SetResultTable(new_table);
        pql_result_.SetColumnCount(0);
        pql_result_.ClearColumnHeader();
        groupcount++;
      }
    }  // end all group iteration
       // Go to projector here
  }

  cout << "Result size: " << final_results.size() << endl;

  return final_results;
}

void PqlEvaluator::StoreClauseResultInTable(QueryResultList result_list,
                                            string new_header_name) {
  if (pql_result_.GetResultTable().empty()) {
    pql_result_.InitTable(result_list, new_header_name);
  } else {
    ColumnHeader col_header = pql_result_.GetColumnHeader();
    // Conflict found
    if (col_header.find(new_header_name) != col_header.end()) {
      pql_result_.MergeResults(result_list, kConflict,
                               col_header.find(new_header_name)->second,
                               new_header_name);
    }
    // No conflict
    else {
      pql_result_.MergeResults(result_list, kNoConflict, -1, new_header_name);
    }
  }

  // If after merging result, result table is empty
  if (pql_result_.GetResultTable().empty()) {
    SetClauseFlag(false);
  }
}

void PqlEvaluator::StoreClauseResultInTable(
    QueryResultPairList result_pair_list, string header_name_left,
    string header_name_right) {
  if (pql_result_.GetResultTable().empty()) {
    pql_result_.InitTable(result_pair_list, header_name_left,
                          header_name_right);
  } else {
    ColumnHeader col_header = pql_result_.GetColumnHeader();
    // Two conflict found
    if (col_header.find(header_name_left) != col_header.end() &&
        col_header.find(header_name_right) != col_header.end()) {
      pql_result_.MergeResults(result_pair_list, kTwoConflict,
                               col_header.find(header_name_left)->second,
                               col_header.find(header_name_right)->second,
                               header_name_left, header_name_right);
    }
    // One conflict Left
    else if (col_header.find(header_name_left) != col_header.end()) {
      pql_result_.MergeResults(result_pair_list, kOneConflictLeft,
                               col_header.find(header_name_left)->second, -1,
                               header_name_left, header_name_right);
    }
    // One conflict Right
    else if (col_header.find(header_name_right) != col_header.end()) {
      pql_result_.MergeResults(result_pair_list, kOneConflictRight, -1,
                               col_header.find(header_name_right)->second,
                               header_name_left, header_name_right);
    }
    // No conflict
    else {
      pql_result_.MergeResults(result_pair_list, kNoConflict, -1, -1,
                               header_name_left, header_name_right);
    }
  }

  // If after merging result, result table is empty
  if (pql_result_.GetResultTable().empty()) {
    SetClauseFlag(false);
  }
}

/* Getters and Setters */

void PqlEvaluator::SetClauseFlag(bool clause_flag) {
  this->clause_flag_ = clause_flag;
}

bool PqlEvaluator::IsValidClause() { return clause_flag_; }

void PqlEvaluator::SetPqlResult(PqlResult pql_result) {
  this->pql_result_ = pql_result;
}

void PqlEvaluator::SetResultTableList(ResultTableList result_t_list) {
  this->result_t_list_ = result_t_list;
}

void PqlEvaluator::SetResultTableColumnHeader(
    ResultTableColumnHeader result_c_header) {
  this->result_c_header_ = result_c_header;
}