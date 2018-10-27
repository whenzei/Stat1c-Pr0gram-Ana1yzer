#pragma once

#ifndef SPA_STMT_DATA_H
#define SPA_STMT_DATA_H

#include "pkb.h"
#include "tokenizer.h"

class StatementData {
 public:
  StmtNum stmt_num_;
  StmtNum stmt_num_int_;
  ProcIndex proc_index_;
  StmtNum GetStmtNum();
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

  VarName GetModifiedVariable();
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
  TokenList GetPostfixedExpr();
};

class IfStmtData : public StatementData {
  VarNameSet control_vars_;
  ConstValueSet used_consts_;

 public:
  IfStmtData(StmtNum stmt_num, ProcIndex proc_index, VarNameSet control_vars,
             ConstValueSet used_consts);
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
};

class WhileStmtData : public StatementData {
  VarNameSet control_vars_;
  ConstValueSet used_consts_;

 public:
  WhileStmtData(StmtNum stmt_num, ProcIndex proc_index, VarNameSet control_vars,
                ConstValueSet used_consts);
  VarNameSet GetUsedVariables();
  ConstValueSet GetUsedConstants();
};

class ReadStmtData : public StatementData {
  VarName modified_var_;

 public:
  ReadStmtData(StmtNum stmt_num, ProcIndex proc_index, VarName modified_var);
  VarName GetModifiedVariable();
};

class PrintStmtData : public StatementData {
  VarName used_var_;

 public:
  PrintStmtData(StmtNum stmt_num, ProcIndex proc_index, VarName used_var);
  VarName GetUsedVariable();
};

class CallStmtData : public StatementData {
  ProcName caller_proc_name_;
  ProcName callee_proc_name_;
 public:
  CallStmtData(StmtNum stmt_num, ProcIndex proc_index, ProcName caller_proc_name,
               ProcName callee_proc_name);

  ProcName GetCallerProcName();
  ProcName GetCalleeProcName();
};

#endif  // !SPA_STMT_DATA_H
