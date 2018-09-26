#pragma once

#ifndef SPA_PARSE_DATA_H
#define SPA_PARSE_DATA_H

#include "pkb.h"

using StmtNumIntList = vector<int>;

class ParseData {
  StmtNumInt stmt_num_;
  StmtNumIntList stmt_num_list_;
  VarNameSet used_vars_;
  VarNameSet modified_vars_;
  VarName modified_var_;

 public:
  ParseData(StmtNumInt stmt_num, VarNameSet used_vars, VarNameSet modified_vars);
  ParseData(StmtNumIntList stmt_num_list, VarNameSet used_vars,
            VarNameSet modified_vars);
  ParseData(VarNameSet used_vars, VarNameSet modified_vars);
  ParseData(VarNameSet used_vars, VarName modified_var);
  StmtNumInt GetStmtNum();
  StmtNumIntList GetStmtNumList();
  VarNameSet GetUsedVariables();
  VarNameSet GetModifiedVariables();
  VarName GetModifiedVariable();
};

#endif  // !SPA_PARSE_DATA_H
