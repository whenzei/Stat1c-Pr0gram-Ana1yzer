#include "pql_evaluate_pattern.h"
#include "debug.h"
#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"

PqlEvaluatePattern::PqlEvaluatePattern() {}

bool PqlEvaluatePattern::EvaluatePatternClause(PqlEvaluator* pql_eval, PKB* pkb,
                                               PqlPattern pattern) {
  SetPKB(pkb);
  SetClauseFlag(true);
  PqlPatternType pattern_type = pattern.GetType().second;

  switch (pattern_type) {
    case PqlPatternType::kAssign:
      EvaluateAssignPattern(pql_eval, pattern);
      break;
    case PqlPatternType::kWhile:
      EvaluateWhilePattern(pql_eval, pattern);
      break;
    case PqlPatternType::kIf:
      EvaluateIfPattern(pql_eval, pattern);
      break;
  }

  return IsValidClause();
}

void PqlEvaluatePattern::EvaluateWhilePattern(PqlEvaluator* pql_eval,
                                              PqlPattern pattern) {
  // Getting parameter of pattern
  string pattern_var_name = pattern.GetType().first;
  pair<string, PqlDeclarationEntity> first_parameter =
      pattern.GetFirstParameter();
  string left_name = first_parameter.first;
  PqlDeclarationEntity left_type = first_parameter.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  Debug::PrintLn(Debug::kLog, "Evaluating While Pattern...");

  switch (left_type) {
    case PqlDeclarationEntity::kUnderscore:
      result_list = pkb_->GetWhileWithPattern("");
      if (result_list.empty()) {
        SetClauseFlag(false);
        Debug::PrintLn(Debug::kLog, "No such pattern");
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kIdent:
      result_list = pkb_->GetWhileWithPattern(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        Debug::PrintLn(Debug::kLog,
                       "No such pattern for variable: " + left_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kVariable:
      result_pair_list = pkb_->GetAllWhilePatternPair();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        Debug::PrintLn(Debug::kLog,
                       "No such pattern for synonym: " + left_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, pattern_var_name,
                                           left_name);
      }
      return;
  }
}

void PqlEvaluatePattern::EvaluateIfPattern(PqlEvaluator* pql_eval,
                                           PqlPattern pattern) {
  // Getting parameter of pattern
  string pattern_var_name = pattern.GetType().first;
  pair<string, PqlDeclarationEntity> first_parameter =
      pattern.GetFirstParameter();
  string left_name = first_parameter.first;
  PqlDeclarationEntity left_type = first_parameter.second;
  QueryResultList result_list;
  QueryResultPairList result_pair_list;

  Debug::PrintLn(Debug::kLog, "Evaluating If Pattern");

  switch (left_type) {
    case PqlDeclarationEntity::kUnderscore:
      result_list = pkb_->GetIfWithPattern("");
      if (result_list.empty()) {
        SetClauseFlag(false);
        Debug::PrintLn(Debug::kLog, "No such pattern");
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kIdent:
      result_list = pkb_->GetIfWithPattern(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        Debug::PrintLn(Debug::kLog, "No such pattern for: " + left_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kVariable:
      result_pair_list = pkb_->GetAllIfPatternPair();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        Debug::PrintLn(Debug::kLog,
                       "No such pattern for synonym: " + left_name);
      } else {
        pql_eval->StoreClauseResultInTable(result_pair_list, pattern_var_name,
                                           left_name);
      }
      return;
  }
}

void PqlEvaluatePattern::EvaluateAssignPattern(PqlEvaluator* pql_eval,
                                               PqlPattern pattern) {
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

  Debug::PrintLn(Debug::kLog, "Evaluating assign pattern");

  switch (expression_type) {
    case PqlPatternExpressionType::kExpression:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = pkb_->GetAssignWithExactPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog, "No such pattern");
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list =
              pkb_->GetAssignWithExactPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog, "No such pattern: " + left_name);
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list =
              pkb_->GetAllAssignExactPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog,
                           "No such pattern for variable: " + left_name);
          } else {
            pql_eval->StoreClauseResultInTable(result_pair_list,
                                               pattern_var_name, left_name);
          }
          return;
      }
    case PqlPatternExpressionType::kUnderscoreExpressionUnderscore:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = pkb_->GetAssignWithPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog, "No such pattern");
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list = pkb_->GetAssignWithPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog, "No such pattern for: " + left_name);
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list = pkb_->GetAllAssignPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog,
                           "No such pattern for variable: " + left_name);
          } else {
            pql_eval->StoreClauseResultInTable(result_pair_list,
                                               pattern_var_name, left_name);
          }
          return;
      }
    case PqlPatternExpressionType::kUnderscore:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = pkb_->GetAssignWithPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog, "No such pattern");
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list = pkb_->GetAssignWithPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog, "No such pattern for: " + left_name);
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list = pkb_->GetAllAssignPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            Debug::PrintLn(Debug::kLog,
                           "No such pattern for variable: " + left_name);
          } else {
            pql_eval->StoreClauseResultInTable(result_pair_list,
                                               pattern_var_name, left_name);
          }
          return;
      }
  }
}

/* Getters and Setters */

void PqlEvaluatePattern::SetClauseFlag(bool clause_flag) {
  this->clause_flag_ = clause_flag;
}

bool PqlEvaluatePattern::IsValidClause() { return clause_flag_; }

void PqlEvaluatePattern::SetPKB(PKB* pkb) { this->pkb_ = pkb; }