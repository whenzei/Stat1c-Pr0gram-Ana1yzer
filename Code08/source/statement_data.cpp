#include "statement_data.h"

StmtNum StatementData::GetStmtNum() { return stmt_num_; }

ProcIndex StatementData::GetProcOfStmt() { return proc_index_; }

AssignStmtData::AssignStmtData(StmtNum stmt_num, ProcIndex proc_index,
                               VarName lhs_var, VarNameSet rhs_vars,
                               ConstValueSet rhs_consts,
                               TokenList postfixed_expr) {
  stmt_num_ = stmt_num;
  proc_index_ = proc_index;
  lhs_var_ = lhs_var;
  rhs_vars_ = rhs_vars;
  rhs_consts_ = rhs_consts;
  postfixed_expr_ = postfixed_expr;
}

VarName AssignStmtData::GetModifiedVariable() { return lhs_var_; }
VarNameSet AssignStmtData::GetUsedVariables() { return rhs_vars_; }
ConstValueSet AssignStmtData::GetUsedConstants() { return rhs_consts_; }
TokenList AssignStmtData::GetPostfixedExpr() { return postfixed_expr_; }

WhileStmtData::WhileStmtData(StmtNum stmt_num, ProcIndex proc_index,
                             VarNameSet control_vars,
                             ConstValueSet used_consts) {
  stmt_num_ = stmt_num;
  proc_index_ = proc_index;
  control_vars_ = control_vars;
  used_consts_ = used_consts;
}

VarNameSet WhileStmtData::GetUsedVariables() { return control_vars_; }

ConstValueSet WhileStmtData::GetUsedConstants() { return used_consts_; }

ReadStmtData::ReadStmtData(StmtNum stmt_num, ProcIndex proc_index,
                           VarName modified_var) {
  stmt_num_ = stmt_num;
  proc_index_ = proc_index;
  modified_var_ = modified_var;
}

VarName ReadStmtData::GetModifiedVariable() { return modified_var_; }

PrintStmtData::PrintStmtData(StmtNum stmt_num, ProcIndex proc_index,
                             VarName used_var) {
  stmt_num_ = stmt_num;
  proc_index_ = proc_index;
  used_var_ = used_var;
}

VarName PrintStmtData::GetUsedVariable() { return used_var_; }

IfStmtData::IfStmtData(StmtNum stmt_num, ProcIndex proc_index,
                       VarNameSet control_vars, ConstValueSet used_consts) {
  stmt_num_ = stmt_num;
  proc_index_ = proc_index;
  control_vars_ = control_vars;
  used_consts_ = used_consts;
}

VarNameSet IfStmtData::GetUsedVariables() { return control_vars_; }

ConstValueSet IfStmtData::GetUsedConstants() { return used_consts_; }

CallStmtData::CallStmtData(StmtNum stmt_num, ProcIndex proc_index,
                           ProcName caller_proc_name,
                           ProcName callee_proc_name) {
  stmt_num_ = stmt_num;
  proc_index_ = proc_index;
  caller_proc_name_ = caller_proc_name;
  callee_proc_name_ = callee_proc_name;
}

ProcName CallStmtData::GetCallerProcName() { return caller_proc_name_; }

ProcName CallStmtData::GetCalleeProcName() { return callee_proc_name_; }
