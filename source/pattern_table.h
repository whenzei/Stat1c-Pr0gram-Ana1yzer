#pragma once

#ifndef SPA_PATTERN_TABLE_H
#define SPA_PATTERN_TABLE_H

#include <string>
#include <unordered_map>
#include "tokenizer.h"

using std::pair;
using std::string;
using std::unordered_map;

using StmtNum = string;
using StmtNumList = vector<string>;
using VarName = string;
using Expr = string;
using StmtVarPairList = vector<pair<string, string>>;
using AssignVarStmtMap = unordered_map<string, vector<string>>;
using AssignStmtVarMap = unordered_map<string, string>;
using AssignExactExprMap = unordered_map<string, vector<string>>;
using AssignSubExprMap = unordered_map<string, vector<string>>;

class PatternTable {
  AssignVarStmtMap assign_var_stmt_map_;
  AssignStmtVarMap assign_stmt_var_map_;
  AssignExactExprMap assign_exact_expr_map_;
  AssignSubExprMap assign_sub_expr_map_;
  
 public:
  void InsertAssignPattern(StmtNum stmt_num, VarName var_name,
                           TokenList token_list);

  StmtNumList GetAssignWithLfsVar(VarName var_name);

  StmtNumList GetAssignWithSubExpr(TokenList sub_expr_tokenlist);

  StmtNumList GetAssignWithExactExpr(TokenList exact_expr_tokenlist);

  StmtNumList GetAssignWithPattern(VarName var_name, TokenList sub_expr_tokenlist);

  StmtNumList GetAssignWithExactPattern(VarName var_name, TokenList exact_expr_tokenlist);

  StmtVarPairList GetAllAssignPatternPair(TokenList sub_expr_tokenlist);

  StmtVarPairList GetAllAssignExactPatternPair(TokenList exact_expr_tokenlist);

private:
  Expr ToString(TokenList token_list);
};

#endif !SPA_PATTERN_TABLE_H