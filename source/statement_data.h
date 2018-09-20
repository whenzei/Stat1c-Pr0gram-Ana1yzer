#pragma once

#ifndef SPA_STMT_DATA_H
#define SPA_STMT_DATA_H

#include "pkb.h"
#include "tokenizer.h"

class StatementData {
 public:
  string stmt_num_;
  int stmt_list_index_;
  string GetStmtNum();
  int GetStmtListIndex();
};

class AssignStmtData : public StatementData {
  VarName lhs_var_;
  VarNameSet rhs_vars_;
  ConstValueSet rhs_consts_;
  TokenList postfixed_expr_;

 public:
  AssignStmtData(int, int, VarName, VarNameSet, ConstValueSet, TokenList);
  VarName GetModifiedVariable();
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
  TokenList GetPostfixedExpr();
};

class IfStmtData {
  StmtListIndex then_stmt_list_index_;
  StmtListIndex else_stmt_list_index_;
};

class WhileStmtData {
  StmtListIndex while_stmt_list_index_;
};

#endif  // !SPA_STMT_DATA_H
