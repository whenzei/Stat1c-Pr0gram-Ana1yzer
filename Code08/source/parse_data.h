#pragma once

#ifndef SPA_PARSE_DATA_H
#define SPA_PARSE_DATA_H

#include "pkb.h"

using StmtNumIntList = vector<int>;

class ParseData {
  StmtNumInt stmt_num_ = 0;
  StmtNumInt last_stmt_num_1_ = 0;
  StmtNumInt last_stmt_num_2_ = 0;
  StmtNumIntList stmt_num_list_;
  StmtNumIntList nested_last_stmts_1_;
  StmtNumIntList nested_last_stmts_2_;
  VarNameSet used_vars_;
  VarNameSet modified_vars_;
  VarName modified_var_;

 public:
  // Default constructor, all variables are initialized to empty
  ParseData();
  ParseData(StmtNumInt stmt_num, VarNameSet used_vars,
            VarNameSet modified_vars);

  ParseData(StmtNumInt stmt_num, VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumIntList nested_last_stmts_1, StmtNumIntList nested_last_stmts_2);

  ParseData(StmtNumIntList stmt_num_list, VarNameSet used_vars,
            VarNameSet modified_vars);

  ParseData(StmtNumIntList stmt_num_list, VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumIntList nested_last_stmts_1, StmtNumIntList nested_last_stmts_2);

  ParseData(VarNameSet used_vars, VarNameSet modified_vars);

  // For if in if relation, and if in else relation
  ParseData(VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumIntList nested_last_stmts_1, StmtNumIntList nest_last_stmts_2);

  ParseData(VarNameSet used_vars, VarName modified_var);
  StmtNumInt GetStmtNum();
  StmtNumIntList GetStmtNumList();
  VarNameSet GetUsedVariables();
  VarNameSet GetModifiedVariables();
  VarName GetModifiedVariable();
  StmtNumIntList ParseData::GetNestedLastStmtsOne();
  StmtNumIntList ParseData::GetNestedLastStmtsTwo();
};

#endif  // !SPA_PARSE_DATA_H
