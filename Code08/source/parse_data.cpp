#include "parse_data.h"

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

ParseData::ParseData(VarNameSet used_vars, VarNameSet modified_vars) {
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(VarNameSet used_vars, VarName modified_var) {
  used_vars_ = used_vars;
  modified_var_ = modified_var;
}

StmtNumInt ParseData::GetStmtNum() { return stmt_num_; }

StmtNumIntList ParseData::GetStmtNumList() { return stmt_num_list_; }

VarNameSet ParseData::GetUsedVariables() { return used_vars_; }

VarNameSet ParseData::GetModifiedVariables() { return modified_vars_; }

VarName ParseData::GetModifiedVariable() { return modified_var_; }
