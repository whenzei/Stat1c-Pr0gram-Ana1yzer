#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_evaluate_pattern.h"
#include "pql_evaluator.h"
#include "pql_global.h"

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::vector;

PqlEvaluatePattern::PqlEvaluatePattern() {}

bool PqlEvaluatePattern::EvaluatePatternClause(PqlEvaluator* pql_eval, PKB pkb,
                                               PqlPattern pattern) {
  SetPKB(pkb);
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

  cout << "Evaluating While Pattern." << endl;

  switch (left_type) {
    case PqlDeclarationEntity::kUnderscore:
      result_list = this->pkb_.GetWhileWithPattern("");
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kIdent:
      result_list = this->pkb_.GetWhileWithPattern(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for variable" << left_name << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kVariable:
      result_pair_list = this->pkb_.GetAllWhilePatternPair();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for synonym " << left_name << endl;
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

  cout << "Evaluating If Pattern." << endl;

  switch (left_type) {
    case PqlDeclarationEntity::kUnderscore:
      result_list = this->pkb_.GetIfWithPattern("");
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern " << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kIdent:
      result_list = this->pkb_.GetIfWithPattern(left_name);
      if (result_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for variable" << left_name << endl;
      } else {
        pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
      }
      return;
    case PqlDeclarationEntity::kVariable:
      result_pair_list = this->pkb_.GetAllIfPatternPair();
      if (result_pair_list.empty()) {
        SetClauseFlag(false);
        cout << " No such pattern for synonym " << left_name << endl;
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

  cout << "Evaluating Assign Pattern." << endl;

  switch (expression_type) {
    case PqlPatternExpressionType::kExpression:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list =
              this->pkb_.GetAssignWithExactPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern " << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list =
              this->pkb_.GetAssignWithExactPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for " << left_name << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list =
              this->pkb_.GetAllAssignExactPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for variable " << left_name << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_pair_list,
                                               pattern_var_name, left_name);
          }
          return;
      }
    case PqlPatternExpressionType::kUnderscoreExpressionUnderscore:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = this->pkb_.GetAssignWithPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern " << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list =
              this->pkb_.GetAssignWithPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for " << left_name << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list =
              this->pkb_.GetAllAssignPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for variable " << left_name << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_pair_list,
                                               pattern_var_name, left_name);
          }
          return;
      }
    case PqlPatternExpressionType::kUnderscore:
      switch (left_type) {
        case PqlDeclarationEntity::kUnderscore:
          result_list = this->pkb_.GetAssignWithPattern("", expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern " << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kIdent:
          result_list =
              this->pkb_.GetAssignWithPattern(left_name, expression_token);
          if (result_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for " << left_name << endl;
          } else {
            pql_eval->StoreClauseResultInTable(result_list, pattern_var_name);
          }
          return;
        case PqlDeclarationEntity::kVariable:
          result_pair_list =
              this->pkb_.GetAllAssignPatternPair(expression_token);
          if (result_pair_list.empty()) {
            SetClauseFlag(false);
            cout << " No such pattern for variable " << left_name << endl;
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

void PqlEvaluatePattern::SetPKB(PKB pkb) { this->pkb_ = pkb; }