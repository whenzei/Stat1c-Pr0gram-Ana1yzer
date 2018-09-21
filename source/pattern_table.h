#pragma once

#ifndef SPA_PATTERN_TABLE_H
#define SPA_PATTERN_TABLE_H

#include <list>
#include <string>
#include <unordered_map>
#include "tokenizer.h"

using std::list;
using std::pair;
using std::string;
using std::unordered_map;

using StmtNum = string;
using StmtNumList = list<string>;
using VarName = string;
using Expr = string;
using StmtVarPairList = list<pair<string, string>>;
using AssignVarStmtMap = unordered_map<string, list<string>>;
using AssignStmtVarMap = unordered_map<string, string>;
using AssignExactExprMap = unordered_map<string, list<string>>;
using AssignSubExprMap = unordered_map<string, list<string>>;

class PatternTable {
  AssignVarStmtMap assign_var_stmt_map_;
  AssignStmtVarMap assign_stmt_var_map_;
  AssignExactExprMap assign_exact_expr_map_;
  AssignSubExprMap assign_sub_expr_map_;
  const string kSpace = " ";
  
 public:
  void InsertAssignPattern(StmtNum stmt_num, VarName var_name,
                           TokenList token_list);

  StmtNumList GetAssignWithLfsVar(VarName var_name);

  StmtNumList GetAssignWithSubExpr(Expr sub_expr);

  StmtNumList GetAssignWithExactExpr(Expr exact_expr);

  StmtNumList GetAssignWithPattern(VarName var_name, Expr sub_expr);

  StmtNumList GetAssignWithExactPattern(VarName var_name, Expr exact_expr);

  StmtVarPairList GetAllAssignPatternPair();

  StmtVarPairList GetAllAssignPatternPair(Expr sub_expr);

  StmtVarPairList GetAllAssignExactPatternPair(Expr exact_expr);
};

#endif !SPA_PATTERN_TABLE_H