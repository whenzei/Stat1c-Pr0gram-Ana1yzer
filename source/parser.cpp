#pragma once

#include <fstream>
#include <iostream>
#include <stack>

#include "expression_helper.h"
#include "parser.h"
#include "pkb.h"
#include "statement_data.h"
#include "tokenizer.h"
#include "util.h"
#include "validator.h"

const bool DEBUG_FLAG = true;

using std::cout;
using std::endl;
using std::stack;
using std::vector;
using std::tuple;
using std::get;

using tt = Tokenizer::TokenType;
using ts = Tokenizer::TokenSubtype;
using StmtNumListInt = vector<int>;

const std::unordered_set<string> kKeywords({"procedure", "read", "call",
                                            "print", "if", "then", "else",
                                            "while"});

Parser::Parser(PKB* pkb) {
  pkb_ = pkb;
  current_index_ = 0;  // index for list of tokens
  stmt_num_ = 0;
  stmt_list_num_ = 1;
}

void Parser::Parse(string filepath) {
  // read content from file
  string contents = Util::ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  // validate tokens for syntax errors
  Validator validator = Validator(tokens_);
  if (!validator.ValidateProgram()) {
    return;
  }

  ParseProgram();
}

// Method for testing which provides pre-written tokens of a program
void Parser::Parse(TokenList program_tokenized) {
  tokens_ = program_tokenized;
  // validate tokens for syntax errors
  Validator validator = Validator(tokens_);
  if (!validator.ValidateProgram()) {
    return;
  }

  ParseProgram();
}

void Parser::ParseProgram() {
  do {
    ProcessProcedure(stmt_list_num_);
  } while (!IsAtEnd());
}

void Parser::ProcessProcedure(int given_stmt_list_index) {
  ReadNextToken();
  pkb_->InsertProcName(ReadNextToken().value);
  // eat the open brace
  ReadNextToken();

  ProcessStatementList(given_stmt_list_index);

  ReadNextToken();
}

StmtNumListInt Parser::ProcessStatementList(int given_stmt_list_num) {
  StmtNumListInt stmt_nums;
  VarNameSet modified_vars;
  VarNameSet used_vars;
  do {
    tuple<int, VarNameSet, VarNameSet> stmt_info =
        ProcessStatement(given_stmt_list_num);
    int stmt_num_to_insert = get<0>(stmt_info);
    VarNameSet used_vars_to_insert = get<1>(stmt_info);
    VarNameSet modified_vars_to_insert = get<2>(stmt_info);
				
				// Keep track of all statements in the statement list
    stmt_nums.push_back(stmt_num_to_insert);
				// Keep track of all modified variables
    modified_vars.insert(modified_vars_to_insert.begin(),
                         modified_vars_to_insert.end());
				// Keep track of all used variables
    used_vars.insert(used_vars_to_insert.begin(), used_vars_to_insert.end());
  } while (!IsNextType(tt::kCloseBrace));

  PopulatePkbFollows(stmt_nums);
  return stmt_nums;
}

tuple<int, VarNameSet, VarNameSet> Parser::ProcessStatement(int given_stmt_list_num) {
  int num_to_return = ++stmt_num_;
  VarNameSet used_vars;
  VarNameSet modified_vars;

		ReadNextToken();
  if (IsCurrentTokenKeyword() && !IsNextType(tt::kAssignment)) {
    pair<VarNameSet, VarNameSet> keyword_stmt_info = ProcessKeyword(given_stmt_list_num);
    used_vars = get<0>(keyword_stmt_info);
    modified_vars = get<1>(keyword_stmt_info);
  } else {
    pair<VarName, VarNameSet> assignment_stmt_info = ProcessAssignment(given_stmt_list_num);
    used_vars.insert(get<0>(assignment_stmt_info));
				modified_vars = get<1>(assignment_stmt_info);
  }
  return make_tuple(num_to_return, used_vars, modified_vars);
}

pair<VarNameSet, VarNameSet> Parser::ProcessKeyword(int given_stmt_list_num) {
  VarNameSet used_vars;
  VarNameSet modified_vars;

  if (IsCurrentKeywordType(ts::kIf)) {
    return ProcessIfBlock(given_stmt_list_num);
  } else if (IsCurrentKeywordType(ts::kWhile)) {
    return ProcessWhileBlock(given_stmt_list_num);
  } else if (IsCurrentKeywordType(ts::kCall)) {
    // todo call handling
  } else if (IsCurrentKeywordType(ts::kRead)) {
    VarName modified_var = ProcessRead(given_stmt_list_num);
    modified_vars.insert(modified_var);
    return make_pair(used_vars, modified_vars);
  } else if (IsCurrentKeywordType(ts::kPrint)) {
    VarName used_var = ProcessPrint(given_stmt_list_num);
    used_vars.insert(used_var);
    return make_pair(used_vars, modified_vars);
  }
}

VarName Parser::ProcessRead(int given_stmt_list_num) {
  VarName modified_var = ReadNextToken().value;
  pkb_->InsertReadStmt(
      &ReadStmtData(stmt_num_, given_stmt_list_num, modified_var));

  // eat semicolon
  ReadNextToken();
  return modified_var;
}

VarName Parser::ProcessPrint(int given_stmt_list_num) {
  VarName used_var = ReadNextToken().value;
  pkb_->InsertPrintStmt(
      &PrintStmtData(stmt_num_, given_stmt_list_num, used_var));

  // eat semicolon
  ReadNextToken();
  return used_var;
}

pair<VarName, VarNameSet> Parser::ProcessAssignment(int given_stmt_list_num) {
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
  pkb_->InsertAssignStmt(&AssignStmtData(stmt_num_, given_stmt_list_num,
                                         lhs_var, rhs_vars, rhs_consts,
                                         postfix_tokens_rhs));

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
       << " added, stmtLst#: " << given_stmt_list_num << ", lhs: " << lhs_var
       << ", rhs_vars: " << rhsvar << ", rhs_consts: " << rhsconst << endl;
  //***********************************************

  return make_pair(lhs_var, rhs_vars);
}

pair<VarNameSet, VarNameSet> Parser::ProcessIfBlock(int given_stmt_list_num) {
  int if_stmt_num = stmt_num_;
  int then_stmt_list_num = ++stmt_list_num_;
  int else_stmt_list_num = ++stmt_list_num_;
  pair<VarNameSet, ConstValueSet> used_set = ProcessConditional();

  //************* DEBUG******************
  string control_var_str = string();
  for (VarName var : used_set.first) {
    control_var_str = control_var_str + " " + var;
  }
  cout << "If statement#" << if_stmt_num
       << " added, stmtLst#: " << given_stmt_list_num
       << ", control_variable: " << control_var_str << endl;
  //*************************************

  // eat 'then {' tokens
  EatNumTokens(2);

  cout << "[inside then block, stmtLst# should be " << then_stmt_list_num << "]"
       << endl;
  // Process everything inside the if block
  StmtNumListInt children_stmt_nums_part_1 =
      ProcessStatementList(then_stmt_list_num);

  // eat "} else {" tokens
  EatNumTokens(3);
  cout << "[inside else block, stmtLst# should be " << else_stmt_list_num << "]"
       << endl;
  // Process everything inside the counterpart else block
  StmtNumListInt children_stmt_nums_part_2 =
      ProcessStatementList(else_stmt_list_num);

		// Get all child statements in the then and else block
		// Used for populating Parent relation in PKB
  StmtNumListInt children_stmt_nums;
  children_stmt_nums.insert(
      children_stmt_nums_part_1.begin(), children_stmt_nums_part_2.begin(),
      children_stmt_nums_part_2.end());

		PopulatePkbParent(if_stmt_num, children_stmt_nums);

  // eat closing brace
  ReadNextToken();

  cout << "[leaving if block]" << endl;

  // Update PKB of the 'if' block
  pkb_->InsertIfStmt(&IfStmtData(if_stmt_num, given_stmt_list_num,
                                 then_stmt_list_num, else_stmt_list_num,
                                 used_set.first, used_set.second));
}

void Parser::ProcessWhileBlock(int given_stmt_list_num) {
  int while_stmt_num = stmt_num_;
  int while_stmt_list_num = ++stmt_list_num_;
  pair<VarNameSet, ConstValueSet> used_set = ProcessConditional();

  //************* DEBUG******************
  string control_var_str = string();
  for (VarName var : used_set.first) {
    control_var_str = control_var_str + " " + var;
  }
  cout << "While statement#" << while_stmt_num
       << " added, stmtList#: " << given_stmt_list_num
       << ", control_variable: " << control_var_str << endl;
  //*************************************

  // eat open brace
  ReadNextToken();

  cout << "[inside while block, stmtLst# should be " << while_stmt_list_num
       << "]" << endl;

  StmtNumListInt children_stmt_nums = ProcessStatementList(while_stmt_list_num);
  PopulatePkbParent(while_stmt_num, children_stmt_nums);

  // eat close brace
  ReadNextToken();

  cout << "[leaving while block]" << endl;

  // Update PKB of the 'while' block
  pkb_->InsertWhileStmt(&WhileStmtData(while_stmt_num, given_stmt_list_num,
                                       while_stmt_list_num, used_set.first,
                                       used_set.second));
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

void Parser::PopulatePkbFollows(StmtNumListInt stmt_nums) {
  for (size_t i = 0; i < stmt_nums.size(); i++) {
    for (size_t j = i + 1; j < stmt_nums.size(); j++) {
      string followee = std::to_string(stmt_nums[i]);
      string follower = std::to_string(stmt_nums[j]);
      pkb_->InsertFollows(followee, follower);
    }
  }
}

void Parser::PopulatePkbParent(int parent_num, StmtNumListInt children_stmt_nums) {
  string parent_num_str = std::to_string(parent_num);
		for (size_t i = 0; i < children_stmt_nums.size(); i++) {
    string child_num_str = std::to_string(children_stmt_nums[i]);
				//pkb_.InsertParent(parent_num_str, child_num_str);
  }
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