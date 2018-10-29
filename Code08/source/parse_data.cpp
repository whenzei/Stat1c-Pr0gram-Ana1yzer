#include "parse_data.h"

ParseData::ParseData() {
  stmt_num_list_ = StmtNumList();
  used_vars_ = VarNameSet();
  modified_vars_ = VarNameSet();
  modified_var_ = VarName();
}

ParseData::ParseData(StmtNum stmt_num, VarNameSet used_vars,
                     VarNameSet modified_vars) {
  stmt_num_ = stmt_num;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(StmtNumList stmt_num_list, VarNameSet used_vars,
                     VarNameSet modified_vars) {
  stmt_num_list_ = stmt_num_list;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(StmtNumList stmt_num_list, VarNameSet used_vars,
                     VarNameSet modified_vars, StmtNumList nested_last_stmts_1,
                     StmtNumList nested_last_stmts_2) {
  nested_last_stmts_1_ = nested_last_stmts_1;
  nested_last_stmts_2_ = nested_last_stmts_2;
  stmt_num_list_ = stmt_num_list;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(StmtNum stmt_num, VarNameSet used_vars,
                     VarNameSet modified_vars, StmtNumList nested_last_stmts_1,
                     StmtNumList nested_last_stmts_2) {
  nested_last_stmts_1_ = nested_last_stmts_1;
  nested_last_stmts_2_ = nested_last_stmts_2;
  stmt_num_ = stmt_num;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(VarNameSet used_vars, VarNameSet modified_vars) {
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(VarNameSet used_vars, VarName modified_var) {
  used_vars_ = used_vars;
  modified_var_ = modified_var;
}

ParseData::ParseData(VarNameSet used_vars, VarNameSet modified_vars,
                     StmtNumList nested_last_stmts_1,
                     StmtNumList nested_last_stmts_2) {
  nested_last_stmts_1_ = nested_last_stmts_1;
  nested_last_stmts_2_ = nested_last_stmts_2;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

StmtNum ParseData::GetStmtNum() { return stmt_num_; }

StmtNumList ParseData::GetStmtNumList() { return stmt_num_list_; }

VarNameSet ParseData::GetUsedVariables() { return used_vars_; }

VarNameSet ParseData::GetModifiedVariables() { return modified_vars_; }

VarName ParseData::GetModifiedVariable() { return modified_var_; }

StmtNumList ParseData::GetNestedLastStmtsOne() { return nested_last_stmts_1_; }

StmtNumList ParseData::GetNestedLastStmtsTwo() { return nested_last_stmts_2_; }
