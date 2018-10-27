#pragma once

#ifndef SPA_STMT_DATA_H
#define SPA_STMT_DATA_H

#include "pkb.h"
#include "tokenizer.h"

class StatementData {
 public:
  StmtNum stmt_num_;
  ProcIndex proc_index_;

  // @returns statement number of this statement
  StmtNum GetStmtNum();

  // @returns the index of the procedure that this statement belongs to
  ProcIndex GetProcOfStmt();
};

class AssignStmtData : public StatementData {
  VarName lhs_var_;
  VarNameSet rhs_vars_;
  ConstValueSet rhs_consts_;
  TokenList postfixed_expr_;

 public:
  AssignStmtData(StmtNum stmt_num, ProcIndex proc_index, VarName lhs_var,
                 VarNameSet rhs_vars, ConstValueSet rhs_consts,
                 TokenList postfixed_expr);

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
  IfStmtData(StmtNum stmt_num, ProcIndex proc_index, VarNameSet control_vars,
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
  WhileStmtData(StmtNum stmt_num, ProcIndex proc_index, VarNameSet control_vars,
                ConstValueSet used_consts);

  // @returns the used variables of this while statement
  VarNameSet GetUsedVariables();

  // @returns the used constants of this while statement
  ConstValueSet GetUsedConstants();
};

class ReadStmtData : public StatementData {
  VarName modified_var_;

 public:
  ReadStmtData(StmtNum stmt_num, ProcIndex proc_index, VarName modified_var);

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
  CallStmtData(StmtNum stmt_num, ProcIndex proc_index, ProcName caller_proc_name,
               ProcName callee_proc_name);

  // @returns the caller's procedure name of this call statement
  ProcName GetCallerProcName();

  // @returns the callee's procedure name of this call statement
  ProcName GetCalleeProcName();
};

#endif  // !SPA_STMT_DATA_H
