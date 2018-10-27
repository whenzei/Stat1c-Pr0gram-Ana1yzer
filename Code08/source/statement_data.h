#pragma once

#ifndef SPA_STMT_DATA_H
#define SPA_STMT_DATA_H

#include "pkb.h"
#include "tokenizer.h"

using StmtNum = int;

class StatementData {
 public:
  StmtNum stmt_num_;
  StmtNum stmt_num_int_;
  int stmt_list_index_;

  // @returns statement number of this statement
  StmtNum GetStmtNum();

  // @returns the statement list index of this statement
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

  // @returns the modified variable of this assign statement
  VarName GetModifiedVariable();

  // @returns the used variables of this assign statement
  VarNameSet GetUsedVariables();

  // @returns the used constants of this assign statement
  ConstValueSet GetUsedConstants();

  // @returns the post-fixed expression from the expression passed in during
  // construction
  TokenList GetPostfixedExpr();
};

class IfStmtData : public StatementData {
  VarNameSet control_vars_;
  ConstValueSet used_consts_;

 public:
  IfStmtData(int stmt_num, int stmt_list_index, VarNameSet control_vars,
             ConstValueSet used_consts);

  // @returns the used variables of this if statement
  VarNameSet GetUsedVariables();

  // @returns the used constants of this if statement
  ConstValueSet GetUsedConstants();
};

class WhileStmtData : public StatementData {
  VarNameSet control_vars_;
  ConstValueSet used_consts_;

 public:
  WhileStmtData(int stmt_num, int stmt_list_index, VarNameSet control_vars,
                ConstValueSet used_consts);

  // @returns the used variables of this while statement
  VarNameSet GetUsedVariables();

  // @returns the used constants of this while statement
  ConstValueSet GetUsedConstants();
};

class ReadStmtData : public StatementData {
  VarName modified_var_;

 public:
  ReadStmtData(int stmt_num, int stmt_list_index, VarName modified_var);

  // @returns the modified variable of this read statement
  VarName GetModifiedVariable();
};

class PrintStmtData : public StatementData {
  VarName used_var_;

 public:
  PrintStmtData(int stmt_num, int stmt_list_index, VarName used_var);

  // @returns the used variable of this print statement
  VarName GetUsedVariable();
};

class CallStmtData : public StatementData {
  ProcName caller_proc_name_;
  ProcName callee_proc_name_;

 public:
  CallStmtData(int stmt_num, int stmt_list_index, ProcName caller_proc_name,
               ProcName callee_proc_name);

  // @returns the caller's procedure name of this call statement
  ProcName GetCallerProcName();

  // @returns the callee's procedure name of this call statement
  ProcName GetCalleeProcName();
};

#endif  // !SPA_STMT_DATA_H
