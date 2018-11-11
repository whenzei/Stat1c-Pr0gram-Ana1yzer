#include "pql_evaluate_with.h"
#include "debug.h"
#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"

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
      Debug::PrintLn(Debug::kLog, "Select all procedure");
      return pkb_->GetAllProcIndices();
    case PqlDeclarationEntity::kVariable:
      // Get all variable name from PKB and store into
      // results list
      Debug::PrintLn(Debug::kLog, "Select all variables");
      return pkb_->GetAllVarIndices();
    case PqlDeclarationEntity::kAssign:
      // Get all statement number of statement which
      // contains assignment from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all assign statements");
      return pkb_->GetAllAssignStmt();
    case PqlDeclarationEntity::kStmt:
      // Get all stmt number from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all statements");
      return pkb_->GetAllStmt();
    case PqlDeclarationEntity::kRead:
      // Get all read stmt from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all read statements");
      return pkb_->GetAllReadStmt();
    case PqlDeclarationEntity::kReadName:
      // Get all read stmt var from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all read variables");
      return pkb_->GetAllReadVar();
    case PqlDeclarationEntity::kPrint:
      // Get all print stmt from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all print statements");
      return pkb_->GetAllPrintStmt();
    case PqlDeclarationEntity::kPrintName:
      // Get all print var from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all print variables");
      return pkb_->GetAllPrintVar();
    case PqlDeclarationEntity::kCall:
      // Get all call stmt from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all call statements");
      return pkb_->GetAllCallStmt();
    case PqlDeclarationEntity::kCallName:
      // Get all call stmt from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all call.procName statements");
      return pkb_->GetAllCallee();
    case PqlDeclarationEntity::kWhile:
      // Get all while stmt from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all while statements");
      return pkb_->GetAllWhileStmt();
    case PqlDeclarationEntity::kIf:
      // Get all if stmt from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all if statements");
      return pkb_->GetAllIfStmt();
    case PqlDeclarationEntity::kConstant:
      // Get all constant from PKB and store into results
      // list
      Debug::PrintLn(Debug::kLog, "Select all constants");
      return pkb_->GetAllConstValue();
    case PqlDeclarationEntity::kProgline:
      // Get all program line from PKB and store into
      // results list
      Debug::PrintLn(Debug::kLog, "Select all program lines");
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
      Debug::PrintLn(Debug::kLog, "Select all twin procedures.");
      return pkb_->GetAllProcIndexTwin();
    case PqlDeclarationEntity::kVariable:
      Debug::PrintLn(Debug::kLog, "Select all twin variables.");
      return pkb_->GetAllVarIndexTwin();
    case PqlDeclarationEntity::kAssign:
      Debug::PrintLn(Debug::kLog, "Select all twin assign statements.");
      return pkb_->GetAllAssignStmtTwin();
    case PqlDeclarationEntity::kStmt:
      Debug::PrintLn(Debug::kLog, "Select all twin statements.");
      return pkb_->GetAllStmtTwin();
    case PqlDeclarationEntity::kRead:
      Debug::PrintLn(Debug::kLog, "Select all twin read statements.");
      return pkb_->GetAllReadStmtTwin();
    case PqlDeclarationEntity::kReadName:
      Debug::PrintLn(Debug::kLog, "Select all twin read variables.");
      return pkb_->GetAllReadPairWithSameVar();
    case PqlDeclarationEntity::kPrint:
      Debug::PrintLn(Debug::kLog, "Select all twin print statements.");
      return pkb_->GetAllPrintStmtTwin();
    case PqlDeclarationEntity::kPrintName:
      Debug::PrintLn(Debug::kLog, "Select all twin print variables.");
      return pkb_->GetAllPrintPairWithSameVar();
    case PqlDeclarationEntity::kCall:
      Debug::PrintLn(Debug::kLog, "Select all twin call statements.");
      return pkb_->GetAllCallStmtTwin();
    case PqlDeclarationEntity::kCallName:
      Debug::PrintLn(Debug::kLog, "Select all twin call.procName statements.");
      return pkb_->GetAllCallPairWithSameProc();
    case PqlDeclarationEntity::kWhile:
      Debug::PrintLn(Debug::kLog, "Select all twin while statements.");
      return pkb_->GetAllWhileStmtTwin();
    case PqlDeclarationEntity::kIf:
      Debug::PrintLn(Debug::kLog, "Select all twin if statements.");
      return pkb_->GetAllIfStmtTwin();
    case PqlDeclarationEntity::kConstant:
      Debug::PrintLn(Debug::kLog, "Select all twin constants.");
      return pkb_->GetAllConstValueTwin();
    case PqlDeclarationEntity::kProgline:
      Debug::PrintLn(Debug::kLog, "Select all twin program lines.");
      return pkb_->GetAllStmtTwin();
  }

  // Return empty result if nothing is found
  return results;
}

void PqlEvaluateWith::EvaluateWithTwoSynonym(PqlEvaluator* pql_eval,
                                             Synonym left_param,
                                             Synonym right_param) {
  Debug::PrintLn(Debug::kLog, "Evaluating with two synonyms...");

  // Getting parameter of with
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;
  QueryResultList result_left, result_right;
  QueryResultPairList filtered_result;

  // Change the 0c/0r/0p to c/r/p while evaluating
  if (left_type == PqlDeclarationEntity::kCallName ||
      left_type == PqlDeclarationEntity::kReadName ||
      left_type == PqlDeclarationEntity::kPrintName) {
    left_name = left_name.substr(1);
  }
  if (right_type == PqlDeclarationEntity::kCallName ||
      right_type == PqlDeclarationEntity::kReadName ||
      right_type == PqlDeclarationEntity::kPrintName) {
    right_name = right_name.substr(1);
  }

  // If same type, get twin pair
  if (left_type == right_type) {
    QueryResultPairList result_list = GetSelectAllTwinResult(left_type);
    if (result_list.empty()) {
      SetClauseFlag(false);
    } else {
      pql_eval->StoreClauseResultInTable(result_list, left_name, right_name);
    }
  } else {
    result_left = GetSelectAllResult(left_type);
    result_right = GetSelectAllResult(right_type);

    // If empty means invalid clause
    if (result_left.empty() || result_right.empty()) {
      SetClauseFlag(false);
      return;
    }

    // Check size to ensure fastest filter
    if (result_left.size() < result_right.size()) {
      // No efficient check for var/proc type so we dont use them to filter
      if (right_type == PqlDeclarationEntity::kVariable ||
          right_type == PqlDeclarationEntity::kProcedure) {
        filtered_result =
            FilterWithLeft(result_right, result_left, left_type, right_type);
      } else {
        filtered_result =
            FilterWithRight(result_left, result_right, right_type, left_type);
      }
    } else {
      if (left_type == PqlDeclarationEntity::kVariable ||
          left_type == PqlDeclarationEntity::kProcedure) {
        filtered_result =
            FilterWithRight(result_left, result_right, right_type, left_type);
      } else {
        filtered_result =
            FilterWithLeft(result_right, result_left, left_type, right_type);
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
  Debug::PrintLn(Debug::kLog, "Evaluating with one synonym...");

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
      Debug::PrintLn(Debug::kLog, "Checking if this is a procedure?...");
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
      Debug::PrintLn(Debug::kLog, "Checking if this is a variable?...");
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
      Debug::PrintLn(Debug::kLog,
                     "Checking if this is an assign statement?...");
      if (pkb_->IsAssignStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kStmt:
      Debug::PrintLn(Debug::kLog, "Checking if this is a statement?...");
      if (pkb_->IsStmtNum(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kRead:
      Debug::PrintLn(Debug::kLog, "Checking if this is a read statement?...");
      if (pkb_->IsReadStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kReadName:
      Debug::PrintLn(Debug::kLog, "Checking if this is is a read variable?...");
      if (pkb_->IsReadVar(comparison_val)) {
        // Get the related print statment
        result_list = pkb_->GetReadStmt(comparison_val);
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kPrint:
      Debug::PrintLn(Debug::kLog, "Checking if this is a print statement?...");
      if (pkb_->IsPrintStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kPrintName:
      Debug::PrintLn(Debug::kLog, "Checking if this is a print variable?...");
      if (pkb_->IsPrintVar(comparison_val)) {
        // Get the related print statment
        result_list = pkb_->GetPrintStmt(comparison_val);
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCall:
      Debug::PrintLn(Debug::kLog, "Checking if this is a call statement?...");
      if (pkb_->IsCallStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kCallName:
      Debug::PrintLn(Debug::kLog,
                     "Checking if this is call procedure name?...");
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
      Debug::PrintLn(Debug::kLog, "Checking if this is a while statement?...");
      if (pkb_->IsWhileStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kIf:
      Debug::PrintLn(Debug::kLog, "Checking if this is an if statement?...");
      if (pkb_->IsIfStmt(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kConstant:
      Debug::PrintLn(Debug::kLog, "Checking if this is a constant?...");
      if (pkb_->IsConstValue(stoi(comparison_val))) {
        result_list.push_back(stoi(comparison_val));
        pql_eval->StoreClauseResultInTable(result_list, syn_name);
      } else {
        SetClauseFlag(false);
      }
      return;
    case PqlDeclarationEntity::kProgline:
      Debug::PrintLn(Debug::kLog, "Checking if this is a program line?...");
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
    QueryResultList unfiltered_result, QueryResultList comparison_result,
    PqlDeclarationEntity comparison_type,
    PqlDeclarationEntity unfiltered_type) {
  QueryResultPairList filtered_result;
  IndexToVarProcMap index_to_var = pkb_->GetIndexToVarMapping();
  IndexToVarProcMap index_to_proc = pkb_->GetIndexToProcMapping();

  switch (comparison_type) {
    case PqlDeclarationEntity::kProcedure:
      Debug::PrintLn(Debug::kLog, "Filtering procedures vs var...");
      if (unfiltered_type == PqlDeclarationEntity::kVariable) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_proc[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_var[iter])) {
            // If proc name = var name
            filtered_result.push_back(
                std::make_pair(pkb_->GetProcIndex(index_to_var[iter]), iter));
          }
        }
      }
      break;
    case PqlDeclarationEntity::kVariable:
      Debug::PrintLn(Debug::kLog, "Filtering variable vs procedure...");
      if (unfiltered_type == PqlDeclarationEntity::kProcedure) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_var[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_proc[iter])) {
            // If var name = proc name
            filtered_result.push_back(
                std::make_pair(pkb_->GetVarIndex(index_to_proc[iter]), iter));
          }
        }
      }
      break;
    case PqlDeclarationEntity::kAssign:
      Debug::PrintLn(Debug::kLog, "Filtering assignment statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsAssignStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kStmt:
      Debug::PrintLn(Debug::kLog, "Filtering statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsStmtNum(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kRead:
      Debug::PrintLn(Debug::kLog, "Filtering read statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsReadStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kReadName:
      Debug::PrintLn(Debug::kLog, "Filtering read variables...");
      // If its vs proc
      if (unfiltered_type == PqlDeclarationEntity::kProcedure ||
          unfiltered_type == PqlDeclarationEntity::kCallName) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_var[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_proc[iter])) {
            QueryResultList result_list = pkb_->GetReadStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(result, iter));
            }
          }
        }
      }
      // Else its var vs var
      else {
        for (auto& iter : unfiltered_result) {
          if (index_to_var.find(iter) != index_to_var.end()) {
            if (pkb_->IsReadVar(index_to_var[iter])) {
              QueryResultList result_list =
                  pkb_->GetReadStmt(index_to_var[iter]);
              for (auto& result : result_list) {
                filtered_result.push_back(std::make_pair(result, iter));
              }
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kPrint:
      Debug::PrintLn(Debug::kLog, "Filtering print statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsPrintStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kPrintName:
      Debug::PrintLn(Debug::kLog, "Filtering print variables...");
      // If its vs proc
      if (unfiltered_type == PqlDeclarationEntity::kProcedure ||
          unfiltered_type == PqlDeclarationEntity::kCallName) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_var[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_proc[iter])) {
            QueryResultList result_list =
                pkb_->GetPrintStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(result, iter));
            }
          }
        }
      }
      // Else its vs var
      else {
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
      }
      break;
    case PqlDeclarationEntity::kCall:
      Debug::PrintLn(Debug::kLog, "Filtering call statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsCallStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kCallName:
      Debug::PrintLn(Debug::kLog, "Filtering call procedure names...");
      // If its vs var
      if (unfiltered_type == PqlDeclarationEntity::kVariable ||
          unfiltered_type == PqlDeclarationEntity::kReadName ||
          unfiltered_type == PqlDeclarationEntity::kPrintName) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_proc[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_var[iter])) {
            QueryResultList result_list = pkb_->GetCallingStmts(iter);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(result, iter));
            }
          }
        }
      }
      // Else its vs proc
      else {
        for (auto& iter : unfiltered_result) {
          if (pkb_->IsCalledProc(iter)) {
            QueryResultList result_list = pkb_->GetCallingStmts(iter);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(result, iter));
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kWhile:
      Debug::PrintLn(Debug::kLog, "Filtering while statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsWhileStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kIf:
      Debug::PrintLn(Debug::kLog, "Filtering if statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsIfStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kConstant:
      Debug::PrintLn(Debug::kLog, "Filtering constants...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsConstValue(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kProgline:
      Debug::PrintLn(Debug::kLog, "Filtering program lines...");
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
    QueryResultList unfiltered_result, QueryResultList comparison_result,
    PqlDeclarationEntity comparison_type,
    PqlDeclarationEntity unfiltered_type) {
  QueryResultPairList filtered_result;
  IndexToVarProcMap index_to_var = pkb_->GetIndexToVarMapping();
  IndexToVarProcMap index_to_proc = pkb_->GetIndexToProcMapping();

  switch (comparison_type) {
    case PqlDeclarationEntity::kProcedure:
      Debug::PrintLn(Debug::kLog, "Filtering procedures vs variables...");
      if (unfiltered_type == PqlDeclarationEntity::kVariable) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_proc[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_var[iter])) {
            // If proc name = var name
            filtered_result.push_back(
                std::make_pair(iter, pkb_->GetProcIndex(index_to_var[iter])));
          }
        }
      }
      break;
    case PqlDeclarationEntity::kVariable:
      Debug::PrintLn(Debug::kLog, "Filtering variables vs procedures...");
      if (unfiltered_type == PqlDeclarationEntity::kProcedure) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_var[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_proc[iter])) {
            // If var name = proc name
            filtered_result.push_back(
                std::make_pair(iter, pkb_->GetVarIndex(index_to_proc[iter])));
          }
        }
      }
      break;
    case PqlDeclarationEntity::kAssign:
      Debug::PrintLn(Debug::kLog, "Filtering assignment statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsAssignStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kStmt:
      Debug::PrintLn(Debug::kLog, "Filtering statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsStmtNum(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kRead:
      Debug::PrintLn(Debug::kLog, "Filtering read statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsReadStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kReadName:
      Debug::PrintLn(Debug::kLog, "Filtering read variables...");
      // If its vs proc
      if (unfiltered_type == PqlDeclarationEntity::kProcedure ||
          unfiltered_type == PqlDeclarationEntity::kCallName) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_var[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_proc[iter])) {
            QueryResultList result_list = pkb_->GetReadStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(iter, result));
            }
          }
        }
      }
      // Else its var vs var
      else {
        for (auto& iter : unfiltered_result) {
          if (index_to_var.find(iter) != index_to_var.end()) {
            if (pkb_->IsReadVar(index_to_var[iter])) {
              QueryResultList result_list =
                  pkb_->GetReadStmt(index_to_var[iter]);
              for (auto& result : result_list) {
                filtered_result.push_back(std::make_pair(iter, result));
              }
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kPrint:
      Debug::PrintLn(Debug::kLog, "Filtering print statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsPrintStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kPrintName:
      Debug::PrintLn(Debug::kLog, "Filtering print variables...");
      // If its vs proc
      if (unfiltered_type == PqlDeclarationEntity::kProcedure ||
          unfiltered_type == PqlDeclarationEntity::kCallName) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_var[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_proc[iter])) {
            QueryResultList result_list =
                pkb_->GetPrintStmt(index_to_var[iter]);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(iter, result));
            }
          }
        }
      }
      // Else its vs var
      else {
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
      }
      break;
    case PqlDeclarationEntity::kCall:
      Debug::PrintLn(Debug::kLog, "Filtering call statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsCallStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kCallName:
      Debug::PrintLn(Debug::kLog, "Filtering call procedure names...");
      // If its vs var
      if (unfiltered_type == PqlDeclarationEntity::kVariable ||
          unfiltered_type == PqlDeclarationEntity::kReadName ||
          unfiltered_type == PqlDeclarationEntity::kPrintName) {
        unordered_set<string> comparison;
        for (auto& compare_value : comparison_result) {
          comparison.insert(index_to_proc[compare_value]);
        }
        for (auto& iter : unfiltered_result) {
          if (comparison.count(index_to_var[iter])) {
            QueryResultList result_list = pkb_->GetCallingStmts(iter);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(iter, result));
            }
          }
        }
      }
      // Else its vs proc
      else {
        for (auto& iter : unfiltered_result) {
          if (pkb_->IsCalledProc(iter)) {
            QueryResultList result_list = pkb_->GetCallingStmts(iter);
            for (auto& result : result_list) {
              filtered_result.push_back(std::make_pair(iter, result));
            }
          }
        }
      }
      break;
    case PqlDeclarationEntity::kWhile:
      Debug::PrintLn(Debug::kLog, "Filtering while statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsWhileStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kIf:
      Debug::PrintLn(Debug::kLog, "Filtering if statements...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsIfStmt(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kConstant:
      Debug::PrintLn(Debug::kLog, "Filtering constants...");
      for (auto& iter : unfiltered_result) {
        if (pkb_->IsConstValue(iter)) {
          filtered_result.push_back(std::make_pair(iter, iter));
        }
      }
      break;
    case PqlDeclarationEntity::kProgline:
      Debug::PrintLn(Debug::kLog, "Filtering program lines...");
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
