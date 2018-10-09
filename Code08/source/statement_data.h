#pragma once

#ifndef SPA_STMT_DATA_H
#define SPA_STMT_DATA_H

#include "pkb.h"
#include "tokenizer.h"

class StatementData {
 public:
  string stmt_num_;
  int stmt_num_int_;
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
  AssignStmtData(int stmt_num, int stmt_list_index, VarName, VarNameSet,
                 ConstValueSet, TokenList);

  VarName GetModifiedVariable();
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
  TokenList GetPostfixedExpr();
};

class IfStmtData : public StatementData {
  VarNameSet control_vars_;
  ConstValueSet used_consts_;

 public:
  IfStmtData(int stmt_num, int stmt_list_index, VarNameSet control_vars,
             ConstValueSet used_consts);
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
};

class WhileStmtData : public StatementData {
  VarNameSet control_vars_;
  ConstValueSet used_consts_;

 public:
  WhileStmtData(int stmt_num, int stmt_list_index, VarNameSet control_vars,
                ConstValueSet used_consts);
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
};

class ReadStmtData : public StatementData {
  VarName modified_var_;

 public:
  ReadStmtData(int stmt_num, int stmt_list_index, VarName modified_var);
  VarName GetModifiedVariable();
};

class PrintStmtData : public StatementData {
  VarName used_var_;

 public:
  PrintStmtData(int stmt_num, int stmt_list_index, VarName used_var);
  VarName GetUsedVariable();
};

class CallStmtData : public StatementData {
  ProcName caller_proc_name_;
  ProcName callee_proc_name_;
 public:
  CallStmtData(int stmt_num, int stmt_list_index, ProcName caller_proc_name,
               ProcName callee_proc_name);

  ProcName GetCallerProcName();
  ProcName GetCalleeProcName();
};

#endif  // !SPA_STMT_DATA_H
