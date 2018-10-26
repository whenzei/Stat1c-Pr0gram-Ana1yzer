#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_evaluate_with.h"
#include "pql_evaluator.h"
#include "pql_global.h"

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::vector;

PqlEvaluateWith::PqlEvaluateWith() {}

bool PqlEvaluateWith::EvaluateWithClause(PqlEvaluator* pql_eval, PKB pkb,
                                         PqlWith with) {
  SetPKB(pkb);
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
      break;
    case kWithOneSynonymLeft:
      EvaluateWithOneSynonym(pql_eval, left_param, right_name);
      break;
    case kWithOneSynonymRight:
      EvaluateWithOneSynonym(pql_eval, right_param, left_name);
      break;
    case kWithTwoSynonym:
      EvaluateWithTwoSynonym(pql_eval, left_param, right_param);
      break;
  }

  return IsValidClause();
}

QueryResultList PqlEvaluateWith::GetSelectAllResult(
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

QueryResultPairList PqlEvaluateWith::GetSelectAllTwinResult(
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

void PqlEvaluateWith::EvaluateWithTwoSynonym(PqlEvaluator* pql_eval,
                                             Synonym left_param,
                                             Synonym right_param) {
  cout << "Evaluating with two synonym" << endl;

  PKB pkb = GetPKB();
  // Getting parameter of with
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_left, result_right;
  QueryResultPairList filtered_result;

  // If same type, get twin pair
  if (left_type == right_type) {
    pql_eval->StoreClauseResultInTable(GetSelectAllTwinResult(left_type),
                                       left_name, right_name);
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
      pql_eval->StoreClauseResultInTable(filtered_result, left_name, right_name);
    }
  }
}

void PqlEvaluateWith::EvaluateWithOneSynonym(PqlEvaluator* pql_eval,
                                             Synonym with_syn,
                                             string comparison_val) {
  cout << "Evaluating with one synonym" << endl;

  PKB pkb = GetPKB();
  // Getting parameter of with
  string syn_name = with_syn.first;
  PqlDeclarationEntity syn_type = with_syn.second;
  QueryResultList result_list;
  VarProcToIndexMap proc_to_index = pkb.GetProcToIndexMapping();	  
  VarProcToIndexMap var_to_index = pkb.GetVarToIndexMapping();

  switch (syn_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Is procedure?" << endl;
      if (proc_to_index.find(comparison_val) != proc_to_index.end()) {
        if (pkb.IsProcIndex(proc_to_index[comparison_val])) {
          result_list.push_back(proc_to_index[comparison_val]);
          pql_eval->StoreClauseResultInTable(result_list, syn_name);
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
          pql_eval->StoreClauseResultInTable(result_list, syn_name);
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
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kStmt:
      cout << "Is stmt?" << endl;
      if (pkb.IsStmtNum(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kRead:
      cout << "Is read?" << endl;
      if (pkb.IsReadStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kPrint:
      cout << "Is print?" << endl;
      if (pkb.IsPrintStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCall:
      cout << "Is call?" << endl;
      if (pkb.IsCallStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCallName:
      cout << "Is call procname?" << endl;
      if (proc_to_index.find(comparison_val) != proc_to_index.end()) {
        if (pkb.IsCalledProc(proc_to_index[comparison_val])) {
          result_list.push_back(proc_to_index[comparison_val]);
          pql_eval->StoreClauseResultInTable(result_list, syn_name);
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
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kIf:
      cout << "Is if?" << endl;
      if (pkb.IsIfStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kConstant:
      cout << "Is const?" << endl;
      if (pkb.IsConstValue(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kProgline:
      cout << "Is progline?" << endl;
      if (pkb.IsStmtNum(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
  }
}

QueryResultPairList PqlEvaluateWith::FilterWithResult(
    QueryResultList unfiltered_result, PqlDeclarationEntity entity_type) {
  QueryResultPairList filtered_result;
  PKB pkb = GetPKB();

  switch (entity_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Filter Proc" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsProcIndex(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kVariable:
      cout << "Filter Var" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsVarIndex(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kAssign:
      cout << "Filter Assign" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsAssignStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kStmt:
      cout << "Filter Stmt" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsStmtNum(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kRead:
      cout << "Filter Read" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsReadStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kPrint:
      cout << "Filter Print" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsPrintStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kCall:
      cout << "Filter Call" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsCallStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kCallName:
      cout << "Filter CallName" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsCalledProc(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kWhile:
      cout << "Filter While" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsWhileStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kIf:
      cout << "Filter If" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsIfStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kConstant:
      cout << "Filter Constant" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsConstValue(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kProgline:
      cout << "Filter Progline" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb.IsStmtNum(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
  }

  return filtered_result;
}

WithParamType PqlEvaluateWith::CheckWithParamType(Parameters with_param) {
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

/* Getters and Setters */

void PqlEvaluateWith::SetClauseFlag(bool clause_flag) {
  this->clause_flag_ = clause_flag;
}

bool PqlEvaluateWith::IsValidClause() { return clause_flag_; }

void PqlEvaluateWith::SetPKB(PKB pkb) { this->pkb_ = pkb; }

PKB PqlEvaluateWith::GetPKB() { return pkb_; }
