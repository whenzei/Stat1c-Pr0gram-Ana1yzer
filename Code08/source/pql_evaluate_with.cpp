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

bool PqlEvaluateWith::EvaluateWithClause(PqlEvaluator* pql_eval, PKB* pkb,
                                         PqlWith with) {
  SetPKB(pkb);
  SetClauseFlag(true);
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
  QueryResultList results;

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
    case PqlDeclarationEntity::kReadName:
      // Get all read stmt var from PKB and store into results
      // list
      cout << "Select all read var." << endl;
      return pkb_->GetAllReadVar();
    case PqlDeclarationEntity::kPrint:
      // Get all print stmt from PKB and store into results
      // list
      cout << "Select all print statement." << endl;
      return pkb_->GetAllPrintStmt();
    case PqlDeclarationEntity::kPrintName:
      // Get all print var from PKB and store into results
      // list
      cout << "Select all print var." << endl;
      return pkb_->GetAllPrintVar();
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

QueryResultPairList PqlEvaluateWith::GetSelectAllTwinResult(
    PqlDeclarationEntity select_type) {
  QueryResultPairList results;

  switch (select_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Select all twin procedure." << endl;
      return pkb_->GetAllProcIndexTwin();
    case PqlDeclarationEntity::kVariable:
      cout << "Select all twin variables." << endl;
      return pkb_->GetAllVarIndexTwin();
    case PqlDeclarationEntity::kAssign:
      cout << "Select all twin assign statement." << endl;
      return pkb_->GetAllAssignStmtTwin();
    case PqlDeclarationEntity::kStmt:
      cout << "Select all twin statement." << endl;
      return pkb_->GetAllStmtTwin();
    case PqlDeclarationEntity::kRead:
      cout << "Select all twin read statement." << endl;
      return pkb_->GetAllReadStmtTwin();
    case PqlDeclarationEntity::kReadName:
      cout << "Select all twin read var." << endl;
      return pkb_->GetAllReadStmtTwin();
    case PqlDeclarationEntity::kPrint:
      cout << "Select all twin print statement." << endl;
      return pkb_->GetAllPrintStmtTwin();
    case PqlDeclarationEntity::kPrintName:
      cout << "Select all twin print var." << endl;
      return pkb_->GetAllPrintStmtTwin();
    case PqlDeclarationEntity::kCall:
      cout << "Select all twin call statement." << endl;
      return pkb_->GetAllCallStmtTwin();
    case PqlDeclarationEntity::kCallName:
      cout << "Select all twin call.procName statement." << endl;
      return pkb_->GetAllCallStmtTwin();
    case PqlDeclarationEntity::kWhile:
      cout << "Select all twin while statement." << endl;
      return pkb_->GetAllWhileStmtTwin();
    case PqlDeclarationEntity::kIf:
      cout << "Select all twin if statement." << endl;
      return pkb_->GetAllIfStmtTwin();
    case PqlDeclarationEntity::kConstant:
      cout << "Select all twin constants." << endl;
      return pkb_->GetAllConstValueTwin();
    case PqlDeclarationEntity::kProgline:
      cout << "Select all twin program lines." << endl;
      return pkb_->GetAllStmtTwin();
  }

  // Return empty result if nothing is found
  return results;
}

void PqlEvaluateWith::EvaluateWithTwoSynonym(PqlEvaluator* pql_eval,
                                             Synonym left_param,
                                             Synonym right_param) {
  cout << "Evaluating with two synonym" << endl;

  // Getting parameter of with
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_left, result_right;
  QueryResultPairList filtered_result;

  // Change the 0c/0r/0p to c/r/p for now
  if (left_type == PqlDeclarationEntity::kCallName ||
      left_type == PqlDeclarationEntity::kReadName ||
      left_type == PqlDeclarationEntity::kPrintName) {
    left_name = left_name.substr(1);
  } else if (right_type == PqlDeclarationEntity::kCallName ||
             right_type == PqlDeclarationEntity::kReadName ||
             right_type == PqlDeclarationEntity::kPrintName) {
    right_name = right_name.substr(1);
  }

  // If same type, get twin pair
  if (left_type == right_type) {
    pql_eval->StoreClauseResultInTable(GetSelectAllTwinResult(left_type),
                                       left_name, right_name);
  } else {
    result_left = GetSelectAllResult(left_type);
    result_right = GetSelectAllResult(right_type);

    // Check size to ensure fastest filter
    if (result_left.size() < result_right.size()) {
      if (right_type == PqlDeclarationEntity::kVariable ||
          right_type == PqlDeclarationEntity::kProcedure) {
        filtered_result = FilterWithLeft(result_right, left_type);
      } else {
        filtered_result = FilterWithRight(result_left, right_type);
      }
    } else {
      if (left_type == PqlDeclarationEntity::kVariable ||
          left_type == PqlDeclarationEntity::kProcedure) {
        filtered_result = FilterWithRight(result_left, right_type);
      } else {
        filtered_result = FilterWithLeft(result_right, left_type);
      }
    }

    if (filtered_result.empty()) {
      SetClauseFlag(false);
    } else {
      // Store both synonym in result table
      pql_eval->StoreClauseResultInTable(filtered_result, left_name,
                                         right_name);
    }
  }
}

void PqlEvaluateWith::EvaluateWithOneSynonym(PqlEvaluator* pql_eval,
                                             Synonym with_syn,
                                             string comparison_val) {
  cout << "Evaluating with one synonym" << endl;

  // Getting parameter of with
  string syn_name = with_syn.first;
  PqlDeclarationEntity syn_type = with_syn.second;
  QueryResultList result_list;
  VarProcToIndexMap proc_to_index = pkb_->GetProcToIndexMapping();
  VarProcToIndexMap var_to_index = pkb_->GetVarToIndexMapping();

  // Change the 0c/0r/0p to c/r/p for now
  if (syn_type == PqlDeclarationEntity::kCallName ||
      syn_type == PqlDeclarationEntity::kReadName ||
      syn_type == PqlDeclarationEntity::kPrintName) {
    syn_name = syn_name.substr(1);
  }

  switch (syn_type) {
    case PqlDeclarationEntity::kProcedure:
      cout << "Is procedure?" << endl;
      if (proc_to_index.find(comparison_val) != proc_to_index.end()) {
        if (pkb_->IsProcIndex(proc_to_index[comparison_val])) {
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
        if (pkb_->IsVarIndex(var_to_index[comparison_val])) {
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
      if (pkb_->IsAssignStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kStmt:
      cout << "Is stmt?" << endl;
      if (pkb_->IsStmtNum(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kRead:
      cout << "Is read?" << endl;
      if (pkb_->IsReadStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kReadName:
      cout << "Is read var?" << endl;
      if (pkb_->IsReadVar(comparison_val)) {
        // Get the related print statment
        result_list = pkb_->GetReadStmt(comparison_val);
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kPrint:
      cout << "Is print?" << endl;
      if (pkb_->IsPrintStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kPrintName:
      cout << "Is print var?" << endl;
      if (pkb_->IsPrintVar(comparison_val)) {
        // Get the related print statment
        result_list = pkb_->GetPrintStmt(comparison_val);
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCall:
      cout << "Is call?" << endl;
      if (pkb_->IsCallStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCallName:
      cout << "Is call procname?" << endl;
      if (proc_to_index.find(comparison_val) != proc_to_index.end()) {
        if (pkb_->IsCalledProc(proc_to_index[comparison_val])) {
          // Get all related call stmt
          result_list = pkb_->GetCallingStmts(proc_to_index[comparison_val]);
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
      if (pkb_->IsWhileStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kIf:
      cout << "Is if?" << endl;
      if (pkb_->IsIfStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kConstant:
      cout << "Is const?" << endl;
      if (pkb_->IsConstValue(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kProgline:
      cout << "Is progline?" << endl;
      if (pkb_->IsStmtNum(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
  }
}

QueryResultPairList PqlEvaluateWith::FilterWithLeft(
    QueryResultList unfiltered_result, PqlDeclarationEntity entity_type) {
  QueryResultPairList filtered_result;
  IndexToVarProcMap index_to_var = pkb_->GetIndexToVarMapping();

  switch (entity_type) {
    case PqlDeclarationEntity::kAssign:
      cout << "Filter Assign" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsAssignStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kStmt:
      cout << "Filter Stmt" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsStmtNum(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kRead:
      cout << "Filter Read" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsReadStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kReadName:
      cout << "Filter Read var" << endl;
      for (auto& iter : unfiltered_result) {
        if (index_to_var.find(iter) != index_to_var.end()) {
          if (pkb_->IsReadVar(index_to_var[iter])) {
            QueryResultList result_list = pkb_->GetReadStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(result, iter));
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kPrint:
      cout << "Filter Print" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsPrintStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kPrintName:
      cout << "Filter Print var" << endl;
      for (auto& iter : unfiltered_result) {
        if (index_to_var.find(iter) != index_to_var.end()) {
          if (pkb_->IsPrintVar(index_to_var[iter])) {
            QueryResultList result_list =
                pkb_->GetPrintStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(result, iter));
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kCall:
      cout << "Filter Call" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsCallStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kCallName:
      cout << "Filter Call proc" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsCalledProc(iter)) {
          QueryResultList result_list = pkb_->GetCallingStmts(iter);
          for (auto& result : result_list) {
            filtered_result.push_back(std::make_pair(result, iter));
          }
        }
      }
      break;
    case PqlDeclarationEntity::kWhile:
      cout << "Filter While" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsWhileStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kIf:
      cout << "Filter If" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsIfStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kConstant:
      cout << "Filter Constant" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsConstValue(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kProgline:
      cout << "Filter Progline" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsStmtNum(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
  }

  return filtered_result;
}

QueryResultPairList PqlEvaluateWith::FilterWithRight(
    QueryResultList unfiltered_result, PqlDeclarationEntity entity_type) {
  QueryResultPairList filtered_result;
  IndexToVarProcMap index_to_var = pkb_->GetIndexToVarMapping();

  switch (entity_type) {
    case PqlDeclarationEntity::kAssign:
      cout << "Filter Assign" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsAssignStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kStmt:
      cout << "Filter Stmt" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsStmtNum(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kRead:
      cout << "Filter Read" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsReadStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kReadName:
      cout << "Filter Read var" << endl;
      for (auto& iter : unfiltered_result) {
        if (index_to_var.find(iter) != index_to_var.end()) {
          if (pkb_->IsReadVar(index_to_var[iter])) {
            QueryResultList result_list = pkb_->GetReadStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(iter, result));
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kPrint:
      cout << "Filter Print" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsPrintStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kPrintName:
      cout << "Filter Print var" << endl;
      for (auto& iter : unfiltered_result) {
        if (index_to_var.find(iter) != index_to_var.end()) {
          if (pkb_->IsPrintVar(index_to_var[iter])) {
            QueryResultList result_list =
                pkb_->GetPrintStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(iter, result));
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kCall:
      cout << "Filter Call" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsCallStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kCallName:
      cout << "Filter Call proc" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsCalledProc(iter)) {
          QueryResultList result_list = pkb_->GetCallingStmts(iter);
          for (auto& result : result_list) {
            filtered_result.push_back(std::make_pair(iter, result));
          }
        }
      }
      break;
    case PqlDeclarationEntity::kWhile:
      cout << "Filter While" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsWhileStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kIf:
      cout << "Filter If" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsIfStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kConstant:
      cout << "Filter Constant" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsConstValue(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kProgline:
      cout << "Filter Progline" << endl;
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsStmtNum(iter)) {
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

void PqlEvaluateWith::SetPKB(PKB* pkb) { this->pkb_ = pkb; }
