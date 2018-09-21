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
  AssignStmtData(int stmt_num, int stmtlist_index, VarName, VarNameSet,
                 ConstValueSet, TokenList);

  VarName GetModifiedVariable();
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
  TokenList GetPostfixedExpr();
};

class IfStmtData {
  StmtListIndex then_stmt_list_index_;
  StmtListIndex else_stmt_list_index_;
};

class WhileStmtData : public StatementData {
  StmtListIndex child_stmt_list_index_;
  VarNameSet control_vars_;
  ConstValueSet used_consts_;

 public:
  WhileStmtData(int stmt_num, int stmtlist_index, int child_stmtlist_index,
                VarNameSet control_vars, ConstValueSet used_consts);
  StmtListIndex GetChildStmtListIndex();
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
};

#endif  // !SPA_STMT_DATA_H
