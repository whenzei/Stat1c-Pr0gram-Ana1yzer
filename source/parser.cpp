#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stack>

#include "parser.h"
#include "pkb.h"
#include "simple_validator.h"
#include "statement.h"
#include "tokenizer.h"

const bool DEBUG_FLAG = true;

using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::stack;

using tt = Tokenizer::TokenType;

// Constructor
Parser::Parser() {}
Parser::Parser(PKB* pkb) { pkb_ = pkb; }

// Setters
void Parser::SetPkb(PKB* pkb) { pkb_ = pkb; }

// Getters
TokenList Parser::GetTokenList() { return tokens_; }

PKB Parser::GetPkb() { return *pkb_; }

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

void Parser::Parse(string filepath) {
  // read content from file
  string contents = ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  if (DEBUG_FLAG) {
    for (Token& token : tokens_) {
      cout << Tokenizer::Debug(token) << endl;
    }
  }

  if (SimpleValidator::ValidateProcedure(tokens_, 0, tokens_.size() - 1)) {
    if (DEBUG_FLAG) {
      cout << "Procedure " << tokens_[1].value << " is syntactically correct"
           << endl;
    }
    ProcessProcedure(0, tokens_.size() - 1);
  }
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

void Parser::ProcessProcedure(size_t start, size_t end) {
  StmtNum stmt_num = 1;
  stack<string> indent_stack{};

  // Second index from start will always be a procedure name
  string procedure_name = tokens_[start + 1].value;
  if (DEBUG_FLAG) {
    std::cout << "Procedure added: " << procedure_name << endl;
  }

  pkb_->InsertProc(procedure_name);

  TokenList stmt_list;

  // Starts at 4th index and ends at 2nd last index
  for (size_t i = start + 3; i < end; i++) {
    Token curr_token = tokens_[i];

    if (curr_token.value == "{" || curr_token.value == "else") {
      indent_stack.push(curr_token.value);
    } else if (curr_token.value == "}") {
      indent_stack.pop();
      if (!indent_stack.empty() && indent_stack.top() == "else") {
        indent_stack.pop();
      }
    } else {
      stmt_list.push_back(curr_token);
    }

    if (stmt_list.size() > 0 &&
        (curr_token.type == tt::kSemicolon || curr_token.value == "{")) {
      Token first_token = stmt_list[0];

      // if it starts with a name, it must be an assignment
      if (first_token.type == tt::kName) {
        StmtAssignInfo stmt_info = GetAssignmentInfo(stmt_list);
        // InsertAssignStmt(1, 0, "x", <>)
        // 1 : STMT_NUM
        // 1 : STMTLIST_INDEX
        // "x" : VAR_NAME on the left hand side
        // <> : LIST_OF_VAR_NAME on the right hand side
        /*pkb_->InsertAssignStmt(stmt_num, stmt_list_index,
                                                       stmt_info.lhs_variable,
                                                       stmt_info.rhs_variables);*/
        if (DEBUG_FLAG) {
          string rhs_vars = string();
          for (string element : stmt_info.rhs_variables) {
            rhs_vars += element;
          }
          string rhs_consts = string();
          for (Variable element : stmt_info.rhs_constants) {
            rhs_consts += element;
          }
          cout << "Assignment statement " << stmt_num
               << " added, indent_lvl: " << indent_stack.size()
               << ", lhs: " << stmt_info.lhs_variable
               << ", rhs_vars: " << rhs_vars << ", rhs_consts: " << rhs_consts
               << endl;
        }
        stmt_num++;
      } else if (first_token.type == tt::kKeyword) {
        // todo: add to relevant tables like IfTable, etc
        if (first_token.value == "while" || first_token.value == "if") {
          /*InsertWhileStmt(3, 0, <"i">)
          3: STMT_NUM
          0 : STMTLIST_INDEX
          <“i”> : LIST_OF_VAR_NAME used as control variables*/
          VariableSet control_vars = GetControlVariables(stmt_list);

          // remember to remove 1 indentation level because it just added one up
          // top before coming down
          if (first_token.value == "while") {
            // pkb_->InsertWhileStmt(stmt_num, indent_stack.size() - 1,
            // control_vars);
          } else {
            // pkb_->InsertIfStmt(stmt_num, indent_stack.size() - 1,
            // control_vars);
          }

          if (DEBUG_FLAG) {
            string control_str = string();
            for (Variable element : control_vars) {
              control_str += element;
            }
            cout << first_token.value << " keyword statement " << stmt_num
                 << " added, control_vars: " << control_str
                 << ", indent_lvl: " << indent_stack.size() - 1 << endl;
          }
        }
        stmt_num++;
      } else if (first_token.type == tt::kBrace) {
        // this should not happen, since braces are not pushed into the
        // stmt_list
      }

      // clean stmt_list again
      stmt_list.clear();
    }
  }
}

StmtAssignInfo Parser::GetAssignmentInfo(TokenList stmt) {
  string lhs_var = stmt[0].value;
  VariableSet rhs_vars;
  VariableSet rhs_consts;

  for (size_t i = 1; i < stmt.size(); i++) {
    if (stmt[i].type == tt::kName) {
      rhs_vars.insert(stmt[i].value);
    } else if (stmt[i].type == tt::kDigit) {
      rhs_consts.insert(stmt[i].value);
    }
  }

  return StmtAssignInfo({lhs_var, rhs_vars, rhs_consts});
}

VariableSet Parser::GetControlVariables(TokenList stmt) {
  VariableSet control_vars;

  for (Token token : stmt) {
    if (token.type == tt::kName) {
      control_vars.insert(token.value);
    }
  }

  return control_vars;
}