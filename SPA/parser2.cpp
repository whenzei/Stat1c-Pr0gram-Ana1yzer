#pragma once

#include <fstream>
#include <iostream>
#include <stack>

#include "parser2.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::stack;

using tt = Tokenizer::TokenType;

Parser2::Parser2(PKB* pkb) {
  pkb_ = pkb;
  current_index_ = 0;
  stmt_index_ = 0;
  stmt_list_index_ = 0;
}

bool Parser2::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

string Parser2::ReadContentFromFile(string filepath) {
  if (!IsValidFile(filepath)) {
    cout << "File not found! Content is set to empty string" << endl;
    return string();
  }
  // start reading file
  ifstream ifs(filepath);
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

Token Parser2::ReadNextToken() {
  current_token_ = tokens_[current_index_++];
  return current_token_;
}

void Parser2::ProcessKeyword(int curr_stmt_list_index) {
  // while (current_token_.type != tt::kBrace) {
  //  ReadNextToken();
  //}
  if (current_token_.value == "if") {
    /*   while (current_token_.type != tt::kBrace) {
         ReadNextToken();
       }*/
    ProcessIfBlock(curr_stmt_list_index);
  } else if (current_token_.value == "procedure") {
    ReadNextToken();
  } else if (current_token_.value == "while") {
    ProcessWhileBlock(curr_stmt_list_index);
		}
}

void Parser2::ProcessAssignment(int curr_stmt_list_index) {
  // must be a kName since this function is called
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

  // TO DO: Integration with PKB
  // pkb_->InsertAssignStmt(stmt_index_, curr_stmt_list_index, lhs_var,
  // rhs_vars);

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

void Parser2::Parse(string filepath) {
  // read content from file
  string contents = ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  ReadNextToken();
  int curr_stmt_list_index = stmt_list_index_++;

  while (current_token_.type != tt::kEOF) {
    if (current_token_.type == tt::kKeyword) {
      ProcessKeyword(curr_stmt_list_index);
    } else if (current_token_.type == tt::kOpenBrace || current_token_.type == tt::kCloseBrace) {
    } else if (current_token_.type == tt::kName) {
      ProcessAssignment(curr_stmt_list_index);
    }
    ReadNextToken();
  }
}

void Parser2::ProcessIfBlock(int curr_stmt_list_index) {
  // Reads the first open bracket '('
  ReadNextToken();
  // Reads the first condition token
  ReadNextToken();

  int if_stmt_index = stmt_index_++;
  int inside_if_stmt_list_index = stmt_list_index_++;
  int inside_else_stmt_list_index = stmt_list_index_++;

  VariableSet control_var;

  // Reads the condition of the 'If' block
  while (current_token_.type != tt::kCloseParen) {
    if (current_token_.type == tt::kName) {
      control_var.insert(current_token_.value);
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
  for (Variable var : control_var) {
    control_var_str = control_var_str + " " + var;
  }
  cout << "If statement "
       << "added, indent_lvl: " << curr_stmt_list_index
       << ", control_variable: " << control_var_str << endl;
  //*************************************

  // To be uncommented: Integration with PKB
  // insertIfStmt.....
}

void Parser2::ProcessWhileBlock(int curr_stmt_list_index) {
  ReadNextToken();
  ReadNextToken();

  int while_stmt_index = stmt_index_++;
  int inside_while_stmt_list_index = stmt_list_index_++;

  VariableSet control_var;

  while (current_token_.type != tt::kCloseParen) {
    if (current_token_.type == tt::kName) {
      control_var.insert(current_token_.value);
    }
    ReadNextToken();
  }

  ReadNextToken();

  ProcessBlockContent(inside_while_stmt_list_index);
  // TO DO: Integration with PKB
  // insertWhileStmt.....
}

void Parser2::ProcessBlockContent(int curr_stmt_list_index) {
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
