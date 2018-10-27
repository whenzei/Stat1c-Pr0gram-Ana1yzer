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

  // Constructor to initialize only StmtNum, used and modified vars
  ParseData(StmtNum stmt_num, VarNameSet used_vars, VarNameSet modified_vars);

  // Constructor to initialize with a single modified variable name and a set of
  // used variables
  ParseData(VarNameSet used_vars, VarName modified_var);

  ParseData(StmtNum stmt_num, VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumList nested_last_stmts_1, StmtNumList nested_last_stmts_2);

  ParseData(StmtNumList stmt_num_list, VarNameSet used_vars,
            VarNameSet modified_vars);

  ParseData(StmtNumList stmt_num_list, VarNameSet used_vars,
            VarNameSet modified_vars, StmtNumList nested_last_stmts_1,
            StmtNumList nested_last_stmts_2);

  ParseData(VarNameSet used_vars, VarNameSet modified_vars);

  // Constructor for if in if relation, and if in else relation
  ParseData(VarNameSet used_vars, VarNameSet modified_vars,
            StmtNumList nested_last_stmts_1, StmtNumList nest_last_stmts_2);

  // Get the statement number in this object
  // @returns statement number in this object
  StmtNum GetStmtNum();

  // Get the list of all statement numbers in this object
  // @returns vector of statement numbers in this object
  StmtNumList GetStmtNumList();

  // Get all used variables in this object
  // @returns set of all used variables in this object
  VarNameSet GetUsedVariables();

  // Get all modified variables in this object
  // @returns set of all modified variables in this object
  VarNameSet GetModifiedVariables();

  // Get all the only modified variable in this object
  // Precondition: The object was initialized with the single modified variable
  // constructor
  // @returns the modified variable in this object
  VarName GetModifiedVariable();

  // @returns vector of statements denoting the last statements of all
  // (including nested blocks) "then" or the "while" block in this object
  // (if relevant)
  StmtNumList ParseData::GetNestedLastStmtsOne();

  // @returns vector of statements denoting the last statements of all
  // (including nested blocks) "else" block in this object (if relevant)
  StmtNumList ParseData::GetNestedLastStmtsTwo();
};

#endif  // !SPA_PARSE_DATA_H
