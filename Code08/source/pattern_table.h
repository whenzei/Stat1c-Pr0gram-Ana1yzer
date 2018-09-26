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

// The pattern table class for the PKB component
// Used to store assign statement patterns
// TODO: add while and if patterns
class PatternTable {
  AssignVarStmtMap assign_var_stmt_map_;
  AssignStmtVarMap assign_stmt_var_map_;
  AssignExactExprMap assign_exact_expr_map_;
  AssignSubExprMap assign_sub_expr_map_;
  
 public:
  // add the stmt_num and var_name to assign_var_stmt_map_ and
  // assign_stmt_var_map_
  // generate all sub-expressions and add to assign_sub_expr_map_
  // add the exact epression to assign_exact_expr_map_
  void InsertAssignPattern(StmtNum stmt_num, VarName var_name,
                           TokenList token_list);

  // @returns a list of assign statements with var_name as the left hand side
  // variable
  StmtNumList GetAssignWithLfsVar(VarName var_name);

  // @returns a list of assign statements that contain the given sub-expression on
  // the right hand side
  StmtNumList GetAssignWithSubExpr(TokenList sub_expr_tokenlist);

  // @returns a list of assign statements that have the given expression on
  // the right hand side
  StmtNumList GetAssignWithExactExpr(TokenList exact_expr_tokenlist);

  // @returns a list of assign statements that have var_name as the left hand side
  // variable and contain the given sub-expression on the right hand side
  StmtNumList GetAssignWithPattern(VarName var_name,
                                   TokenList sub_expr_tokenlist);

  // @returns a list of assign statements that have var_name as the left hand side
  // variable and have the given expression on the right hand side
  StmtNumList GetAssignWithExactPattern(VarName var_name, TokenList exact_expr_tokenlist);

  // @returns a list of all pairs of assign statements and their corresponding
  // lfs var_name that contain the given sub-expression on the right hand side
  StmtVarPairList GetAllAssignPatternPair(TokenList sub_expr_tokenlist);

  // @returns a list of all pairs of assign statements and their corresponding
  // lfs var_name that have the given expression on the right hand side
  StmtVarPairList GetAllAssignExactPatternPair(TokenList exact_expr_tokenlist);

private:
  Expr ToString(TokenList token_list);
};

#endif !SPA_PATTERN_TABLE_H