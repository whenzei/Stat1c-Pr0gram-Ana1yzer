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
                                                 PKB pkb,
                                                 PqlSuchthat suchthat) {
  SetPKB(pkb);
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
  }

  return IsValidClause();
}

QueryResultList PqlEvaluateSuchthat::GetSelectAllResult(
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

void PqlEvaluateSuchthat::EvaluateCalls(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllCaller();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "there are no call for procedure" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetCallee(stoi(left_name));
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not caller " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = pkb.GetAllCallee();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " there are no procedures being called" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllCallPairs();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Call(proc,proc)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateCallsT(PqlEvaluator* pql_eval,
                                         PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllCaller();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "there are no call for procedure" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetCalleeT(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirect caller " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = pkb.GetAllCallee();
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " there are no procedures being called" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllCallTPairs();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Call*(proc,proc)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateNext(PqlEvaluator* pql_eval,
                                       PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllPrevious(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "left type is not executed before anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetNext(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not executed before any right type" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllNext(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right type is not executed before anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pkb.GetAllNextPairs(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Next(left,right)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateNextT(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllPrevious(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "left type is not indirectly executed before anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pqle.GetNextT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << left_name << " is not indirectly executed before any right type"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllNext(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right type is not indirectly executed after anything" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterBoth, pqle.GetAllNextTPairs(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " no pair of Next*(left,right)" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateAffects(PqlEvaluator* pql_eval,
                                          PqlSuchthat suchthat,
                                          SuchthatParamType arrangement) {}

void PqlEvaluateSuchthat::EvaluateAffectsT(PqlEvaluator* pql_eval,
                                           PqlSuchthat suchthat,
                                           SuchthatParamType arrangement) {}

void PqlEvaluateSuchthat::EvaluateFollows(PqlEvaluator* pql_eval,
                                          PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllFollowedBy(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have any follower" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetFollows(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No follower of this type " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllFollows(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt is not following anyone" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
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
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateFollowsT(PqlEvaluator* pql_eval,
                                           PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllFollowedBy(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt does not have any follower" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetFollowsT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No indirect follower of this type " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllFollows(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt is not following anyone" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
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
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateParent(PqlEvaluator* pql_eval,
                                         PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllParent(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt is not a parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetChild(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have stmt " << left_name << " as parent"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllChild(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have any parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
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
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateParentT(PqlEvaluator* pql_eval,
                                          PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllParent(), left_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " left stmt is not a parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = FilterResult(pkb.GetChildT(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have stmt " << left_name
             << " as indirect parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kOneSynonymRightUnderscoreLeft:
      result_list = FilterResult(pkb.GetAllChild(), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " right stmt does not have any parent" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
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
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateUsesS(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllUsingStmt(), left_type);
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
          FilterVariableResult(pkb.GetUsedVarS(stoi(left_name)), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesnt use any variable of this type"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(kFilterLeft, pkb.GetAllUsesPairS(),
                                          left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt use any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateUsesP(PqlEvaluator* pql_eval,
                                        PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllUsingProc();
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
          FilterVariableResult(pkb.GetUsedVarP(left_name), right_type);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesnt use any variable of this type"
             << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllUsesPairP();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "Procedure doesnt use any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateModifiesS(PqlEvaluator* pql_eval,
                                            PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = FilterResult(pkb.GetAllModifyingS(), left_type);
      // If no stmt of left syn entity type modifies any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt of left type doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetModifiedVarS(stoi(left_name));
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Stmt " << left_name << " doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = FilterPairResult(
          kFilterLeft, pkb.GetAllModifiesPairS(), left_type, right_type);
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "No stmt/variable pair found" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

void PqlEvaluateSuchthat::EvaluateModifiesP(PqlEvaluator* pql_eval,
                                            PqlSuchthat suchthat,
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
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymLeftUnderscoreRight:
      result_list = pkb.GetAllModifyingP();
      // If no proc modifies any variable
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, left_name);
      }
      return;
    case kOneSynonymRight:
      result_list = pkb.GetModifiedVarP(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << "Proc " << left_name << " doesnt modify any variable" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, right_name);
      }
      return;
    case kTwoSynonym:
      result_pair_list = pkb.GetAllModifiesPairP();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << "No proc/variable pair found" << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, left_name, right_name);
      }
      return;
  }
}

QueryResultList PqlEvaluateSuchthat::FilterResult(
    QueryResultList unfiltered_result, PqlDeclarationEntity entity_type) {
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

QueryResultList PqlEvaluateSuchthat::FilterVariableResult(
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

QueryResultPairList PqlEvaluateSuchthat::FilterPairResult(
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

void PqlEvaluateSuchthat::SetPKB(PKB pkb) { this->pkb_ = pkb; }

PKB PqlEvaluateSuchthat::GetPKB() { return pkb_; }