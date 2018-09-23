#pragma once

#include "pattern_table.h"
#include <stack>

using std::make_pair;
using std::stack;
using TokenType = Tokenizer::TokenType;

void PatternTable::InsertAssignPattern(StmtNum stmt_num, VarName var_name,
                                       TokenList expr_tokenlist) {
  if (!expr_tokenlist.empty()) {
    TokenList::iterator iter = expr_tokenlist.begin();
    stack<string> subtree_stack;
    Token token;
    string operand1;
    string operand2;
    string subtree;
    while (iter != expr_tokenlist.end()) {
      token = *iter;
      switch (token.type) {
        case TokenType::kDigit:
        case TokenType::kName:
          subtree_stack.push(token.value);
          assign_sub_expr_map_[token.value].push_back(stmt_num);
          break;
        case TokenType::kOperator:
          operand2 = subtree_stack.top();
          subtree_stack.pop();
          operand1 = subtree_stack.top();
          subtree_stack.pop();
          subtree = operand1 + " " + operand2 + " " + token.value;
          assign_sub_expr_map_[subtree].push_back(stmt_num);
          subtree_stack.push(subtree);
          break;
      }
      iter++;
    }
    assign_sub_expr_map_[string()].push_back(stmt_num);
    assign_exact_expr_map_[subtree_stack.top()].push_back(stmt_num);
    assign_stmt_var_map_[stmt_num] = var_name;
    assign_var_stmt_map_[var_name].push_back(stmt_num);
  }
}

StmtNumList PatternTable::GetAssignWithLfsVar(VarName var_name) {
  AssignVarStmtMap::iterator iter = assign_var_stmt_map_.find(var_name);
  if (iter != assign_var_stmt_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

StmtNumList PatternTable::GetAssignWithSubExpr(TokenList sub_expr_tokenlist) {
  Expr sub_expr = ToString(sub_expr_tokenlist);
  AssignSubExprMap::iterator iter = assign_sub_expr_map_.find(sub_expr);
  if (iter != assign_sub_expr_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

StmtNumList PatternTable::GetAssignWithExactExpr(TokenList exact_expr_tokenlist) {
  Expr exact_expr = ToString(exact_expr_tokenlist);
  AssignExactExprMap::iterator iter = assign_exact_expr_map_.find(exact_expr);
  if (iter != assign_exact_expr_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  }
}

StmtNumList PatternTable::GetAssignWithPattern(VarName var_name,
                                               TokenList sub_expr_tokenlist) {
  Expr sub_expr = ToString(sub_expr_tokenlist);
  AssignSubExprMap::iterator iter = assign_sub_expr_map_.find(sub_expr);
  StmtNumList result;
  if (iter != assign_sub_expr_map_.end()) {
    StmtNumList intermediate_result = (*iter).second;
    for (StmtNum& stmt_num : intermediate_result) {
      if (assign_stmt_var_map_[stmt_num] == var_name) {
        result.push_back(stmt_num);
      }
    }
  }
  return result;
}

StmtNumList PatternTable::GetAssignWithExactPattern(VarName var_name,
                                                    TokenList exact_expr_tokenlist) {
  Expr exact_expr = ToString(exact_expr_tokenlist);
  AssignExactExprMap::iterator iter = assign_exact_expr_map_.find(exact_expr);
  StmtNumList result;
  if (iter != assign_exact_expr_map_.end()) {
    StmtNumList intermediate_result = (*iter).second;
    for (StmtNum& stmt_num : intermediate_result) {
      if (assign_stmt_var_map_[stmt_num] == var_name) {
        result.push_back(stmt_num);
      }
    }
  }
  return result;
}

StmtVarPairList PatternTable::GetAllAssignPatternPair(TokenList sub_expr_tokenlist) {
  Expr sub_expr = ToString(sub_expr_tokenlist);
  AssignSubExprMap::iterator iter = assign_sub_expr_map_.find(sub_expr);
  StmtVarPairList result;
  if (iter != assign_sub_expr_map_.end()) {
    StmtNumList intermediate_result = (*iter).second;
    for (StmtNum& stmt_num : intermediate_result) {
      result.push_back(make_pair(stmt_num, assign_stmt_var_map_[stmt_num]));
    }
  }
  return result;
}

StmtVarPairList PatternTable::GetAllAssignExactPatternPair(TokenList exact_expr_tokenlist) {
  Expr exact_expr = ToString(exact_expr_tokenlist);
  AssignExactExprMap::iterator iter = assign_exact_expr_map_.find(exact_expr);
  StmtVarPairList result;
  if (iter != assign_exact_expr_map_.end()) {
    StmtNumList intermediate_result = (*iter).second;
    for (StmtNum& stmt_num : intermediate_result) {
      result.push_back(make_pair(stmt_num, assign_stmt_var_map_[stmt_num]));
    }
  }
  return result;
}

Expr PatternTable::ToString(TokenList token_list) {
  Expr expr;
  if (token_list.empty()) {
    return expr;
  } else {
    TokenList::iterator iter = token_list.begin();
    while (iter != token_list.end()) {
      expr += (*iter).value + " ";
      iter++;
    };
    return expr.substr(0, expr.size() - 1);
  }
}
