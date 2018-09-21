#include "statement_data.h"

int StatementData::GetStmtListIndex() { return stmt_list_index_; }

string StatementData::GetStmtNum() { return stmt_num_; }

AssignStmtData::AssignStmtData(int stmt_num, int stmt_list_index,
                               VarName lhs_var, VarNameSet rhs_vars,
                               ConstValueSet rhs_consts,
                               TokenList postfixed_expr) {
  stmt_num_ = std::to_string(stmt_num);
  stmt_list_index_ = stmt_list_index_;
  lhs_var_ = lhs_var;
  rhs_vars_ = rhs_vars;
  rhs_consts_ = rhs_consts;
  postfixed_expr_ = postfixed_expr_;
}

VarName AssignStmtData::GetModifiedVariable() { return lhs_var_; }
VarNameSet AssignStmtData::GetUsedVariables() { return rhs_vars_; }
ConstValueSet AssignStmtData::GetUsedConstants() { return rhs_consts_; }
TokenList AssignStmtData::GetPostfixedExpr() { return postfixed_expr_; }

WhileStmtData::WhileStmtData(int stmt_num, int stmt_list_index,
                             int child_stmt_list_index, VarNameSet control_vars,
                             ConstValueSet used_consts) {
  stmt_num_ = std::to_string(stmt_num);
  stmt_list_index_ = stmt_list_index_;
  child_stmt_list_index_ = child_stmt_list_index;
  control_vars_ = control_vars;
  used_consts_ = used_consts;
}

StmtListIndex WhileStmtData::GetChildStmtListIndex() {
  return child_stmt_list_index_;
}

VarNameSet WhileStmtData::GetUsedVariables() { return control_vars_; }

ConstValueSet WhileStmtData::GetUsedConstants() { return used_consts_; }
