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
  SetIndexToProc(pkb.GetIndexToProcMapping());
  SetIndexToVar(pkb.GetIndexToVarMapping());

  // Default value should be true, until the clause returns a false
  SetClauseFlag(true);
  FinalResult final_results;
  vector<FinalResult> tuple_results;

  PqlEvaluateWith with_evaluator;
  PqlEvaluatePattern pattern_evaluator;
  PqlEvaluateSuchthat suchthat_evaluator;

  // If there is no such that/pattern/with clause, then evaluator will use
  // GetSelectAllResult method
  if (query->GetClauses().empty()) {
    if (query->GetSelections().size() > 1) {
      for (auto& select_syn : query->GetSelections()) {
        QueryResultList select_all_result =
            GetSelectAllResult(select_syn.second, pkb);
        if (select_all_result.empty()) {
          SetClauseFlag(false);
        } else {
          tuple_results.push_back(
              ConvertListIntToString(select_all_result, select_syn.second));
        }
        // If the clause is false already, no need to continue evaluating
        if (!IsValidClause()) {
          break;
        }
      }
    }
    // Only one select
    else {
      PqlDeclarationEntity select_type = query->GetSelections().front().second;
      QueryResultList select_all_result = GetSelectAllResult(select_type, pkb);
      if (select_all_result.empty()) {
        SetClauseFlag(false);
      } else {
        final_results = ConvertListIntToString(select_all_result, select_type);
      }
    }
  }
  // Else evaluate all the such that/with/pattern clause
  else {
    for (auto& clause_iter : query->GetClauses()) {
      switch (clause_iter->GetClauseType()) {
        case PqlClauseType::kSuchthat:
          SetClauseFlag(suchthat_evaluator.EvaluateSuchthatClause(
              this, pkb, *(PqlSuchthat*)clause_iter));
          // GetSuchThatResult(*(PqlSuchthat*)clause_iter);
          continue;
        case PqlClauseType::kPattern:
          SetClauseFlag(pattern_evaluator.EvaluatePatternClause(
              this, pkb, *(PqlPattern*)clause_iter));
          // GetPatternResult(*(PqlPattern*)clause_iter);
          continue;
        case PqlClauseType::kWith:
          SetClauseFlag(with_evaluator.EvaluateWithClause(
              this, pkb, *(PqlWith*)clause_iter));
          // GetWithResult(*(PqlWith*)clause_iter);
          continue;
      }
      // If the clause is false already, no need to continue evaluating
      if (!IsValidClause()) {
        break;
      }
    }

    // If there are no false clauses and it is not Select BOOLEAN, prepare final
    // result
    if (IsValidClause() && !query->GetSelections().empty()) {
      // Select tuple result
      if (query->GetSelections().size() > 1) {
        for (auto& select_syn : query->GetSelections()) {
          tuple_results.push_back(ConvertListIntToString(
              GetResultFromTable(select_syn, pkb), select_syn.second));
        }
      }
      // Select single result
      else {
        Synonym select_syn = query->GetSelections().front();

        QueryResultList other_clause_result =
            GetResultFromTable(select_syn, pkb);
        final_results =
            ConvertListIntToString(other_clause_result, select_syn.second);
      }
    }
  }

  // Do cross product for tuple results
  if (!tuple_results.empty() && IsValidClause()) {
    string temp;
    TupleCrossProduct(final_results, temp, tuple_results.begin(),
                      tuple_results.end());
  }

  // Select BOOLEAN
  if (query->GetSelections().empty()) {
    if (IsValidClause()) {
      final_results.push_back("true");
    } else {
      final_results.push_back("false");
    }
  }

  cout << "Result size: " << final_results.size() << endl;

  return final_results;
}

QueryResultList PqlEvaluator::GetResultFromTable(Synonym select_syn, PKB pkb) {
  QueryResultList table_result;
  ResultTable result_table = pql_result_.GetResultTable();

  ColumnHeader column_header = pql_result_.GetColumnHeader();

  ColumnHeader::iterator col_iter = column_header.find(select_syn.first);

  // Get results from result table
  if (col_iter != column_header.end()) {
    int column_index = col_iter->second;
    for (auto& row : result_table) {
      table_result.push_back(row[column_index]);
    }
  }
  // Select synonym not in result table
  else {
    table_result = GetSelectAllResult(select_syn.second, pkb);
    if (table_result.empty()) {
      SetClauseFlag(false);
    }
  }

  return table_result;
}

QueryResultList PqlEvaluator::GetSelectAllResult(
    PqlDeclarationEntity select_type, PKB pkb) {
  QueryResultList results;

  switch (select_type) {
    case PqlDeclarationEntity::kProcedure:
      // Get all procedures name from PKB and store into
      // results list
      cout << "Select all procedure." << endl;
      return pkb.GetAllProcIndices();
    case PqlDeclarationEntity::kVariable:
      // Get all variable name from PKB and store into
      // results list
      cout << "Select all variables." << endl;
      return pkb.GetAllVarIndices();
    case PqlDeclarationEntity::kAssign:
      // Get all statement number of statement which
      // contains assignment from PKB and store into results
      // list
      cout << "Select all assign statement." << endl;
      return pkb.GetAllAssignStmt();
    case PqlDeclarationEntity::kStmt:
      // Get all stmt number from PKB and store into results
      // list
      cout << "Select all statement." << endl;
      return pkb.GetAllStmt();
    case PqlDeclarationEntity::kRead:
      // Get all read stmt from PKB and store into results
      // list
      cout << "Select all read statement." << endl;
      return pkb.GetAllReadStmt();
    case PqlDeclarationEntity::kPrint:
      // Get all print stmt from PKB and store into results
      // list
      cout << "Select all print statement." << endl;
      return pkb.GetAllPrintStmt();
    case PqlDeclarationEntity::kCall:
      // Get all call stmt from PKB and store into results
      // list
      cout << "Select all call statement." << endl;
      return pkb.GetAllCallStmt();
    case PqlDeclarationEntity::kCallName:
      // Get all call stmt from PKB and store into results
      // list
      cout << "Select all call.procName statement." << endl;
      return pkb.GetAllCallee();
    case PqlDeclarationEntity::kWhile:
      // Get all while stmt from PKB and store into results
      // list
      cout << "Select all while statement." << endl;
      return pkb.GetAllWhileStmt();
    case PqlDeclarationEntity::kIf:
      // Get all if stmt from PKB and store into results
      // list
      cout << "Select all if statement." << endl;
      return pkb.GetAllIfStmt();
    case PqlDeclarationEntity::kConstant:
      // Get all constant from PKB and store into results
      // list
      cout << "Select all constants." << endl;
      return pkb.GetAllConstValue();
    case PqlDeclarationEntity::kProgline:
      // Get all program line from PKB and store into
      // results list
      cout << "Select all program lines." << endl;
      return pkb.GetAllStmt();
  }

  // Return empty result if nothing is found
  return results;
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

FinalResult PqlEvaluator::ConvertListIntToString(
    QueryResultList result_list, PqlDeclarationEntity select_type) {
  FinalResult string_list;

  cout << "Converting int to string" << endl;

  if (select_type != PqlDeclarationEntity::kVariable &&
      select_type != PqlDeclarationEntity::kProcedure) {
    for (auto& result : result_list) {
      string_list.push_back(to_string(result));
    }
  } else if (select_type == PqlDeclarationEntity::kVariable) {
    IndexToVarProcMap itv = GetIndexToVar();
    for (auto& result : result_list) {
      string_list.push_back(itv[result]);
    }
  } else if (select_type == PqlDeclarationEntity::kProcedure) {
    IndexToVarProcMap itp = GetIndexToProc();
    for (auto& result : result_list) {
      string_list.push_back(itp[result]);
    }
  }

  return string_list;
}

void PqlEvaluator::TupleCrossProduct(FinalResult& final_result,
                                     string& temp_result,
                                     vector<FinalResult>::iterator curr,
                                     vector<FinalResult>::iterator end) {
  if (curr == end) {
    // terminal condition of the recursion. We no longer have
    // any input vectors to manipulate. Add the current result (temp_result)
    // to the final set of results (final_result).
    final_result.push_back(Trim(temp_result));
    return;
  }
  const FinalResult& curr_list = *curr;
  for (FinalResult::const_iterator it = curr_list.begin();
       it != curr_list.end(); it++) {
    string temp_prev = temp_result;
    temp_result += *it + " ";
    TupleCrossProduct(final_result, temp_result, curr + 1, end);
    temp_result = temp_prev;
  }
}

string PqlEvaluator::Trim(const string& str) {
  size_t first = str.find_first_not_of(' ');
  if (string::npos == first) {
    return str;
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

/* Getters and Setters */

void PqlEvaluator::SetIndexToVar(IndexToVarProcMap map) {
  this->index_to_var = map;
}

void PqlEvaluator::SetIndexToProc(IndexToVarProcMap map) {
  this->index_to_proc = map;
}

IndexToVarProcMap PqlEvaluator::GetIndexToVar() { return this->index_to_var; }

IndexToVarProcMap PqlEvaluator::GetIndexToProc() { return this->index_to_proc; }

void PqlEvaluator::SetClauseFlag(bool clause_flag) {
  this->clause_flag_ = clause_flag;
}

bool PqlEvaluator::IsValidClause() { return clause_flag_; }

void PqlEvaluator::SetPqlResult(PqlResult pql_result) {
  this->pql_result_ = pql_result;
}