#pragma once

#ifndef SPA_PATTERN_TABLE_H
#define SPA_PATTERN_TABLE_H

#include <string>
#include <unordered_map>
#include "tokenizer.h"
#include "var_list.h"

using std::pair;
using std::string;
using std::unordered_map;

using StmtNum = int;
using StmtNumList = vector<int>;
using VarName = string;
using VarIndex = int;
using Expr = string;
using StmtVarPairList = vector<pair<int, VarIndex>>;
using VarStmtMap = unordered_map<VarIndex, vector<int>>;
using StmtVarMap = unordered_map<int, VarIndex>;
using ExprStmtMap = unordered_map<string, vector<int>>;

// The pattern table class for the PKB component
// Used to store assign statement patterns
class PatternTable {
  VarStmtMap assign_var_stmt_map_;
  StmtVarMap assign_stmt_var_map_;
  ExprStmtMap assign_exact_expr_map_;
  ExprStmtMap assign_sub_expr_map_;
  VarStmtMap while_var_stmt_map_;
  VarStmtMap if_var_stmt_map_;
  VarList var_list;
  
 public:
  // add the stmt_num and var_name to assign_var_stmt_map_ and
  // assign_stmt_var_map_
  // generate all sub-expressions and add to assign_sub_expr_map_
  // add the exact epression to assign_exact_expr_map_
  void InsertAssignPattern(StmtNum stmt_num, VarName var_name,
                           TokenList token_list);

  // add the stmt_num and var_name to while_var_stmt_map_ 
  void InsertWhilePattern(StmtNum stmt_num, VarName var_name);

  // add the stmt_num and var_name to if_var_stmt_map_
  void InsertIfPattern(StmtNum stmt_num, VarName var_name);

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

  // @returns a list of while statements that have var_name as a control
  // variable
  StmtNumList GetWhileWithPattern(VarName var_name);

  // @returns a list of all pairs of while statements and their control
  // variables
  StmtVarPairList GetAllWhilePatternPair();

  // @returns a list of if statements that have var_name as a control
  // variable
  StmtNumList GetIfWithPattern(VarName var_name);

  // @returns a list of all pairs of if statements and their control
  // variables
  StmtVarPairList GetAllIfPatternPair();

private:
  Expr ToString(TokenList token_list);
};

#endif !SPA_PATTERN_TABLE_H