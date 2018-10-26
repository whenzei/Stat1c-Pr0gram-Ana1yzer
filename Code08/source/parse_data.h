#pragma once

#ifndef SPA_PARSE_DATA_H
#define SPA_PARSE_DATA_H

#include "pkb.h"

class ParseData {
  StmtNum stmt_num_ = 0;
  StmtNum last_stmt_num_1_ = 0;
  StmtNum last_stmt_num_2_ = 0;
  StmtNumList stmt_num_list_;
  StmtNumList nested_last_stmts_1_;
  StmtNumList nested_last_stmts_2_;
  VarNameSet used_vars_;
  VarNameSet modified_vars_;
  VarName modified_var_;

 public:
  // Default constructor, all variables are initialized to empty
  ParseData();
  ParseData(StmtNum stmt_num, VarNameSet used_vars,
            VarNameSet modified_vars);

  ParseData(StmtNum stmt_num, VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumList nested_last_stmts_1, StmtNumList nested_last_stmts_2);

  ParseData(StmtNumList stmt_num_list, VarNameSet used_vars,
            VarNameSet modified_vars);

  ParseData(StmtNumList stmt_num_list, VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumList nested_last_stmts_1, StmtNumList nested_last_stmts_2);

  ParseData(VarNameSet used_vars, VarNameSet modified_vars);

  // For if in if relation, and if in else relation
  ParseData(VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumList nested_last_stmts_1, StmtNumList nest_last_stmts_2);

  ParseData(VarNameSet used_vars, VarName modified_var);
  StmtNum GetStmtNum();
  StmtNumList GetStmtNumList();
  VarNameSet GetUsedVariables();
  VarNameSet GetModifiedVariables();
  VarName GetModifiedVariable();
  StmtNumList ParseData::GetNestedLastStmtsOne();
  StmtNumList ParseData::GetNestedLastStmtsTwo();
};

#endif  // !SPA_PARSE_DATA_H
