#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
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
  SetQuery(*query);
  SetPKB(pkb);
  // Initialise new result table class
  PqlResult* pql_result = new PqlResult();
  SetPqlResult(*pql_result);
  // Default value should be true, until the clause returns a false
  SetClauseFlag(true);
  FinalResult final_results;

  PqlQuery user_query = GetQuery();

  // If there is no such that/pattern/with clause, then evaluator will use
  // GetSelectAllResult method
  if (user_query.GetClauses().empty()) {
    // Temp code, will implement tuple processing later
    QueryResultList get_all_result =
        GetSelectAllResult(user_query.GetSelections()[0].second);
    copy(get_all_result.begin(), get_all_result.end(),
         back_inserter(final_results));
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
      // Temp code, will implement tuple processing later
      Synonym select_syn = GetQuery().GetSelections()[0];

      QueryResultList other_clause_result = GetResultFromTable(select_syn);
      copy(other_clause_result.begin(), other_clause_result.end(),
           back_inserter(final_results));
    }

    // If BOOLEAN
    if (user_query.GetSelections().empty()) {
      if (IsValidClause()) {
        final_results.push_back("true");
      } else {
        final_results.push_back("false");
      }
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
      return pkb.GetAllProcName();
    case PqlDeclarationEntity::kVariable:
      // Get all variable name from PKB and store into
      // results list
      cout << "Select all variables." << endl;
      return pkb.GetAllVarName();
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
      return pkb.GetAllProcNameTwin();
    case PqlDeclarationEntity::kVariable:
      cout << "Select all twin variables." << endl;
      return pkb.GetAllVarNameTwin();
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

  switch (syn_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Is procedure?" << endl;
      if (pkb.IsProcName(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kVariable:
      cout << "Is variable?" << endl;
      if (pkb.IsVarName(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kAssign:
      cout << "Is assign?" << endl;
      if (pkb.IsAssignStmt(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kStmt:
      cout << "Is stmt?" << endl;
      if (pkb.IsStmtNum(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kRead:
      cout << "Is read?" << endl;
      if (pkb.IsReadStmt(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kPrint:
      cout << "Is print?" << endl;
      if (pkb.IsPrintStmt(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCall:
      cout << "Is call?" << endl;
      if (pkb.IsCallStmt(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCallName:
      cout << "Is call procname?" << endl;
      if (pkb.IsCalledProc(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kWhile:
      cout << "Is while?" << endl;
      if (pkb.IsWhileStmt(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kIf:
      cout << "Is if?" << endl;
      if (pkb.IsIfStmt(comparison_val)) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kConstant:
      cout << "Is const?" << endl;
      if (pkb.IsConstValue(stoi(comparison_val))) {
        result_list.push_back(comparison_val);
        StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kProgline:
      cout << "Is progline?" << endl;
      if (pkb.IsStmtNum(comparison_val)) {
        result_list.push_back(comparison_val);
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

  cout << "Evaluating Calls." << endl;

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
      result_list = pkb.GetCallee(left_name);
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

  cout << "Evaluating Calls*." << endl;

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

  cout << "Evaluating Next." << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb.IsNext(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not executed after" << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb.GetPrevious(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not executed after anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb.GetNext(left_name).empty()) {
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
      result_list = pkb.GetPrevious(right_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not executed after any progline" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllPrevious();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "progline is not executed before anything" << endl;
      } else {
        StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetNext(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not executed before any progline" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = pkb.GetAllNext();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " progline is not executed after anything" << endl;
      } else {
        StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllNextPairs();
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
                                 SuchthatParamType arrangement) {}

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
      if (!pkb.IsFollows(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not followed by " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If right is not follower
      if (pkb.GetFollowedBy(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not a follower " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If left is not followee
      if (pkb.GetFollows(left_name).empty()) {
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
      result_list = FilterResult(pkb.GetFollowedBy(right_name), left_type);
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
      result_list = FilterResult(pkb.GetFollows(left_name), right_type);
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
      if (!pkb.IsFollowsT(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not indirectly followed by " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If right is not follower
      if (pkb.GetFollowedByT(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not an indirect follower " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If left is not followee
      if (pkb.GetFollowsT(left_name).empty()) {
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
      result_list = FilterResult(pkb.GetFollowedByT(right_name), left_type);
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
      result_list = FilterResult(pkb.GetFollowsT(left_name), right_type);
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
      if (!pkb.IsParent(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not parent of " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If no parent
      if (pkb.GetParent(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " has no parent " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no child
      if (pkb.GetChild(left_name).empty()) {
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
      result_list = FilterResult(pkb.GetParent(right_name), left_type);
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
      result_list = FilterResult(pkb.GetChild(left_name), right_type);
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
      if (!pkb.IsParentT(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not indirect parent of " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If no parent
      if (pkb.GetParentT(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " has no indirect parent " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no child
      if (pkb.GetChildT(left_name).empty()) {
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
      result_list = FilterResult(pkb.GetParentT(right_name), left_type);
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
      result_list = FilterResult(pkb.GetChildT(left_name), right_type);
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
      if (!pkb.IsUsedByS(left_name, right_name)) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't use " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If nothing were used by this stmt
      if (pkb.GetUsedVarS(left_name).empty()) {
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
          FilterVariableResult(pkb.GetUsedVarS(left_name), right_type);
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
      if (!pkb.IsModifiedByS(left_name, right_name)) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no variables were modified by this stmt
      if (pkb.GetModifiedVarS(left_name).empty()) {
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
      result_list = pkb.GetModifiedVarS(left_name);
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

QueryResultList PqlEvaluator::FilterWithResult(
    QueryResultList unfiltered_result, PqlDeclarationEntity entity_type) {
  QueryResultList filtered_result;
  PKB pkb = GetPKB();

  switch (entity_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Filter Proc" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsProcName(iter)) {
          filtered_result.push_back(iter);
        }
      }
      break;
    case PqlDeclarationEntity::kVariable:
      cout << "Filter Var" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsVarName(iter)) {
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
        if (pkb.IsConstValue(stoi(iter))) {
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

QueryResultList PqlEvaluator::FilterResult(vector<string> unfiltered_result,
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
    string result = iter;

    if (pkb.GetStmtType(iter) == entity_type) {
      filtered_result.push_back(result);
    }
  }

  return filtered_result;
}

QueryResultList PqlEvaluator::FilterVariableResult(
    vector<string> unfiltered_result, PqlDeclarationEntity variable_type) {
  QueryResultList filtered_result;

  for (auto& iter : unfiltered_result) {
    if (variable_type == PqlDeclarationEntity::kConstant && isdigit(iter[0])) {
      filtered_result.push_back(iter);
    } else if (variable_type == PqlDeclarationEntity::kVariable &&
               !isdigit(iter[0])) {
      filtered_result.push_back(iter);
    }
  }

  return filtered_result;
}

QueryResultPairList PqlEvaluator::FilterPairResult(
    PqlResultFilterType filter_type,
    vector<pair<string, string>> unfiltered_pair_result,
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
    string left_result = iter.first;
    string right_result = iter.second;
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

/* Getters and Setters */

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
