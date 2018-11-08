#pragma once

#include "pattern_table.h"
#include <stack>

using std::make_pair;
using std::stack;
using TokenType = Tokenizer::TokenType;

void PatternTable::InsertAssignPattern(StmtNum stmt_num, VarIndex var_index,
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
          assign_sub_expr_map_[token.value].insert(stmt_num);
          break;
        case TokenType::kOperator:
          operand2 = subtree_stack.top();
          subtree_stack.pop();
          operand1 = subtree_stack.top();
          subtree_stack.pop();
          subtree = operand1 + " " + operand2 + " " + token.value;
          assign_sub_expr_map_[subtree].insert(stmt_num);
          subtree_stack.push(subtree);
          break;
      }
      iter++;
    }
    assign_sub_expr_map_[string()].insert(stmt_num);
    assign_exact_expr_map_[subtree_stack.top()].insert(stmt_num);
    assign_stmt_var_map_[stmt_num] = var_index;
    assign_var_stmt_map_[var_index].insert(stmt_num);
  }
}

void PatternTable::InsertWhilePattern(StmtNum stmt_num, VarIndex var_index) {
  while_var_stmt_map_[var_index].insert(stmt_num);
}

void PatternTable::InsertIfPattern(StmtNum stmt_num, VarIndex var_index) {
  if_var_stmt_map_[var_index].insert(stmt_num);
}

StmtNumSet PatternTable::GetAssignWithLfsVar(VarIndex var_index) {
  VarStmtMap::iterator iter = assign_var_stmt_map_.find(var_index);
  if (iter != assign_var_stmt_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtNumSet PatternTable::GetAssignWithSubExpr(TokenList sub_expr_tokenlist) {
  Expr sub_expr = ToString(sub_expr_tokenlist);
  ExprStmtMap::iterator iter = assign_sub_expr_map_.find(sub_expr);
  if (iter != assign_sub_expr_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtNumSet PatternTable::GetAssignWithExactExpr(
    TokenList exact_expr_tokenlist) {
  Expr exact_expr = ToString(exact_expr_tokenlist);
  ExprStmtMap::iterator iter = assign_exact_expr_map_.find(exact_expr);
  if (iter != assign_exact_expr_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtNumSet PatternTable::GetAssignWithPattern(VarIndex var_index,
                                               TokenList sub_expr_tokenlist) {
  Expr sub_expr = ToString(sub_expr_tokenlist);
  ExprStmtMap::iterator iter = assign_sub_expr_map_.find(sub_expr);
  StmtNumSet result;
  if (iter != assign_sub_expr_map_.end()) {
    StmtNumSet intermediate_result = (*iter).second;
    for (auto& stmt_num : intermediate_result) {
      if (assign_stmt_var_map_[stmt_num] == var_index) {
        result.insert(stmt_num);
      }
    }
  }
  return result;
}

StmtNumSet PatternTable::GetAssignWithExactPattern(
    VarIndex var_index, TokenList exact_expr_tokenlist) {
  Expr exact_expr = ToString(exact_expr_tokenlist);
  ExprStmtMap::iterator iter = assign_exact_expr_map_.find(exact_expr);
  StmtNumSet result;
  if (iter != assign_exact_expr_map_.end()) {
    StmtNumSet intermediate_result = (*iter).second;
    for (auto& stmt_num : intermediate_result) {
      if (assign_stmt_var_map_[stmt_num] == var_index) {
        result.insert(stmt_num);
      }
    }
  }
  return result;
}

StmtVarPairSet PatternTable::GetAllAssignPatternPair(
    TokenList sub_expr_tokenlist) {
  Expr sub_expr = ToString(sub_expr_tokenlist);
  ExprStmtMap::iterator iter = assign_sub_expr_map_.find(sub_expr);
  StmtVarPairSet result;
  if (iter != assign_sub_expr_map_.end()) {
    StmtNumSet intermediate_result = (*iter).second;
    for (auto& stmt_num : intermediate_result) {
      result.insert(make_pair(stmt_num, assign_stmt_var_map_[stmt_num]));
    }
  }
  return result;
}

StmtVarPairSet PatternTable::GetAllAssignExactPatternPair(
    TokenList exact_expr_tokenlist) {
  Expr exact_expr = ToString(exact_expr_tokenlist);
  ExprStmtMap::iterator iter = assign_exact_expr_map_.find(exact_expr);
  StmtVarPairSet result;
  if (iter != assign_exact_expr_map_.end()) {
    StmtNumSet intermediate_result = (*iter).second;
    for (auto& stmt_num : intermediate_result) {
      result.insert(make_pair(stmt_num, assign_stmt_var_map_[stmt_num]));
    }
  }
  return result;
}

StmtNumSet PatternTable::GetWhileWithPattern(VarIndex var_index) {
  VarStmtMap::iterator iter = while_var_stmt_map_.find(var_index);
  if (iter != while_var_stmt_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtVarPairSet PatternTable::GetAllWhilePatternPair() {
  StmtVarPairSet result;
  for (auto entry : while_var_stmt_map_) {
    for (auto& stmt_num : entry.second) {
      result.insert(make_pair(stmt_num, entry.first));
    }
  }
  return result;
}

StmtNumSet PatternTable::GetIfWithPattern(VarIndex var_index) {
  VarStmtMap::iterator iter = if_var_stmt_map_.find(var_index);
  if (iter != if_var_stmt_map_.end()) {
    return (*iter).second;
  } else {
    return StmtNumSet();
  }
}

StmtVarPairSet PatternTable::GetAllIfPatternPair() {
  StmtVarPairSet result;
  for (auto entry : if_var_stmt_map_) {
    for (auto& stmt_num : entry.second) {
      result.insert(make_pair(stmt_num, entry.first));
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
