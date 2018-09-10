#pragma once

#include <fstream>
#include <iostream>
#include <stack>

#include "parser.h"
#include "pkb.h"
#include "simple_validator.h"
#include "simple_validator2.h"
#include "tokenizer.h"

const bool DEBUG_FLAG = true;

using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::stack;

using tt = Tokenizer::TokenType;

Parser::Parser(PKB* pkb) {
  pkb_ = pkb;
  current_index_ = 0;
  stmt_index_ = 1;
  stmt_list_index_ = 0;
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

string Parser::ReadContentFromFile(string filepath) {
  if (!IsValidFile(filepath)) {
    cout << "File not found! Content is set to empty string" << endl;
    return string();
  }
  // start reading file
  ifstream ifs(filepath);
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

Token Parser::ReadNextToken() {
  current_token_ = tokens_[current_index_++];
  return current_token_;
}

void Parser::ProcessKeyword(int curr_stmt_list_index) {
  if (current_token_.value == "if") {
    ProcessIfBlock(curr_stmt_list_index);
  } else if (current_token_.value == "procedure") {
    ReadNextToken();
  } else if (current_token_.value == "while") {
    ProcessWhileBlock(curr_stmt_list_index);
  }
}

void Parser::ProcessAssignment(int curr_stmt_list_index) {

  int assign_stmt_index = stmt_index_++;
  string lhs_var = current_token_.value;
  VariableSet rhs_vars;
  VariableSet rhs_consts;

  while (current_token_.type != tt::kSemicolon) {
    ReadNextToken();
    if (current_token_.type == tt::kName) {
      rhs_vars.insert(current_token_.value);
    }
    if (current_token_.type == tt::kDigit) {
      rhs_consts.insert(current_token_.value);
    }
  }

  // Update PKB of assignment statement
   pkb_->InsertAssignStmt(assign_stmt_index, curr_stmt_list_index, lhs_var,
   rhs_vars);

  //**************** DEBUG********************
  string rhsvar = string();
  for (Variable var : rhs_vars) {
    rhsvar = rhsvar + " " + var;
  }
  string rhsconst = string();
  for (Variable var : rhs_consts) {
    rhsconst = rhsconst + " " + var;
  }

  cout << "Assignment statement "
       << " added, indent_lvl: " << curr_stmt_list_index << ", lhs: " << lhs_var
       << ", rhs_vars: " << rhsvar << ", rhs_consts: " << rhsconst << endl;
  //***********************************************
}

void Parser::Parse(string filepath) {
  // read content from file
  string contents = ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

		//TO DO: Implement Validator

  ReadNextToken();
  int curr_stmt_list_index = stmt_list_index_++;

  while (current_token_.type != tt::kEOF) {
    if (current_token_.type == tt::kKeyword) {
      ProcessKeyword(curr_stmt_list_index);
    } else if (current_token_.type == tt::kOpenBrace ||
               current_token_.type == tt::kCloseBrace) {
    } else if (current_token_.type == tt::kName) {
      ProcessAssignment(curr_stmt_list_index);
    } else { //Should not happen
      exit(1);
				}
    ReadNextToken();
  }
}

void Parser::ProcessIfBlock(int curr_stmt_list_index) {
  // Reads the first open bracket '('
  ReadNextToken();
  // Reads the first condition token
  ReadNextToken();

  int if_stmt_index = stmt_index_++;
  int inside_if_stmt_list_index = stmt_list_index_++;
  int inside_else_stmt_list_index = stmt_list_index_++;

  VariableSet control_vars;

  // Reads the condition of the 'If' block
  while (current_token_.type != tt::kCloseParen) {
    if (current_token_.type == tt::kName) {
      control_vars.insert(current_token_.value);
    }
    ReadNextToken();
  }

  // Open brace '{' of if block
  ReadNextToken();
  ReadNextToken();

  // Process everything inside the if block
  ProcessBlockContent(inside_if_stmt_list_index);

  ReadNextToken();

  // Process everything inside the counterpart else block
  ReadNextToken();
  ProcessBlockContent(inside_else_stmt_list_index);

  //************* DEBUG******************
  string control_var_str = string();
  for (Variable var : control_vars) {
    control_var_str = control_var_str + " " + var;
  }
  cout << "If statement "
       << "added, indent_lvl: " << curr_stmt_list_index
       << ", control_variable: " << control_var_str << endl;
  //*************************************

  // Update PKB of the 'if' block
  pkb_->InsertIfStmt(if_stmt_index, curr_stmt_list_index,
                     inside_if_stmt_list_index, inside_else_stmt_list_index,
                     control_vars);
}

void Parser::ProcessWhileBlock(int curr_stmt_list_index) {
  ReadNextToken();
  ReadNextToken();

  int while_stmt_index = stmt_index_++;
  int inside_while_stmt_list_index = stmt_list_index_++;

  VarNameSet control_vars;

  while (current_token_.type != tt::kCloseParen) {
    if (current_token_.type == tt::kName) {
      control_vars.insert(current_token_.value);
    }
    ReadNextToken();
  }

  ReadNextToken();

  ProcessBlockContent(inside_while_stmt_list_index);

		//************* DEBUG******************
  string control_var_str = string();
  for (Variable var : control_vars) {
    control_var_str = control_var_str + " " + var;
  }
  cout << "While statement "
       << "added, indent_lvl: " << curr_stmt_list_index
       << ", control_variable: " << control_var_str << endl;
  //*

  // Update PKB of the 'while' block
  pkb_->InsertWhileStmt(while_stmt_index, curr_stmt_list_index,
                        inside_while_stmt_list_index, control_vars);
}

void Parser::ProcessBlockContent(int curr_stmt_list_index) {
  while (current_token_.value.compare("}") != 0) {
    if (current_token_.type == tt::kName) {
      ProcessAssignment(curr_stmt_list_index);
    } else if (current_token_.type == tt::kKeyword) {
      if (current_token_.value.compare("if") == 0) {
        ProcessIfBlock(curr_stmt_list_index);
      }

      if (current_token_.value.compare("while") == 0) {
        ProcessWhileBlock(curr_stmt_list_index);
      }
    }
    ReadNextToken();
  }
}
