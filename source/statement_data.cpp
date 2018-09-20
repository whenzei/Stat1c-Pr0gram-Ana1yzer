#include "statement_data.h"

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