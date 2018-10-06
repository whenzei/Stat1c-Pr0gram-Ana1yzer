#include "parse_data.h"

ParseData::ParseData() {
  stmt_num_list_ = StmtNumIntList();
  used_vars_ = VarNameSet();
  modified_vars_ = VarNameSet();
  modified_var_ = VarName();
}

ParseData::ParseData(StmtNumInt stmt_num, VarNameSet used_vars,
                     VarNameSet modified_vars) {
  stmt_num_ = stmt_num;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(StmtNumIntList stmt_num_list, VarNameSet used_vars,
                     VarNameSet modified_vars) {
  stmt_num_list_ = stmt_num_list;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(StmtNumInt stmt_num, VarNameSet used_vars,
                     VarNameSet modified_vars, StmtNumInt last_stmt_num_1,
                     StmtNumInt last_stmt_num_2) {
  last_stmt_num_1_ = last_stmt_num_1;
  last_stmt_num_2_ = last_stmt_num_2;
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

// default parameter for last_stmt_num_2 is 0
ParseData::ParseData(VarNameSet used_vars, VarNameSet modified_vars,
                     StmtNumInt last_stmt_num_1, StmtNumInt last_stmt_num_2) {
  last_stmt_num_1_ = last_stmt_num_1;
  last_stmt_num_2_ = last_stmt_num_2;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

StmtNumInt ParseData::GetStmtNum() { return stmt_num_; }

StmtNumIntList ParseData::GetStmtNumList() { return stmt_num_list_; }

VarNameSet ParseData::GetUsedVariables() { return used_vars_; }

VarNameSet ParseData::GetModifiedVariables() { return modified_vars_; }

VarName ParseData::GetModifiedVariable() { return modified_var_; }

StmtNumInt ParseData::GetLastStmtNumOne() { return last_stmt_num_1_; }

StmtNumInt ParseData::GetLastStmtNumTwo() { return last_stmt_num_2_; }
