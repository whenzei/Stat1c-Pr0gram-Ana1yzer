#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_extractor.h"
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
  SetQuery(*query);
  SetPKB(pkb);
  // Initialise new result table class
  PqlResult* pql_result = new PqlResult();
  SetPqlResult(*pql_result);
  SetIndexToProc(pkb.GetIndexToProcMapping());
  SetIndexToVar(pkb.GetIndexToVarMapping());
  SetProcToIndex(pkb.GetProcToIndexMapping());
  SetVarToIndex(pkb.GetVarToIndexMapping());

  // Default value should be true, until the clause returns a false
  SetClauseFlag(true);
  FinalResult final_results;
  vector<FinalResult> tuple_results;

  PqlQuery user_query = GetQuery();

  // If there is no such that/pattern/with clause, then evaluator will use
  // GetSelectAllResult method
  if (user_query.GetClauses().empty()) {
    if (user_query.GetSelections().size() > 1) {
      for (auto& select_syn : user_query.GetSelections()) {
        QueryResultList select_all_result =
            GetSelectAllResult(select_syn.second);
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
      PqlDeclarationEntity select_type =
          user_query.GetSelections().front().second;
      QueryResultList select_all_result = GetSelectAllResult(select_type);
      if (select_all_result.empty()) {
        SetClauseFlag(false);
      } else {
        final_results = ConvertListIntToString(select_all_result, select_type);
      }
    }
  }
  // Else use GetSuchThatResult/GetPatternResult method
  else {
    for (auto& clause_iter : user_query.GetClauses()) {
      switch (clause_iter->GetClauseType()) {
        case PqlClauseType::kSuchthat:
          GetSuchThatResult(*(PqlSuchthat*)clause_iter);
          continue;
        case PqlClauseType::kPattern:
          GetPatternResult(*(PqlPattern*)clause_iter);
          continue;
        case PqlClauseType::kWith:
          GetWithResult(*(PqlWith*)clause_iter);
          continue;
      }
      // If the clause is false already, no need to continue evaluating
      if (!IsValidClause()) {
        break;
      }
    }

    // No false clause and it is not BOOLEAN
    if (IsValidClause() && !user_query.GetSelections().empty()) {
      if (user_query.GetSelections().size() > 1) {
        for (auto& select_syn : user_query.GetSelections()) {
          tuple_results.push_back(ConvertListIntToString(
              GetResultFromTable(select_syn), select_syn.second));
        }
      }
      // Only one select
      else {
        Synonym select_syn = GetQuery().GetSelections().front();

        QueryResultList other_clause_result = GetResultFromTable(select_syn);
        final_results =
            ConvertListIntToString(other_clause_result, select_syn.second);
      }
    }
  }

  cout << "Tuple size: " << tuple_results.size() << endl;

  // Do tuple processing
  if (!tuple_results.empty() && IsValidClause()) {
    string temp;
    TupleCrossProduct(final_results, temp, tuple_results.begin(),
                      tuple_results.end());
  }

  // If BOOLEAN
  if (user_query.GetSelections().empty()) {
    if (IsValidClause()) {
      final_results.push_back("true");
    } else {
      final_results.push_back("false");
    }
  }

  cout << "Result size: " << final_results.size() << endl;

  return final_results;
}

QueryResultList PqlEvaluator::GetResultFromTable(Synonym select_syn) {
  QueryResultList table_result;
  ResultTable result_table = GetPqlResult().GetResultTable();

  ColumnHeader column_header = GetPqlResult().GetColumnHeader();

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
    cout << "Select all in (GetFinalResultFromTable)" << endl;
    table_result = GetSelectAllResult(select_syn.second);
    if (table_result.empty()) {
      SetClauseFlag(false);
    }
  }

  return table_result;
}

void PqlEvaluator::GetPatternResult(PqlPattern pattern) {
  PqlPatternType pattern_type = pattern.GetType().second;

  switch (pattern_type) {
    case PqlPatternType::kAssign:
      EvaluateAssignPattern(pattern);
      break;
    case PqlPatternType::kWhile:
      EvaluateWhilePattern(pattern);
      break;
    case PqlPatternType::kIf:
      EvaluateIfPattern(pattern);
      break;
  }
}

void PqlEvaluator::GetWithResult(PqlWith with) {
  WithParamType arrangement = CheckWithParamType(with.GetParameters());
  Parameters with_param = with.GetParameters();
  Synonym left_param = with_param.first;
  Synonym right_param = with_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;

  switch (arrangement) {
    case kWithNoSynonym:
      // If not equal
      if (left_name.compare(right_name) != 0) {
        SetClauseFlag(false);
      }
      return;
    case kWithOneSynonymLeft:
      EvaluateWithOneSynonym(left_param, right_name);
      return;
    case kWithOneSynonymRight:
      EvaluateWithOneSynonym(right_param, left_name);
      return;
    case kWithTwoSynonym:
      EvaluateWithTwoSynonym(left_param, right_param);
      return;
  }
}

void PqlEvaluator::GetSuchThatResult(PqlSuchthat suchthat) {
  SuchthatParamType arrangement =
      CheckSuchthatParamType(suchthat.GetParameters());

  switch (suchthat.GetType()) {
    case PqlSuchthatType::kFollows:
      EvaluateFollows(suchthat, arrangement);
      break;
    case PqlSuchthatType::kFollowsT:
      EvaluateFollowsT(suchthat, arrangement);
      break;
    case PqlSuchthatType::kParent:
      EvaluateParent(suchthat, arrangement);
      break;
    case PqlSuchthatType::kParentT:
      EvaluateParentT(suchthat, arrangement);
      break;
    case PqlSuchthatType::kUsesS:
      EvaluateUsesS(suchthat, arrangement);
      break;
    case PqlSuchthatType::kUsesP:
      EvaluateUsesP(suchthat, arrangement);
      break;
    case PqlSuchthatType::kModifiesS:
      EvaluateModifiesS(suchthat, arrangement);
      break;
    case PqlSuchthatType::kModifiesP:
      EvaluateModifiesP(suchthat, arrangement);
      break;
    case PqlSuchthatType::kCalls:
      EvaluateCalls(suchthat, arrangement);
      break;
    case PqlSuchthatType::kCallsT:
      EvaluateCallsT(suchthat, arrangement);
      break;
    case PqlSuchthatType::kNext:
      EvaluateNext(suchthat, arrangement);
      break;
    case PqlSuchthatType::kNextT:
      EvaluateNextT(suchthat, arrangement);
      break;
    case PqlSuchthatType::kAffects:
      EvaluateAffects(suchthat, arrangement);
      break;
    case PqlSuchthatType::kAffectsT:
      EvaluateAffectsT(suchthat, arrangement);
      break;
  }
}

QueryResultList PqlEvaluator::GetSelectAllResult(
    PqlDeclarationEntity select_type) {
  PKB pkb = GetPKB();
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

QueryResultPairList PqlEvaluator::GetSelectAllTwinResult(
    PqlDeclarationEntity select_type) {
  PKB pkb = GetPKB();
  QueryResultPairList results;

  switch (select_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Select all twin procedure." << endl;
      return pkb.GetAllProcIndexTwin();
    case PqlDeclarationEntity::kVariable:
      cout << "Select all twin variables." << endl;
      return pkb.GetAllVarIndexTwin();
    case PqlDeclarationEntity::kAssign:
      cout << "Select all twin assign statement." << endl;
      return pkb.GetAllAssignStmtTwin();
    case PqlDeclarationEntity::kStmt:
      cout << "Select all twin statement." << endl;
      return pkb.GetAllStmtTwin();
    case PqlDeclarationEntity::kRead:
      cout << "Select all twin read statement." << endl;
      return pkb.GetAllReadStmtTwin();
    case PqlDeclarationEntity::kPrint:
      cout << "Select all twin print statement." << endl;
      return pkb.GetAllPrintStmtTwin();
    case PqlDeclarationEntity::kCall:
      cout << "Select all twin call statement." << endl;
      return pkb.GetAllCallStmtTwin();
    case PqlDeclarationEntity::kCallName:
      cout << "Select all twin call.procName statement." << endl;
      return pkb.GetAllCalleeTwin();
    case PqlDeclarationEntity::kWhile:
      cout << "Select all twin while statement." << endl;
      return pkb.GetAllWhileStmtTwin();
    case PqlDeclarationEntity::kIf:
      cout << "Select all twin if statement." << endl;
      return pkb.GetAllIfStmtTwin();
    case PqlDeclarationEntity::kConstant:
      cout << "Select all twin constants." << endl;
      return pkb.GetAllConstValueTwin();
    case PqlDeclarationEntity::kProgline:
      cout << "Select all twin program lines." << endl;
      return pkb.GetAllStmtTwin();
  }

  // Return empty result if nothing is found
  return results;
}

void PqlEvaluator::EvaluateWithTwoSynonym(Synonym left_param,
                                          Synonym right_param) {
  cout << "Evaluating with two synonym" << endl;

  PKB pkb = GetPKB();
  // Getting parameter of with
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_left, result_right;
  QueryResultList filtered_result;

  // If same type, get twin pair
  if (left_type == right_type) {
    StoreClauseResultInTable(GetSelectAllTwinResult(left_type), left_name,
                             right_name);
  } else {
    result_left = GetSelectAllResult(left_type);
    result_right = GetSelectAllResult(right_type);

    // Check size to ensure fastest filter
    if (result_left.size() < result_right.size()) {
      filtered_result = FilterWithResult(result_left, right_type);
    } else {
      filtered_result = FilterWithResult(result_right, left_type);
    }

    if (filtered_result.empty()) {
      SetClauseFlag(false);
    } else {
      // Store both synonym in result table
      StoreClauseResultInTable(filtered_result, left_name);
      StoreClauseResultInTable(filtered_result, right_name);
    }
  }
}

void PqlEvaluator::EvaluateWithOneSynonym(Synonym with_syn,
                                          string comparison_val) {
  cout << "Evaluating with one synonym" << endl;

  PKB pkb = GetPKB();
  // Getting parameter of with
  string syn_name = with_syn.first;
  PqlDeclarationEntity syn_type = with_syn.second;
  QueryResultList result_list;
  VarProcToIndexMap proc_to_index = GetProcToIndex();
  VarProcToIndexMap var_to_index = GetVarToIndex();

  switch (syn_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Is procedure?" << endl;
      if (proc_to_index.find(comparison_val) != proc_to_index.end()) {
        if (pkb.IsProcIndex(proc_to_index[comparison_val])) {
          result_list.push_back(proc_to_index[comparison_val]);
          StoreClauseResultInTable(result_list, syn_name);
        } else {
          SetClauseFlag(false);
        }
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kVariable:
      cout << "Is variable?" << endl;
      if (var_to_index.find(comparison_val) != var_to_index.end()) {
        if (pkb.IsVarIndex(var_to_index[comparison_val])) {
          result_list.push_back(var_to_index[comparison_val]);
          StoreClauseResultInTable(result_list, syn_name);
        } else {
          SetClauseFlag(false);
        }
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kAssign:
      cout << "Is assign?" << endl;
      if (pkb.IsAssignStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kStmt:
      cout << "Is stmt?" << endl;
      if (pkb.IsStmtNum(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kRead:
      cout << "Is read?" << endl;
      if (pkb.IsReadStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kPrint:
      cout << "Is print?" << endl;
      if (pkb.IsPrintStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCall:
      cout << "Is call?" << endl;
      if (pkb.IsCallStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCallName:
      cout << "Is call procname?" << endl;
      if (proc_to_index.find(comparison_val) != proc_to_index.end()) {
        if (pkb.IsCalledProc(proc_to_index[comparison_val])) {
          result_list.push_back(proc_to_index[comparison_val]);
          StoreClauseResultInTable(result_list, syn_name);
        } else {
          SetClauseFlag(false);
        }
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kWhile:
      cout << "Is while?" << endl;
      if (pkb.IsWhileStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kIf:
      cout << "Is if?" << endl;
      if (pkb.IsIfStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kConstant:
      cout << "Is const?" << endl;
      if (pkb.IsConstValue(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kProgline:
      cout << "Is progline?" << endl;
      if (pkb.IsStmtNum(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
  }
}

void PqlEvaluator::EvaluateWhilePattern(PqlPattern pattern) {
  PKB pkb = GetPKB();
  // Getting parameter of pattern
  string pattern_var_name = pattern.GetType().first;
  pair<string, PqlDeclarationEntity> first_parameter =
      pattern.GetFirstParameter();
  string left_name = first_parameter.first;
  PqlDeclarationEntity left_type = first_parameter.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating While Pattern." << endl;

  switch (left_type) {
    case PqlDeclarationEntity::kUnderscore:
      result_list = pkb.GetWhileWithPattern("");
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern " << endl;
      } else {
        StoreClauseResultInTable(result_list, pattern_var_name);
        cout << " Size: " << result_list.size() << endl;
      }
      return;
    case PqlDeclarationEntity::kIdent:
      result_list = pkb.GetWhileWithPattern(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for variable" << left_name << endl;
      } else {
        StoreClauseResultInTable(result_list, pattern_var_name);
        cout << " Size: " << result_list.size() << endl;
      }
      return;
    case PqlDeclarationEntity::kVariable:
      result_pair_list = pkb.GetAllWhilePatternPair();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for synonym " << left_name << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, pattern_var_name, left_name);
        cout << " Size: " << result_pair_list.size() << endl;
      }
      return;
  }
}

void PqlEvaluator::EvaluateIfPattern(PqlPattern pattern) {
  PKB pkb = GetPKB();
  // Getting parameter of pattern
  string pattern_var_name = pattern.GetType().first;
  pair<string, PqlDeclarationEntity> first_parameter =
      pattern.GetFirstParameter();
  string left_name = first_parameter.first;
  PqlDeclarationEntity left_type = first_parameter.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating If Pattern." << endl;

  switch (left_type) {
    case PqlDeclarationEntity::kUnderscore:
      result_list = pkb.GetIfWithPattern("");
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern " << endl;
      } else {
        StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kIdent:
      result_list = pkb.GetIfWithPattern(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for variable" << left_name << endl;
      } else {
        StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kVariable:
      result_pair_list = pkb.GetAllIfPatternPair();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for synonym " << left_name << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, pattern_var_name, left_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateAssignPattern(PqlPattern pattern) {
  PKB pkb = GetPKB();
  // Getting parameter of pattern
  string pattern_var_name = pattern.GetType().first;
  pair<string, PqlDeclarationEntity> first_parameter =
      pattern.GetFirstParameter();
  Expression expression = pattern.GetAssignExpression();
  string left_name = first_parameter.first;
  PqlDeclarationEntity left_type = first_parameter.second;
  PqlPatternExpressionType expression_type = expression.first;
  TokenList expression_token = expression.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Assign Pattern." << endl;

  switch (expression_type) {
    case PqlPatternExpressionType::kExpression:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = pkb.GetAssignWithExactPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern " << endl;
          } else {
            StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list =
              pkb.GetAssignWithExactPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for " << left_name << endl;
          } else {
            StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list = pkb.GetAllAssignExactPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for variable " << left_name << endl;
          } else {
            StoreClauseResultInTable(result_pair_list, pattern_var_name,
                                     left_name);
          }
          return;
      }
    case PqlPatternExpressionType::kUnderscoreExpressionUnderscore:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = pkb.GetAssignWithPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern " << endl;
          } else {
            StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list = pkb.GetAssignWithPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for " << left_name << endl;
          } else {
            StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list = pkb.GetAllAssignPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for variable " << left_name << endl;
          } else {
            StoreClauseResultInTable(result_pair_list, pattern_var_name,
                                     left_name);
          }
          return;
      }
    case PqlPatternExpressionType::kUnderscore:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = pkb.GetAssignWithPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern " << endl;
          } else {
            StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list = pkb.GetAssignWithPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for " << left_name << endl;
          } else {
            StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list = pkb.GetAllAssignPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for variable " << left_name << endl;
          } else {
            StoreClauseResultInTable(result_pair_list, pattern_var_name,
                                     left_name);
          }
          return;
      }
  }
}

void PqlEvaluator::EvaluateCalls(PqlSuchthat suchthat,
                                 SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Calls" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb.IsCall(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not call by " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb.GetCaller(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not called " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb.GetCallee(left_name).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not caller " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasCallsRelationship()) {
        SetClauseFlag(false);
        cout << " no calls relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = pkb.GetCaller(right_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not called " << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllCaller();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "there are no call for procedure" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetCallee(stoi(left_name));
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not caller " << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = pkb.GetAllCallee();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " there are no procedures being called" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllCallPairs();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Call(proc,proc)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateCallsT(PqlSuchthat suchthat,
                                  SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Calls*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb.IsCallT(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not indirectly call by " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb.GetCallerT(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly called " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb.GetCalleeT(left_name).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirect caller " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasCallsRelationship()) {
        SetClauseFlag(false);
        cout << " no calls relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = pkb.GetCallerT(right_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly called " << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllCaller();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "there are no call for procedure" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetCalleeT(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirect caller " << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = pkb.GetAllCallee();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " there are no procedures being called" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllCallTPairs();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Call*(proc,proc)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateNext(PqlSuchthat suchthat,
                                SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Next" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb.IsNext(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not executed after" << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb.GetPrevious(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not executed after anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb.GetNext(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not executed before anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasNextRelationship()) {
        SetClauseFlag(false);
        cout << " no next relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = FilterResult(pkb.GetPrevious(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not executed after any left type" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllPrevious(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "left type is not executed before anything" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetNext(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not executed before any right type" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllNext(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right type is not executed before anything" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pkb.GetAllNextPairs(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Next(left,right)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateNextT(PqlSuchthat suchthat,
                                 SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  PqlExtractor pqle = PqlExtractor(pkb);
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Next*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pqle.IsNextT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not indirectly executed after" << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pqle.GetPreviousT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly executed after anything "
             << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pqle.GetNextT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirectly executed before anything "
             << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasNextRelationship()) {
        SetClauseFlag(false);
        cout << " no next relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pqle.GetPreviousT(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly executed after any left type"
             << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllPrevious(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "left type is not indirectly executed before anything" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pqle.GetNextT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirectly executed before any right type"
             << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllNext(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right type is not indirectly executed after anything" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pqle.GetAllNextTPairs(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Next*(left,right)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateAffects(PqlSuchthat suchthat,
                                   SuchthatParamType arrangement) {}

void PqlEvaluator::EvaluateAffectsT(PqlSuchthat suchthat,
                                    SuchthatParamType arrangement) {}

void PqlEvaluator::EvaluateFollows(PqlSuchthat suchthat,
                                   SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Follows." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT followed by right
      if (!pkb.IsFollows(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not followed by " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If right is not follower
      if (pkb.GetFollowedBy(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not a follower " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If left is not followee
      if (pkb.GetFollows(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not a followee " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasFollowsRelationship()) {
        SetClauseFlag(false);
        cout << " no follow relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pkb.GetFollowedBy(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No followee for this type " << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllFollowedBy(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have any follower" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetFollows(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No follower of this type " << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllFollows(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt is not following anyone" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      // Filter left and right
      result_pair_list = FilterPairResult(kFilterBoth, pkb.GetAllFollowsPair(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of (lefttype,righttype)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateFollowsT(PqlSuchthat suchthat,
                                    SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Follows*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT followed by right
      if (!pkb.IsFollowsT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not indirectly followed by " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If right is not follower
      if (pkb.GetFollowedByT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not an indirect follower " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If left is not followee
      if (pkb.GetFollowsT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not an indirect followee " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasFollowsRelationship()) {
        SetClauseFlag(false);
        cout << " no follow relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pkb.GetFollowedByT(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No indirect followee for this type " << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllFollowedBy(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have any follower" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetFollowsT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No indirect follower of this type " << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllFollows(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt is not following anyone" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      // Filter left and right
      result_pair_list = FilterPairResult(kFilterBoth, pkb.GetAllFollowsTPair(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no indirect pair of (lefttype,righttype)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateParent(PqlSuchthat suchthat,
                                  SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Parent." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT parent of right
      if (!pkb.IsParent(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not parent of " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If no parent
      if (pkb.GetParent(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " has no parent " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no child
      if (pkb.GetChild(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " has no child " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasParentRelationship()) {
        SetClauseFlag(false);
        cout << " no parent/child found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = FilterResult(pkb.GetParent(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have " << right_name << " as child"
             << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllParent(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt is not a parent" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetChild(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have stmt " << left_name << " as parent"
             << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllChild(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have any parent" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pkb.GetAllParentPair(),
                                          left_type, right_type);
      // Filter left and right
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of (lefttype,righttype)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateParentT(PqlSuchthat suchthat,
                                   SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating Parent*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT parent of right
      if (!pkb.IsParentT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not indirect parent of " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If no parent
      if (pkb.GetParentT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " has no indirect parent " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no child
      if (pkb.GetChildT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " has no indirect child " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb.HasParentRelationship()) {
        SetClauseFlag(false);
        cout << " no parent/child found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = FilterResult(pkb.GetParentT(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have " << right_name
             << " as indirect child" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllParent(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt is not a parent" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetChildT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have stmt " << left_name
             << " as indirect parent" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllChild(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have any parent" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pkb.GetAllParentTPair(),
                                          left_type, right_type);
      // Filter left and right
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no indirect pair of (lefttype,righttype)" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateUsesS(PqlSuchthat suchthat,
                                 SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating UsesS." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If stmt (left) doesnt use variable (right)
      if (!pkb.IsUsedByS(stoi(left_name), right_name)) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't use " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If nothing were used by this stmt
      if (pkb.GetUsedVarS(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't use any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      // If no stmt of left syn entity type uses the right variable
      result_list = FilterResult(pkb.GetUsingStmt(right_name), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt use right variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllUsingStmt(), left_type);
      // If no stmt of left syn entity type uses the any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt use any variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list =
          FilterVariableResult(pkb.GetUsedVarS(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesnt use any variable of this type"
             << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterLeft, pkb.GetAllUsesPairS(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt use any variable" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateUsesP(PqlSuchthat suchthat,
                                 SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating UsesP." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If proc (left) doesnt use variable (right)
      if (!pkb.IsUsedByP(left_name, right_name)) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesn't use " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If nothing were used by this proc
      if (pkb.GetUsedVarP(left_name).empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesn't use any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      // If no proc uses the right variable
      result_list = pkb.GetUsingProc(right_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc doesnt use right variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllUsingProc();
      // If no proc of left syn entity type uses the any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc of left type doesnt use any variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list =
          FilterVariableResult(pkb.GetUsedVarP(left_name), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesnt use any variable of this type"
             << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllUsesPairP();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "Procedure doesnt use any variable" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateModifiesS(PqlSuchthat suchthat,
                                     SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating ModifiesS." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If stmt (left) doesnt modify variable (right)
      if (!pkb.IsModifiedByS(stoi(left_name), right_name)) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no variables were modified by this stmt
      if (pkb.GetModifiedVarS(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = FilterResult(pkb.GetModifyingS(right_name), left_type);
      // If no stmt of left syn entity type modifies the right variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt modify right variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllModifyingS(), left_type);
      // If no stmt of left syn entity type modifies any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt modify any variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetModifiedVarS(stoi(left_name));
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesnt modify any variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(
          kFilterLeft, pkb.GetAllModifiesPairS(), left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "No stmt/variable pair found" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::EvaluateModifiesP(PqlSuchthat suchthat,
                                     SuchthatParamType arrangement) {
  PKB pkb = GetPKB();
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  cout << "Evaluating ModifiesP." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If proc (left) doesnt modify variable (right)
      if (!pkb.IsModifiedByP(left_name, right_name)) {
        SetClauseFlag(false);
        cout << "Procedure " << left_name << " doesn't modify " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no variables were modified by this proc
      if (pkb.GetModifiedVarP(left_name).empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = pkb.GetModifyingP(right_name);
      // If no proc modifies the right variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "No proc modify the right variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllModifyingP();
      // If no proc modifies any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc doesnt modify any variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetModifiedVarP(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesnt modify any variable" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllModifiesPairP();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "No proc/variable pair found" << endl;
      } else {
        StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluator::StoreClauseResultInTable(QueryResultList result_list,
                                            string new_header_name) {
  PqlResult pql_result = GetPqlResult();

  if (pql_result.GetResultTable().empty()) {
    pql_result.InitTable(result_list, new_header_name);
  } else {
    ColumnHeader col_header = pql_result.GetColumnHeader();
    // Conflict found
    if (col_header.find(new_header_name) != col_header.end()) {
      pql_result.MergeResults(result_list, kConflict,
                              col_header.find(new_header_name)->second,
                              new_header_name);
    }
    // No conflict
    else {
      pql_result.MergeResults(result_list, kNoConflict, -1, new_header_name);
    }
  }

  // If after merging result, result table is empty
  if (pql_result.GetResultTable().empty()) {
    SetClauseFlag(false);
  }
  SetPqlResult(pql_result);
}

void PqlEvaluator::StoreClauseResultInTable(
    QueryResultPairList result_pair_list, string header_name_left,
    string header_name_right) {
  PqlResult pql_result = GetPqlResult();

  if (pql_result.GetResultTable().empty()) {
    pql_result.InitTable(result_pair_list, header_name_left, header_name_right);
  } else {
    ColumnHeader col_header = pql_result.GetColumnHeader();
    // Two conflict found
    if (col_header.find(header_name_left) != col_header.end() &&
        col_header.find(header_name_right) != col_header.end()) {
      pql_result.MergeResults(result_pair_list, kTwoConflict,
                              col_header.find(header_name_left)->second,
                              col_header.find(header_name_right)->second,
                              header_name_left, header_name_right);
    }
    // One conflict Left
    else if (col_header.find(header_name_left) != col_header.end()) {
      pql_result.MergeResults(result_pair_list, kOneConflictLeft,
                              col_header.find(header_name_left)->second, -1,
                              header_name_left, header_name_right);
    }
    // One conflict Right
    else if (col_header.find(header_name_right) != col_header.end()) {
      pql_result.MergeResults(result_pair_list, kOneConflictRight, -1,
                              col_header.find(header_name_right)->second,
                              header_name_left, header_name_right);
    }
    // No conflict
    else {
      pql_result.MergeResults(result_pair_list, kNoConflict, -1, -1,
                              header_name_left, header_name_right);
    }
  }

  // If after merging result, result table is empty
  if (pql_result.GetResultTable().empty()) {
    SetClauseFlag(false);
  }
  SetPqlResult(pql_result);
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

QueryResultList PqlEvaluator::FilterWithResult(
    QueryResultList unfiltered_result, PqlDeclarationEntity entity_type) {
  QueryResultList filtered_result;
  PKB pkb = GetPKB();

  switch (entity_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Filter Proc" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsProcIndex(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kVariable:
      cout << "Filter Var" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsVarIndex(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kAssign:
      cout << "Filter Assign" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsAssignStmt(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kStmt:
      cout << "Filter Stmt" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsStmtNum(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kRead:
      cout << "Filter Read" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsReadStmt(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kPrint:
      cout << "Filter Print" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsPrintStmt(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kCall:
      cout << "Filter Call" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsCallStmt(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kCallName:
      cout << "Filter CallName" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsCalledProc(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kWhile:
      cout << "Filter While" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsWhileStmt(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kIf:
      cout << "Filter If" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsIfStmt(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kConstant:
      cout << "Filter Constant" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsConstValue(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kProgline:
      cout << "Filter Progline" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsStmtNum(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
  }

  return filtered_result;
}

QueryResultList PqlEvaluator::FilterResult(QueryResultList unfiltered_result,
                                           PqlDeclarationEntity entity_type) {
  QueryResultList filtered_result;
  PKB pkb = GetPKB();

  // If its of type stmt (not assign, if, while etc) just return the list,
  // nothing to filter
  if (entity_type == PqlDeclarationEntity::kStmt ||
      entity_type == PqlDeclarationEntity::kProgline) {
    return unfiltered_result;
  }

  for (auto& iter : unfiltered_result) {
    int result = iter;

    if (pkb.GetStmtType(result) == entity_type) {
      filtered_result.push_back(result);
    }
  }

  return filtered_result;
}

QueryResultList PqlEvaluator::FilterVariableResult(
    QueryResultList unfiltered_result, PqlDeclarationEntity variable_type) {
  QueryResultList filtered_result;
  PKB pkb = GetPKB();

  for (auto& iter : unfiltered_result) {
    if (variable_type == PqlDeclarationEntity::kConstant &&
        pkb.IsConstValue(iter)) {
      filtered_result.push_back(iter);
    } else if (variable_type == PqlDeclarationEntity::kVariable &&
               pkb.IsVarIndex(iter)) {
      filtered_result.push_back(iter);
    }
  }

  return filtered_result;
}

QueryResultPairList PqlEvaluator::FilterPairResult(
    PqlResultFilterType filter_type, QueryResultPairList unfiltered_pair_result,
    PqlDeclarationEntity left_type, PqlDeclarationEntity right_type) {
  QueryResultPairList filtered_result;
  PKB pkb = GetPKB();

  // Nothing to filter if the type is stmt
  if (filter_type == kFilterLeft) {
    if (left_type == PqlDeclarationEntity::kStmt ||
        left_type == PqlDeclarationEntity::kProgline) {
      return unfiltered_pair_result;
    }
  } else if (filter_type == kFilterRight) {
    if (right_type == PqlDeclarationEntity::kStmt ||
        right_type == PqlDeclarationEntity::kProgline) {
      return unfiltered_pair_result;
    }
  } else {
    // If both left and right are stmt/program line
    if ((left_type == PqlDeclarationEntity::kStmt ||
         left_type == PqlDeclarationEntity::kProgline) &&
        (right_type == PqlDeclarationEntity::kStmt ||
         right_type == PqlDeclarationEntity::kProgline)) {
      return unfiltered_pair_result;
    } else if (left_type == PqlDeclarationEntity::kStmt ||
               left_type == PqlDeclarationEntity::kProgline) {
      filter_type = kFilterRight;
    } else if (right_type == PqlDeclarationEntity::kStmt ||
               right_type == PqlDeclarationEntity::kProgline) {
      filter_type = kFilterLeft;
    }
  }

  for (auto& iter : unfiltered_pair_result) {
    int left_result = iter.first;
    int right_result = iter.second;
    switch (filter_type) {
      case kFilterLeft:
        if (pkb.GetStmtType(left_result) == left_type) {
          filtered_result.push_back(iter);
        }
        break;
      case kFilterRight:
        if (pkb.GetStmtType(right_result) == right_type) {
          filtered_result.push_back(iter);
        }
        break;
      case kFilterBoth:
        if (pkb.GetStmtType(left_result) == left_type &&
            pkb.GetStmtType(right_result) == right_type) {
          filtered_result.push_back(iter);
        }
        break;
    }
  }

  return filtered_result;
}

WithParamType PqlEvaluator::CheckWithParamType(Parameters with_param) {
  pair<string, PqlDeclarationEntity> left_param = with_param.first;
  pair<string, PqlDeclarationEntity> right_param = with_param.second;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;

  // Syn = ?
  if (left_type != PqlDeclarationEntity::kInteger &&
      left_type != PqlDeclarationEntity::kIdent) {
    // Syn = Syn
    if (right_type != PqlDeclarationEntity::kInteger &&
        right_type != PqlDeclarationEntity::kIdent) {
      return kWithTwoSynonym;
    }
    // Syn = int/ident
    else {
      return kWithOneSynonymLeft;
    }
  }
  // int/ident = ?
  else {
    // int/ident = Syn
    if (right_type != PqlDeclarationEntity::kInteger &&
        right_type != PqlDeclarationEntity::kIdent) {
      return kWithOneSynonymRight;
    }
    // int/ident = int/ident
    else {
      return kWithNoSynonym;
    }
  }
}

SuchthatParamType PqlEvaluator::CheckSuchthatParamType(
    Parameters such_that_param) {
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;

  // (_,?)
  if (left_type == PqlDeclarationEntity::kUnderscore) {
    //(_,_)
    if (right_type == PqlDeclarationEntity::kUnderscore) {
      return kNoSynonymUnderscoreBoth;
    }
    //(_,syn)
    else if (right_type != PqlDeclarationEntity::kInteger &&
             right_type != PqlDeclarationEntity::kIdent) {
      return kOneSynonymRightUnderscoreLeft;
    }
    //(_,int/ident)
    else {
      return kNoSynonymUnderscoreLeft;
    }
  }  // end (_,?)

  //(syn,?)
  else if (left_type != PqlDeclarationEntity::kInteger &&
           left_type != PqlDeclarationEntity::kIdent) {
    //(syn,_)
    if (right_type == PqlDeclarationEntity::kUnderscore) {
      return kOneSynonymLeftUnderscoreRight;
    }
    //(syn,syn)
    else if (right_type != PqlDeclarationEntity::kInteger &&
             right_type != PqlDeclarationEntity::kIdent) {
      return kTwoSynonym;
    }
    //(syn,int/ident)
    else {
      return kOneSynonymLeft;
    }
  }  // end (syn,?)

  //(int/ident, ?)
  else {
    //(int/ident,_)
    if (right_type == PqlDeclarationEntity::kUnderscore) {
      return kNoSynonymUnderscoreRight;
    }
    //(int/ident,syn)
    else if (right_type != PqlDeclarationEntity::kInteger &&
             right_type != PqlDeclarationEntity::kIdent) {
      return kOneSynonymRight;
    }
    //(int/ident,int/ident)
    else {
      return kNoSynonym;
    }
  }  // end (int/ident, ?)
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

void PqlEvaluator::SetVarToIndex(VarProcToIndexMap map) {
  this->var_to_index = map;
}

void PqlEvaluator::SetProcToIndex(VarProcToIndexMap map) {
  this->proc_to_index = map;
}

IndexToVarProcMap PqlEvaluator::GetIndexToVar() { return this->index_to_var; }

IndexToVarProcMap PqlEvaluator::GetIndexToProc() { return this->index_to_proc; }

VarProcToIndexMap PqlEvaluator::GetVarToIndex() { return this->var_to_index; }

VarProcToIndexMap PqlEvaluator::GetProcToIndex() { return this->proc_to_index; }

void PqlEvaluator::SetClauseFlag(bool clause_flag) {
  this->clause_flag_ = clause_flag;
}

bool PqlEvaluator::IsValidClause() { return clause_flag_; }

void PqlEvaluator::SetPKB(PKB pkb) { this->pkb_ = pkb; }

PKB PqlEvaluator::GetPKB() { return pkb_; }

void PqlEvaluator::SetQuery(PqlQuery query) { this->pql_query_ = query; }

PqlQuery PqlEvaluator::GetQuery() { return pql_query_; }

void PqlEvaluator::SetPqlResult(PqlResult pql_result) {
  this->pql_result_ = pql_result;
}

PqlResult PqlEvaluator::GetPqlResult() { return pql_result_; }
