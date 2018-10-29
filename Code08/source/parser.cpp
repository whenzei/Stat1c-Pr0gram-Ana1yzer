#pragma once

#include <fstream>
#include <iostream>
#include <stack>

#include "expression_helper.h"
#include "parse_data.h"
#include "parser.h"
#include "pkb.h"
#include "statement_data.h"
#include "syntactic_error_exception.h"
#include "tokenizer.h"
#include "util.h"
#include "validator.h"

const bool DEBUG_FLAG = false;

using std::cout;
using std::endl;
using std::stack;

using tt = Tokenizer::TokenType;
using ts = Tokenizer::TokenSubtype;

const std::unordered_set<string> kKeywords({"procedure", "read", "call",
                                            "print", "if", "then", "else",
                                            "while"});

Parser::Parser(PKB* pkb) {
  pkb_ = pkb;
  current_index_ = 0;  // index for list of tokens
  stmt_num_ = 0;
}

bool Parser::Parse(string filepath) {
  // read content from file
  string contents = Util::ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  // validate tokens for syntax errors
  Validator validator = Validator(tokens_);
  if (!validator.ValidateProgram()) {
    cout << "Validation failed" << endl;
    return false;
  }

  ParseProgram();
  pkb_->NotifyParseEnd();

  return true;
}

// Method for testing which provides pre-written tokens of a program
bool Parser::Parse(TokenList program_tokenized) {
  tokens_ = program_tokenized;
  // validate tokens for syntax errors
  Validator validator = Validator(tokens_);
  if (!validator.ValidateProgram()) {
    return false;
  }

  ParseProgram();
  pkb_->NotifyParseEnd();

  return true;
}

void Parser::ParseProgram() {
  do {
    ProcessProcedure();
  } while (!IsAtEnd());

  /****** Debug Call graph ******/
  // DFS call graph
  pkb_->GetCallGraph();
  VertexList traverse = pkb_->GetCallGraph()->Toposort();
  for (auto id : traverse) {
    cout << pkb_->GetProcName(id) << endl;
  }
}

void Parser::ProcessProcedure() {
  ReadNextToken();
  current_proc_name_ = ReadNextToken().value;
  pkb_->InsertProcName(current_proc_name_);
  current_proc_index_ = pkb_->GetProcIndex(current_proc_name_);
  current_cfg_ = pkb_->GetCFG(current_proc_name_);
  // eat the open brace
  ReadNextToken();

  ParseData parse_data = ProcessStatementList();

  PopulatePkbModifies(current_proc_name_, parse_data.GetModifiedVariables());
  PopulatePkbUses(current_proc_name_, parse_data.GetUsedVariables());

  ReadNextToken();
}

ParseData Parser::ProcessStatementList() {
  StmtNumList stmt_nums;
  VarNameSet modified_vars;
  VarNameSet used_vars;

  StmtNumList nested_last_stmts_1;
  StmtNumList nested_last_stmts_2;
  StmtNum prev_stmt_num = 0;

  bool is_last_statement_if = false;

  do {
    ParseData stmt_info = ProcessStatement();
    StmtNum stmt_num = stmt_info.GetStmtNum();
    VarNameSet used_vars_to_insert = stmt_info.GetUsedVariables();
    VarNameSet modified_vars_to_insert = stmt_info.GetModifiedVariables();

    //******* Updates cfg****************
    if (prev_stmt_num != 0 && nested_last_stmts_1.empty() &&
        nested_last_stmts_2.empty()) {
      pkb_->InsertNext(current_proc_name_, prev_stmt_num, stmt_num);
    }

    if (!nested_last_stmts_1.empty()) {
      for (auto& nested_stmt : nested_last_stmts_1) {
        pkb_->InsertNext(current_proc_name_, nested_stmt, stmt_num);
      }
    }

    if (!nested_last_stmts_2.empty()) {
      for (auto& nested_stmt : nested_last_stmts_2) {
        pkb_->InsertNext(current_proc_name_, nested_stmt, stmt_num);
      }
    }

    nested_last_stmts_1 = stmt_info.GetNestedLastStmtsOne();
    nested_last_stmts_2 = stmt_info.GetNestedLastStmtsTwo();

    is_last_statement_if =
        !nested_last_stmts_1.empty() || !nested_last_stmts_2.empty();

    prev_stmt_num = stmt_num;
    //************************************

    // Keep track of all statements in the statement list
    stmt_nums.push_back(stmt_num);
    // Keep track of all modified variables
    modified_vars.insert(modified_vars_to_insert.begin(),
                         modified_vars_to_insert.end());
    // Keep track of all used variables
    used_vars.insert(used_vars_to_insert.begin(), used_vars_to_insert.end());
  } while (!IsNextType(tt::kCloseBrace));

  PopulatePkbFollows(stmt_nums);
  if (!is_last_statement_if) {
    nested_last_stmts_1.push_back(prev_stmt_num);
    return ParseData(stmt_nums, used_vars, modified_vars, nested_last_stmts_1,
                     nested_last_stmts_2);
  } else {
    return ParseData(stmt_nums, used_vars, modified_vars, nested_last_stmts_1,
                     nested_last_stmts_2);
  }
}

ParseData Parser::ProcessStatement() {
  int curr_stmt_num = ++stmt_num_;

  VarNameSet used_vars;
  VarNameSet modified_vars;

  ReadNextToken();
  if (IsCurrentTokenKeyword() && !IsNextType(tt::kAssignment)) {
    ParseData keyword_stmt_info = ProcessKeyword();
    used_vars = keyword_stmt_info.GetUsedVariables();
    modified_vars = keyword_stmt_info.GetModifiedVariables();

    StmtNumList nested_last_stmts_1 = keyword_stmt_info.GetNestedLastStmtsOne();
    StmtNumList nested_last_stmts_2 = keyword_stmt_info.GetNestedLastStmtsTwo();

    return ParseData(curr_stmt_num, used_vars, modified_vars,
                     nested_last_stmts_1, nested_last_stmts_2);

  } else {
    ParseData assignment_stmt_info = ProcessAssignment();
    used_vars = assignment_stmt_info.GetUsedVariables();
    modified_vars.insert(assignment_stmt_info.GetModifiedVariable());
    return ParseData(curr_stmt_num, used_vars, modified_vars);
  }
}

ParseData Parser::ProcessKeyword() {
  switch (current_token_.subtype) {
    case ts::kIf:
      return ProcessIfBlock();
    case ts::kCall:
      // Modifies and Used not processed, DE will populate later
      return ProcessCall();
    case ts::kWhile:
      return ProcessWhileBlock();
    case ts::kRead:
      return ProcessRead();
    case ts::kPrint:
      return ProcessPrint();
  }

  throw SyntacticErrorException("Unknown keyword detected.");
}

ParseData Parser::ProcessRead() {
  VarName modified_var = ReadNextToken().value;
  pkb_->InsertReadStmt(
      &ReadStmtData(stmt_num_, current_proc_index_, modified_var));

  PopulatePkbModifies(stmt_num_, modified_var);

  // eat semicolon
  ReadNextToken();
  return ParseData(VarNameSet(), VarNameSet{modified_var});
}

ParseData Parser::ProcessPrint() {
  VarName used_var = ReadNextToken().value;
  pkb_->InsertPrintStmt(
      &PrintStmtData(stmt_num_, current_proc_index_, used_var));

  PopulatePkbUses(stmt_num_, used_var);

  // eat semicolon
  ReadNextToken();
  return ParseData(VarNameSet{used_var}, VarNameSet());
}

ParseData Parser::ProcessCall() {
  VarName called_proc_name = ReadNextToken().value;
  pkb_->InsertEdgeInCallGraph(current_proc_name_, called_proc_name);
  pkb_->InsertCallStmt(&CallStmtData(stmt_num_, current_proc_index_,
                                     current_proc_name_, called_proc_name));

  // eat semicolon
  ReadNextToken();
  // empty parse data
  return ParseData();
}

ParseData Parser::ProcessAssignment() {
  VarName lhs_var = current_token_.value;
  VarNameSet rhs_vars;
  ConstValueSet rhs_consts;
  TokenList infix_tokens_rhs;
  TokenList postfix_tokens_rhs;

  while (!IsCurrentType(tt::kSemicolon)) {
    ReadNextToken();
    if (IsCurrentType(tt::kName)) {
      rhs_vars.insert(current_token_.value);
    } else if (IsCurrentType(tt::kDigit)) {
      rhs_consts.insert(stoi(current_token_.value));
    }
    infix_tokens_rhs.push_back(current_token_);
  }

  // postfix_tokens_rhs to be passed into PKB
  postfix_tokens_rhs = ExpressionHelper::ToPostfix(infix_tokens_rhs);

  // Update PKB of assignment statement
  pkb_->InsertAssignStmt(&AssignStmtData(stmt_num_, current_proc_index_,
                                         lhs_var, rhs_vars, rhs_consts,
                                         postfix_tokens_rhs));

  PopulatePkbModifies(stmt_num_, lhs_var);
  PopulatePkbUses(stmt_num_, rhs_vars);

  if (DEBUG_FLAG) {
    //**************** DEBUG********************
    string rhsvar = string();
    for (VarName var : rhs_vars) {
      rhsvar = rhsvar + " " + var;
    }
    string rhsconst = string();
    for (int var : rhs_consts) {
      rhsconst = rhsconst + " " + std::to_string(var);
    }

    cout << "Assignment statement#" << stmt_num_
         << " added, lhs: " << lhs_var
         << ", rhs_vars: " << rhsvar << ", rhs_consts: " << rhsconst << endl;
    //***********************************************
  }

  return ParseData(rhs_vars, lhs_var);
}

ParseData Parser::ProcessIfBlock() {
  int if_stmt_num = stmt_num_;
  pair<VarNameSet, ConstValueSet> used_set_conditionals = ProcessConditional();

  if (DEBUG_FLAG) {
    //************* DEBUG******************
    string control_var_str = string();
    for (VarName var : used_set_conditionals.first) {
      control_var_str = control_var_str + " " + var;
    }
    cout << "If statement#" << if_stmt_num
         << " added, control_variable: " << control_var_str << endl;
    //*************************************
  }

  // eat 'then {' tokens
  EatNumTokens(2);

  if (DEBUG_FLAG) {
    cout << "[inside then block]" << endl;
  }
  // Process everything inside the if block
  ParseData then_stmt_info = ProcessStatementList();

  // eat "} else {" tokens
  EatNumTokens(3);

  if (DEBUG_FLAG) {
    cout << "[inside else block]" << endl;
  }
  // Process everything inside the counterpart else block
  ParseData else_stmt_info = ProcessStatementList();

  // Get all child statements in the then and else block
  // Used for populating Parent relation in PKB
  StmtNumList then_stmt_nums = then_stmt_info.GetStmtNumList();
  StmtNumList else_stmt_nums = else_stmt_info.GetStmtNumList();

  // Handles cfg
  pkb_->InsertNext(current_proc_name_, if_stmt_num, then_stmt_nums.at(0));
  pkb_->InsertNext(current_proc_name_, if_stmt_num, else_stmt_nums.at(0));

  StmtNumList child_stmt_nums(then_stmt_nums);
  child_stmt_nums.insert(child_stmt_nums.end(), else_stmt_nums.begin(),
                         else_stmt_nums.end());

  PopulatePkbParent(if_stmt_num, child_stmt_nums);

  // Get all used vars in the then and else block
  // Used for populating Uses relation in PKB
  VarNameSet then_used_vars = then_stmt_info.GetUsedVariables();
  VarNameSet else_used_vars = else_stmt_info.GetUsedVariables();

  VarNameSet used_vars(used_set_conditionals.first);
  used_vars.insert(then_used_vars.begin(), then_used_vars.end());
  used_vars.insert(else_used_vars.begin(), else_used_vars.end());

  PopulatePkbUses(if_stmt_num, used_vars);

  // Get all used vars in the then and else block
  // Used for populating Uses relation in PKB
  VarNameSet then_modified_vars = then_stmt_info.GetModifiedVariables();
  VarNameSet else_modified_vars = else_stmt_info.GetModifiedVariables();

  VarNameSet modified_vars(then_modified_vars);
  modified_vars.insert(else_modified_vars.begin(), else_modified_vars.end());

  PopulatePkbModifies(if_stmt_num, modified_vars);

  // eat closing brace
  ReadNextToken();

  if (DEBUG_FLAG) {
    cout << "[leaving if block]" << endl;
  }

  // Update PKB of the 'if' block
  pkb_->InsertIfStmt(&IfStmtData(if_stmt_num, current_proc_index_,
                                 used_set_conditionals.first,
                                 used_set_conditionals.second));

  // Find last stmts
  StmtNumList then_nested_last_stmts_1 = then_stmt_info.GetNestedLastStmtsOne();
  StmtNumList then_nested_last_stmts_2 = then_stmt_info.GetNestedLastStmtsTwo();
  StmtNumList else_nested_last_stmts_1 = else_stmt_info.GetNestedLastStmtsOne();
  StmtNumList else_nested_last_stmts_2 = else_stmt_info.GetNestedLastStmtsTwo();

  if (then_nested_last_stmts_1.size() == 0 &&
      then_nested_last_stmts_2.size() == 0 &&
      else_nested_last_stmts_1.size() == 0 &&
      else_nested_last_stmts_2.size() == 0) {
    StmtNumList then_nested_last_stmts;
    StmtNumList else_nested_last_stmts;

    then_nested_last_stmts.push_back(then_stmt_nums.back());
    else_nested_last_stmts.push_back(else_stmt_nums.back());

    return ParseData(used_vars, modified_vars, then_nested_last_stmts,
                     else_nested_last_stmts);

  } else {
    StmtNumList then_nested_last_stmts(then_nested_last_stmts_1);
    StmtNumList else_nested_last_stmts(else_nested_last_stmts_1);

    then_nested_last_stmts.insert(then_nested_last_stmts.end(),
                                  then_nested_last_stmts_2.begin(),
                                  then_nested_last_stmts_2.end());
    else_nested_last_stmts.insert(else_nested_last_stmts.end(),
                                  else_nested_last_stmts_2.begin(),
                                  else_nested_last_stmts_2.end());

    return ParseData(used_vars, modified_vars, then_nested_last_stmts,
                     else_nested_last_stmts);
  }
}

ParseData Parser::ProcessWhileBlock() {
  int while_stmt_num = stmt_num_;
  pair<VarNameSet, ConstValueSet> used_set_conditional = ProcessConditional();

  if (DEBUG_FLAG) {
    //************* DEBUG******************
    string control_var_str = string();
    for (VarName var : used_set_conditional.first) {
      control_var_str = control_var_str + " " + var;
    }
    cout << "While statement#" << while_stmt_num
         << " added, control_variable: " << control_var_str << endl;
    //*************************************
  }

  // eat open brace
  ReadNextToken();

  if (DEBUG_FLAG) {
    cout << "[inside while block]" << endl;
  }

  ParseData children_stmt_info = ProcessStatementList();
  StmtNumList children_stmt_nums = children_stmt_info.GetStmtNumList();
  VarNameSet used_vars = children_stmt_info.GetUsedVariables();
  VarNameSet modified_vars = children_stmt_info.GetModifiedVariables();
  StmtNumList nested_last_stmts_1 = children_stmt_info.GetNestedLastStmtsOne();
  StmtNumList nested_last_stmts_2 = children_stmt_info.GetNestedLastStmtsTwo();

  // Update variables used in conditionals
  used_vars.insert(used_set_conditional.first.begin(),
                   used_set_conditional.first.end());

  PopulatePkbParent(while_stmt_num, children_stmt_nums);
  PopulatePkbUses(while_stmt_num, used_vars);
  PopulatePkbModifies(while_stmt_num, modified_vars);

  pkb_->InsertNext(current_proc_name_, while_stmt_num,
                   children_stmt_nums.at(0));

  // When the the last statement in the while block is if
  // Grab last statement of then and else block
  if (nested_last_stmts_1.size() > 0 && nested_last_stmts_2.size() > 0) {
    for (auto& stmt_num : nested_last_stmts_1) {
      pkb_->InsertNext(current_proc_name_, stmt_num, while_stmt_num);
    }

    for (auto& stmt_num : nested_last_stmts_2) {
      pkb_->InsertNext(current_proc_name_, stmt_num, while_stmt_num);
    }
  } else {
    pkb_->InsertNext(current_proc_name_, children_stmt_nums.back(),
                     while_stmt_num);
  }

  // eat close brace
  ReadNextToken();

  if (DEBUG_FLAG) {
    cout << "[leaving while block]" << endl;
  }

  // Update PKB of the 'while' block
  pkb_->InsertWhileStmt(&WhileStmtData(while_stmt_num, current_proc_index_,
                                       used_set_conditional.first,
                                       used_set_conditional.second));

  return ParseData(used_vars, modified_vars);
}

pair<VarNameSet, ConstValueSet> Parser::ProcessConditional() {
  stack<Token> paren_stack;

  // Push "(" onto stack
  paren_stack.push(ReadNextToken());
  ReadNextToken();

  VarNameSet control_vars;
  ConstValueSet used_consts;

  while (!paren_stack.empty()) {
    if (IsCurrentType(tt::kOpenParen)) {
      paren_stack.push(current_token_);
    } else if (IsCurrentType(tt::kName)) {
      control_vars.insert(current_token_.value);
    } else if (IsCurrentType(tt::kDigit)) {
      used_consts.insert(stoi(current_token_.value));
    } else if (IsCurrentType(tt::kCloseParen)) {
      paren_stack.pop();

      if (paren_stack.empty()) {
        break;
      }
    }

    // Stops reading the next token when at the end of conditional ')'
    ReadNextToken();
  }

  return make_pair(control_vars, used_consts);
}

void Parser::PopulatePkbFollows(StmtNumList stmt_nums) {
  for (size_t i = 0; i < stmt_nums.size(); i++) {
    for (size_t j = i + 1; j < stmt_nums.size(); j++) {
      pkb_->InsertFollows(stmt_nums[i], stmt_nums[j]);
    }
  }
}

void Parser::PopulatePkbParent(int parent_num, StmtNumList children_stmt_nums) {
  for (size_t i = 0; i < children_stmt_nums.size(); i++) {
    pkb_->InsertParent(parent_num, children_stmt_nums[i]);
  }
}

void Parser::PopulatePkbUses(int stmt_num, VarNameSet used_vars) {
  for (const auto& var : used_vars) {
    pkb_->InsertUsesS(stmt_num, var);
  }
}

void Parser::PopulatePkbUses(ProcName proc_name, VarNameSet used_vars) {
  for (const auto& var : used_vars) {
    pkb_->InsertUsesP(proc_name, var);
  }
}

void Parser::PopulatePkbUses(int stmt_num, VarName used_var) {
  pkb_->InsertUsesS(stmt_num, used_var);
}

void Parser::PopulatePkbModifies(int stmt_num, VarNameSet modified_vars) {
  for (const auto& var : modified_vars) {
    pkb_->InsertModifiesS(stmt_num, var);
  }
}

void Parser::PopulatePkbModifies(ProcName proc_name, VarNameSet modified_vars) {
  for (const auto& var : modified_vars) {
    pkb_->InsertModifiesP(proc_name, var);
  }
}

void Parser::PopulatePkbModifies(int stmt_num, VarName modified_var) {
  pkb_->InsertModifiesS(stmt_num, modified_var);
}

// Helper methods
bool Parser::IsAtEnd() { return PeekNextToken().type == tt::kEOF; }

bool Parser::IsNextType(Tokenizer::TokenType type) {
  return PeekNextToken().type == type;
}

bool Parser::IsCurrentType(Tokenizer::TokenType type) {
  return current_token_.type == type;
}

bool Parser::IsCurrentKeywordType(Tokenizer::TokenSubtype type) {
  return current_token_.subtype == type;
}

bool Parser::IsNextKeywordType(Tokenizer::TokenSubtype type) {
  return PeekNextToken().subtype == type;
}

bool Parser::IsCurrentTokenKeyword() {
  return current_token_.subtype != ts::kNone;
}

bool Parser::IsNextTokenKeyword() {
  return PeekNextToken().subtype != Tokenizer::TokenSubtype::kNone;
}

Token Parser::ReadNextToken() {
  if (!IsAtEnd()) {
    current_index_++;
  }
  current_token_ = tokens_[current_index_ - 1];
  return current_token_;
}

Token Parser::PeekNextToken() { return tokens_[current_index_]; }

void Parser::EatNumTokens(int num) {
  current_index_ += num;
  current_token_ = tokens_[current_index_];
}
