#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_evaluate_suchthat.h"
#include "pql_evaluator.h"
#include "pql_extractor.h"
#include "pql_global.h"

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::vector;

PqlEvaluateSuchthat::PqlEvaluateSuchthat() {}

bool PqlEvaluateSuchthat::EvaluateSuchthatClause(PqlEvaluator* pql_eval,
                                                 PKB* pkb, PqlSuchthat suchthat,
                                                 PqlExtractor* pqle) {
  SetPKB(pkb);
  SetPqlExtractor(pqle);
  SetClauseFlag(true);
  SuchthatParamType arrangement =
      CheckSuchthatParamType(suchthat.GetParameters());

  switch (suchthat.GetType()) {
    case PqlSuchthatType::kFollows:
      EvaluateFollows(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kFollowsT:
      EvaluateFollowsT(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kParent:
      EvaluateParent(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kParentT:
      EvaluateParentT(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kUsesS:
      EvaluateUsesS(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kUsesP:
      EvaluateUsesP(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kModifiesS:
      EvaluateModifiesS(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kModifiesP:
      EvaluateModifiesP(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kCalls:
      EvaluateCalls(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kCallsT:
      EvaluateCallsT(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kNext:
      EvaluateNext(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kNextT:
      EvaluateNextT(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kAffects:
      EvaluateAffects(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kAffectsT:
      EvaluateAffectsT(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kAffectsB:
      EvaluateAffectsBip(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kAffectsBT:
      EvaluateAffectsBipT(pql_eval, suchthat, arrangement);
      break;
    case PqlSuchthatType::kDominates:
      EvaluateDominates(pql_eval, suchthat, arrangement);
      break;
  }

  return IsValidClause();
}

QueryResultSet PqlEvaluateSuchthat::GetSelectAllResult(
    PqlDeclarationEntity select_type) {
  QueryResultSet results;

  switch (select_type) {
    case PqlDeclarationEntity::kProcedure:
      // Get all procedures name from PKB and store into
      // results list
      cout << "Select all procedure." << endl;
      return pkb_->GetAllProcIndices();
    case PqlDeclarationEntity::kVariable:
      // Get all variable name from PKB and store into
      // results list
      cout << "Select all variables." << endl;
      return pkb_->GetAllVarIndices();
    case PqlDeclarationEntity::kAssign:
      // Get all statement number of statement which
      // contains assignment from PKB and store into results
      // list
      cout << "Select all assign statement." << endl;
      return pkb_->GetAllAssignStmt();
    case PqlDeclarationEntity::kStmt:
      // Get all stmt number from PKB and store into results
      // list
      cout << "Select all statement." << endl;
      return pkb_->GetAllStmt();
    case PqlDeclarationEntity::kRead:
      // Get all read stmt from PKB and store into results
      // list
      cout << "Select all read statement." << endl;
      return pkb_->GetAllReadStmt();
    case PqlDeclarationEntity::kPrint:
      // Get all print stmt from PKB and store into results
      // list
      cout << "Select all print statement." << endl;
      return pkb_->GetAllPrintStmt();
    case PqlDeclarationEntity::kCall:
      // Get all call stmt from PKB and store into results
      // list
      cout << "Select all call statement." << endl;
      return pkb_->GetAllCallStmt();
    case PqlDeclarationEntity::kCallName:
      // Get all call stmt from PKB and store into results
      // list
      cout << "Select all call.procName statement." << endl;
      return pkb_->GetAllCallee();
    case PqlDeclarationEntity::kWhile:
      // Get all while stmt from PKB and store into results
      // list
      cout << "Select all while statement." << endl;
      return pkb_->GetAllWhileStmt();
    case PqlDeclarationEntity::kIf:
      // Get all if stmt from PKB and store into results
      // list
      cout << "Select all if statement." << endl;
      return pkb_->GetAllIfStmt();
    case PqlDeclarationEntity::kConstant:
      // Get all constant from PKB and store into results
      // list
      cout << "Select all constants." << endl;
      return pkb_->GetAllConstValue();
    case PqlDeclarationEntity::kProgline:
      // Get all program line from PKB and store into
      // results list
      cout << "Select all program lines." << endl;
      return pkb_->GetAllStmt();
  }

  // Return empty result if nothing is found
  return results;
}

void PqlEvaluateSuchthat::EvaluateCalls(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
                                        SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;
  VarProcToIndexMap proc_to_index = pkb_->GetProcToIndexMapping();

  cout << "Evaluating Calls" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb_->IsCall(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not call by " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb_->GetProcIndex(right_name) != -1) {
        if (pkb_->GetCaller(proc_to_index[right_name]).empty()) {
          SetClauseFlag(false);
          cout << right_name << " is not called " << endl;
        }
      } else {
        SetClauseFlag(false);
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb_->GetProcIndex(left_name) != -1) {
        if (pkb_->GetCallee(proc_to_index[left_name]).empty()) {
          SetClauseFlag(false);
          cout << left_name << " is not caller " << endl;
        }
      } else {
        SetClauseFlag(false);
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasCallsRelationship()) {
        SetClauseFlag(false);
        cout << " no calls relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      if (pkb_->GetProcIndex(right_name) != -1) {
        result_list = pkb_->GetCaller(proc_to_index[right_name]);
        if (result_list.empty()) {
          SetClauseFlag(false);
          cout << right_name << " is not called " << endl;
        } else {
          pql_eval->StoreClauseResultInTable(result_list, left_name);
        }
      } else {
        SetClauseFlag(false);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb_->GetAllCaller();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "there are no call for procedure" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      if (pkb_->GetProcIndex(left_name) != -1) {
        result_list = pkb_->GetCallee(proc_to_index[left_name]);
        if (result_list.empty()) {
          SetClauseFlag(false);
          cout << left_name << " is not caller " << endl;
        } else {
          pql_eval->StoreClauseResultInTable(result_list, right_name);
        }
      } else {
        SetClauseFlag(false);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = pkb_->GetAllCallee();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " there are no procedures being called" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb_->GetAllCallPairs();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Call(proc,proc)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateCallsT(PqlEvaluator* pql_eval,
                                         PqlSuchthat suchthat,
                                         SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Calls*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb_->IsCallT(left_name, right_name)) {
        SetClauseFlag(false);
        cout << left_name << " not indirectly call by " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb_->GetCallerT(right_name).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly called " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb_->GetCalleeT(pkb_->GetProcIndex(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirect caller " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasCallsRelationship()) {
        SetClauseFlag(false);
        cout << " no calls relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = pkb_->GetCallerT(right_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly called " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb_->GetAllCaller();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "there are no call for procedure" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb_->GetCalleeT(pkb_->GetProcIndex(left_name));
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirect caller " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = pkb_->GetAllCallee();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " there are no procedures being called" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb_->GetAllCallTPairs();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Call*(proc,proc)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateNext(PqlEvaluator* pql_eval,
                                       PqlSuchthat suchthat,
                                       SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Next" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb_->IsNext(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not executed after" << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb_->GetPrevious(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not executed after anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb_->GetNext(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not executed before anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasNextRelationship()) {
        SetClauseFlag(false);
        cout << " no next relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pkb_->GetPrevious(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not executed after any left type" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllPrevious(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "left type is not executed before anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb_->GetNext(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not executed before any right type" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb_->GetAllNext(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right type is not executed before anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pkb_->GetAllNextPairs(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Next(left,right)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateNextT(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
                                        SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Next*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pqle_->IsNextT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not indirectly executed after" << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pqle_->GetPreviousT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly executed after anything "
             << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pqle_->GetNextT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirectly executed before anything "
             << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasNextRelationship()) {
        SetClauseFlag(false);
        cout << " no next relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pqle_->GetPreviousT(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not indirectly executed after any left type"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllPrevious(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "left type is not indirectly executed before anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pqle_->GetNextT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirectly executed before any right type"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb_->GetAllNext(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right type is not indirectly executed after anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      AffectsMap next_map;
      switch (left_type) {
        case PqlDeclarationEntity::kAssign:
          next_map = FilterNextTTable(pqle_->GetAssignNextTMap(), right_type);
          break;
        case PqlDeclarationEntity::kWhile:
          next_map = FilterNextTTable(pqle_->GetWhileNextTMap(), right_type);
          break;
        case PqlDeclarationEntity::kIf:
          next_map = FilterNextTTable(pqle_->GetIfNextTMap(), right_type);
          break;
        case PqlDeclarationEntity::kCall:
          next_map = FilterNextTTable(pqle_->GetCallNextTMap(), right_type);
          break;
        case PqlDeclarationEntity::kRead:
          next_map = FilterNextTTable(pqle_->GetReadNextTMap(), right_type);
          break;
        case PqlDeclarationEntity::kPrint:
          next_map = FilterNextTTable(pqle_->GetPrintNextTMap(), right_type);
          break;
        default:
          next_map = FilterNextTTable(pqle_->GetNextTMap(), right_type);
      }
      if (next_map.empty()) {
        SetClauseFlag(false);
        cout << " no map of Next*(lefttype,righttype)" << endl;
      }
      // two syn same
      else if ((left_name.compare(right_name)) == 0) {
        for (auto& before : next_map) {
          for (auto& next : before.second) {
            if (before.first == next) {
              result_pair_list.push_back(make_pair(next, next));
            }
          }
        }
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      } else {
        pql_eval->StoreClauseResultInTable(next_map, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateAffects(PqlEvaluator* pql_eval,
                                          PqlSuchthat suchthat,
                                          SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_set;

  cout << "Evaluating Affects" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pqle_->IsAffects(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pqle_->GetAffectedBy(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pqle_->GetAffects(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pqle_->HasAffectsRelationship()) {
        SetClauseFlag(false);
        cout << " no affects relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_set = pqle_->GetAffectedBy(stoi(right_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_set = pqle_->GetAllAffects();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << "left is not affecting anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymRight:
      result_set = pqle_->GetAffects(stoi(left_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_set = pqle_->GetAllAffectedBy();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << " right is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kTwoSynonym:
      AffectsMap result_map = pqle_->GetAffectsMap();
      QueryResultPairList pair_list;
      if (result_map.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Affects(left,right)" << endl;
      } else if ((left_name.compare(right_name)) == 0) {
        for (auto& affect : result_map) {
          for (auto& affected : affect.second) {
            if (affect.first == affected) {
              pair_list.push_back(make_pair(affected, affected));
            }
          }
        }
        pql_eval->StoreClauseResultInTable(pair_list, left_name, right_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_map, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateAffectsT(PqlEvaluator* pql_eval,
                                           PqlSuchthat suchthat,
                                           SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_set;

  cout << "Evaluating Affects*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pqle_->IsAffectsT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pqle_->GetAffectedByT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pqle_->GetAffectsT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pqle_->HasAffectsRelationship()) {
        SetClauseFlag(false);
        cout << " no affects relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_set = pqle_->GetAffectedByT(stoi(right_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_set = pqle_->GetAllAffectsT();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << "left is not affecting anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymRight:
      result_set = pqle_->GetAffectsT(stoi(left_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_set = pqle_->GetAllAffectedByT();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << " right is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kTwoSynonym:
      AffectsMap result_map = pqle_->GetAffectsTMap();
      QueryResultPairList pair_list;
      if (result_map.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Affects(left,right)" << endl;
      } else if ((left_name.compare(right_name)) == 0) {
        for (auto& affect : result_map) {
          for (auto& affected : affect.second) {
            if (affect.first == affected) {
              pair_list.push_back(make_pair(affected, affected));
            }
          }
        }
        pql_eval->StoreClauseResultInTable(pair_list, left_name, right_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_map, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateAffectsBip(PqlEvaluator* pql_eval,
                                             PqlSuchthat suchthat,
                                             SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_set;

  cout << "Evaluating AffectsBip" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pqle_->IsAffectsBip(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pqle_->GetAffectedByBip(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pqle_->GetAffectsBip(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pqle_->HasAffectsRelationship()) {
        SetClauseFlag(false);
        cout << " no affects relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_set = pqle_->GetAffectedByBip(stoi(right_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_set = pqle_->GetAllAffectsBipT();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << "left is not affecting anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymRight:
      result_set = pqle_->GetAffectsBip(stoi(left_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << left_name << " does not affect anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_set = pqle_->GetAllAffectedByBipT();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << " right is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kTwoSynonym:
      AffectsMap result_map = pqle_->GetAffectsBipMap();
      QueryResultPairList pair_list;
      if (result_map.empty()) {
        SetClauseFlag(false);
        cout << " no pair of AffectsBip(left,right)" << endl;
      } else if ((left_name.compare(right_name)) == 0) {
        for (auto& affect : result_map) {
          for (auto& affected : affect.second) {
            if (affect.first == affected) {
              pair_list.push_back(make_pair(affected, affected));
            }
          }
        }
        pql_eval->StoreClauseResultInTable(pair_list, left_name, right_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_map, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateAffectsBipT(PqlEvaluator* pql_eval,
                                              PqlSuchthat suchthat,
                                              SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_set;

  cout << "Evaluating AffectsBip*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pqle_->IsAffectsBipT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pqle_->GetAffectedByBipT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pqle_->GetAffectsBipT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not affecting anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pqle_->HasAffectsRelationship()) {
        SetClauseFlag(false);
        cout << " no affects relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_set = pqle_->GetAffectedByBipT(stoi(right_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_set = pqle_->GetAllAffectsBipT();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << "left is not affecting anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, left_name);
      }
      return;
    case kOneSynonymRight:
      result_set = pqle_->GetAffectsBipT(stoi(left_name));
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << left_name << " does not affect anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_set = pqle_->GetAllAffectedByBipT();
      if (result_set.empty()) {
        SetClauseFlag(false);
        cout << " right is not affected by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_set, right_name);
      }
      return;
    case kTwoSynonym:
      AffectsMap result_map = pqle_->GetAffectsBipTMap();
      QueryResultPairList pair_list;
      if (result_map.empty()) {
        SetClauseFlag(false);
        cout << " no pair of AffectsBip(left,right)" << endl;
      } else if ((left_name.compare(right_name)) == 0) {
        for (auto& affect : result_map) {
          for (auto& affected : affect.second) {
            if (affect.first == affected) {
              pair_list.push_back(make_pair(affected, affected));
            }
          }
        }
        pql_eval->StoreClauseResultInTable(pair_list, left_name, right_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_map, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateDominates(PqlEvaluator* pql_eval,
                                            PqlSuchthat suchthat,
                                            SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Dominates" << endl;

  switch (arrangement) {
    case kNoSynonym:
      if (!pkb_->IsDominates(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " does not dominate " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      if (pkb_->GetDominating(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not dominated by anything " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      if (pkb_->GetDominated(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not dominating anything" << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasDominatesRelationship()) {
        SetClauseFlag(false);
        cout << " no dominates relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pkb_->GetDominating(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not dominated by left type" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllDominating(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "left type is not dominating anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list =
          FilterResult(pkb_->GetDominated(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not dominating right type" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb_->GetAllDominated(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right type is not dominated by anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(
          kFilterBoth, pkb_->GetAllDominatesPairs(), left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Dominates(left,right)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateFollows(PqlEvaluator* pql_eval,
                                          PqlSuchthat suchthat,
                                          SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Follows." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT followed by right
      if (!pkb_->IsFollows(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not followed by " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If right is not follower
      if (pkb_->GetFollowedBy(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not a follower " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If left is not followee
      if (pkb_->GetFollows(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not a followee " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasFollowsRelationship()) {
        SetClauseFlag(false);
        cout << " no follow relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pkb_->GetFollowedBy(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No followee for this type " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllFollowedBy(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have any follower" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb_->GetFollows(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No follower of this type " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb_->GetAllFollows(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt is not following anyone" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      // Filter left and right
      result_pair_list = FilterPairResult(
          kFilterBoth, pkb_->GetAllFollowsPair(), left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of (lefttype,righttype)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateFollowsT(PqlEvaluator* pql_eval,
                                           PqlSuchthat suchthat,
                                           SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Follows*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT followed by right
      if (!pkb_->IsFollowsT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not indirectly followed by " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If right is not follower
      if (pkb_->GetFollowedByT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " is not an indirect follower " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If left is not followee
      if (pkb_->GetFollowsT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not an indirect followee " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasFollowsRelationship()) {
        SetClauseFlag(false);
        cout << " no follow relationship found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list =
          FilterResult(pkb_->GetFollowedByT(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No indirect followee for this type " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllFollowedBy(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have any follower" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list =
          FilterResult(pkb_->GetFollowsT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No indirect follower of this type " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb_->GetAllFollows(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt is not following anyone" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      // Filter left and right
      result_pair_list = FilterPairResult(
          kFilterBoth, pkb_->GetAllFollowsTPair(), left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no indirect pair of (lefttype,righttype)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateParent(PqlEvaluator* pql_eval,
                                         PqlSuchthat suchthat,
                                         SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Parent." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT parent of right
      if (!pkb_->IsParent(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not parent of " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If no parent
      if (pkb_->GetParent(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " has no parent " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no child
      if (pkb_->GetChild(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " has no child " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasParentRelationship()) {
        SetClauseFlag(false);
        cout << " no parent/child found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = FilterResult(pkb_->GetParent(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have " << right_name << " as child"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllParent(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt is not a parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb_->GetChild(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have stmt " << left_name << " as parent"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb_->GetAllChild(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have any parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pkb_->GetAllParentPair(),
                                          left_type, right_type);
      // Filter left and right
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of (lefttype,righttype)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateParentT(PqlEvaluator* pql_eval,
                                          PqlSuchthat suchthat,
                                          SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating Parent*" << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If left is NOT parent of right
      if (!pkb_->IsParentT(stoi(left_name), stoi(right_name))) {
        SetClauseFlag(false);
        cout << left_name << " not indirect parent of " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreLeft:
      // If no parent
      if (pkb_->GetParentT(stoi(right_name)).empty()) {
        SetClauseFlag(false);
        cout << right_name << " has no indirect parent " << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no child
      if (pkb_->GetChildT(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << left_name << " has no indirect child " << endl;
      }
      return;
    case kNoSynonymUnderscoreBoth:
      if (!pkb_->HasParentRelationship()) {
        SetClauseFlag(false);
        cout << " no parent/child found " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = FilterResult(pkb_->GetParentT(stoi(right_name)), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have " << right_name
             << " as indirect child" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllParent(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt is not a parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb_->GetChildT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have stmt " << left_name
             << " as indirect parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb_->GetAllChild(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have any parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(
          kFilterBoth, pkb_->GetAllParentTPair(), left_type, right_type);
      // Filter left and right
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no indirect pair of (lefttype,righttype)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateUsesS(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
                                        SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating UsesS." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If stmt (left) doesnt use variable (right)
      if (!pkb_->IsUsedByS(stoi(left_name), right_name)) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't use " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If nothing were used by this stmt
      if (pkb_->GetUsedVarS(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't use any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      // If no stmt of left syn entity type uses the right variable
      result_list = FilterResult(pkb_->GetUsingStmt(right_name), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt use right variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllUsingStmt(), left_type);
      // If no stmt of left syn entity type uses the any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt use any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list =
          FilterVariableResult(pkb_->GetUsedVarS(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesnt use any variable of this type"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterLeft, pkb_->GetAllUsesPairS(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt use any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateUsesP(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
                                        SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating UsesP." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If proc (left) doesnt use variable (right)
      if (!pkb_->IsUsedByP(left_name, right_name)) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesn't use " << right_name << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If nothing were used by this proc
      if (pkb_->GetUsedVarP(left_name).empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesn't use any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      // If no proc uses the right variable
      result_list = pkb_->GetUsingProc(right_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc doesnt use right variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb_->GetAllUsingProc();
      // If no proc of left syn entity type uses the any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc of left type doesnt use any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list =
          FilterVariableResult(pkb_->GetUsedVarP(left_name), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesnt use any variable of this type"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb_->GetAllUsesPairP();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "Procedure doesnt use any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateModifiesS(PqlEvaluator* pql_eval,
                                            PqlSuchthat suchthat,
                                            SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating ModifiesS." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If stmt (left) doesnt modify variable (right)
      if (!pkb_->IsModifiedByS(stoi(left_name), right_name)) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no variables were modified by this stmt
      if (pkb_->GetModifiedVarS(stoi(left_name)).empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = FilterResult(pkb_->GetModifyingS(right_name), left_type);
      // If no stmt of left syn entity type modifies the right variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt modify right variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb_->GetAllModifyingS(), left_type);
      // If no stmt of left syn entity type modifies any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb_->GetModifiedVarS(stoi(left_name));
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(
          kFilterLeft, pkb_->GetAllModifiesPairS(), left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "No stmt/variable pair found" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateModifiesP(PqlEvaluator* pql_eval,
                                            PqlSuchthat suchthat,
                                            SuchthatParamType arrangement) {
  // Getting parameter of such that
  Parameters such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultSet result_list;
  QueryResultPairSet result_pair_list;

  cout << "Evaluating ModifiesP." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If proc (left) doesnt modify variable (right)
      if (!pkb_->IsModifiedByP(left_name, right_name)) {
        SetClauseFlag(false);
        cout << "Procedure " << left_name << " doesn't modify " << right_name
             << endl;
      }
      return;
    case kNoSynonymUnderscoreRight:
      // If no variables were modified by this proc
      if (pkb_->GetModifiedVarP(left_name).empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify any variable " << endl;
      }
      return;
    case kOneSynonymLeft:
      result_list = pkb_->GetModifyingP(right_name);
      // If no proc modifies the right variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "No proc modify the right variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb_->GetAllModifyingP();
      // If no proc modifies any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb_->GetModifiedVarP(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb_->GetAllModifiesPairP();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "No proc/variable pair found" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name,
                                           right_name);
      }
      return;
  }
}

QueryResultSet PqlEvaluateSuchthat::FilterResult(
    QueryResultSet unfiltered_result, PqlDeclarationEntity entity_type) {
  QueryResultSet filtered_result;

  // If its of type stmt (not assign, if, while etc) just return the list,
  // nothing to filter
  if (entity_type == PqlDeclarationEntity::kStmt ||
      entity_type == PqlDeclarationEntity::kProgline) {
    return unfiltered_result;
  }

  for (auto& iter : unfiltered_result) {
    int result = iter;

    if (pkb_->GetStmtType(result) == entity_type) {
      filtered_result.emplace(result);
    }
  }

  return filtered_result;
}

QueryResultSet PqlEvaluateSuchthat::FilterVariableResult(
    QueryResultSet unfiltered_result, PqlDeclarationEntity variable_type) {
  QueryResultSet filtered_result;

  for (auto& iter : unfiltered_result) {
    if (variable_type == PqlDeclarationEntity::kConstant &&
        pkb_->IsConstValue(iter)) {
      filtered_result.emplace(iter);
    } else if (variable_type == PqlDeclarationEntity::kVariable &&
               pkb_->IsVarIndex(iter)) {
      filtered_result.emplace(iter);
    }
  }

  return filtered_result;
}

QueryResultPairSet PqlEvaluateSuchthat::FilterPairResult(
    PqlResultFilterType filter_type, QueryResultPairSet unfiltered_pair_result,
    PqlDeclarationEntity left_type, PqlDeclarationEntity right_type) {
  QueryResultPairSet filtered_result;

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
        if (pkb_->GetStmtType(left_result) == left_type) {
          filtered_result.emplace(iter);
        }
        break;
      case kFilterRight:
        if (pkb_->GetStmtType(right_result) == right_type) {
          filtered_result.emplace(iter);
        }
        break;
      case kFilterBoth:
        if (pkb_->GetStmtType(left_result) == left_type &&
            pkb_->GetStmtType(right_result) == right_type) {
          filtered_result.emplace(iter);
        }
        break;
    }
  }

  return filtered_result;
}

NextTMap PqlEvaluateSuchthat::FilterNextTTable(
    NextTMap result_map, PqlDeclarationEntity entity_type) {
  // If its of type stmt (not assign, if, while etc) just return the list,
  // nothing to filter
  if (entity_type == PqlDeclarationEntity::kStmt ||
      entity_type == PqlDeclarationEntity::kProgline) {
    return result_map;
  }

  for (auto& kv : result_map) {
    StmtNumSet* stmts = &kv.second;
    auto iterator = stmts->begin();
    while (iterator != stmts->end()) {
      if (pkb_->GetStmtType(*iterator) != entity_type) {
        iterator = stmts->erase(iterator);
      } else {
        iterator++;
      }
    }
  }

  return result_map;
}

SuchthatParamType PqlEvaluateSuchthat::CheckSuchthatParamType(
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

void PqlEvaluateSuchthat::SetClauseFlag(bool clause_flag) {
  this->clause_flag_ = clause_flag;
}

bool PqlEvaluateSuchthat::IsValidClause() { return clause_flag_; }

void PqlEvaluateSuchthat::SetPKB(PKB* pkb) { this->pkb_ = pkb; }

void PqlEvaluateSuchthat::SetPqlExtractor(PqlExtractor* pqle) {
  this->pqle_ = pqle;
}