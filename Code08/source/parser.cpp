#pragma once

#include <fstream>
#include <iostream>
#include <stack>

#include "expression_helper.h"
#include "parse_data.h"
#include "parser.h"
#include "pkb.h"
#include "statement_data.h"
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
  stmt_list_num_ = 1;
}

bool Parser::Parse(string filepath) {
  // read content from file
  string contents = Util::ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  // validate tokens for syntax errors
  Validator validator = Validator(tokens_);
  if (!validator.ValidateProgram()) {
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
    ProcessProcedure(stmt_list_num_);
  } while (!IsAtEnd());

  /****** Debug Call graph ******/
  // DFS call graph
  vector<string> traverse = pkb_->GetCallGraph()->Toposort();
  for (auto name : traverse) {
    cout << name << endl;
  }
}

void Parser::ProcessProcedure(int given_stmt_list_index) {
  ReadNextToken();
  curr_proc_name_ = ReadNextToken().value;
  pkb_->InsertProcName(curr_proc_name_);
  current_cfg_ = pkb_->InsertCFG(curr_proc_name_);
  // eat the open brace
  ReadNextToken();

  ParseData parse_data = ProcessStatementList(given_stmt_list_index);

  PopulatePkbModifies(curr_proc_name_, parse_data.GetModifiedVariables());
  PopulatePkbUses(curr_proc_name_, parse_data.GetUsedVariables());

  //******* Debug CFG ************
  vector<int> keys;
  for (auto it = current_cfg_->begin(); it != current_cfg_->end(); ++it) {
    keys.push_back(it->first);
    cout << it->second.size() << "\n";
  }
  std::sort(keys.begin(), keys.end());

  for (auto& key : keys) {
    StmtNumIntList adj_list = current_cfg_->at(key);
    string str;
    for (auto& node : adj_list) {
      str.append(std::to_string(node));
      str.append(" ");
    }
    cout << std::to_string(key) << " --> " << str << "\n";
  }
  //*******************************

  ReadNextToken();
}

ParseData Parser::ProcessStatementList(int given_stmt_list_num) {
  StmtNumIntList stmt_nums;
  VarNameSet modified_vars;
  VarNameSet used_vars;

  StmtNumInt last_in_block_1 = 0;
  StmtNumInt last_in_block_2 = 0;
  StmtNumInt prev_stmt_num = 0;

  do {
    ParseData stmt_info = ProcessStatement(given_stmt_list_num);
    StmtNumInt stmt_num = stmt_info.GetStmtNum();
    VarNameSet used_vars_to_insert = stmt_info.GetUsedVariables();
    VarNameSet modified_vars_to_insert = stmt_info.GetModifiedVariables();

    //******* Updates cfg****************
    if (last_in_block_1 != 0) {
      StmtNumIntList curr_adj_list = current_cfg_->at(last_in_block_1);
      curr_adj_list.push_back(stmt_num);
      current_cfg_->emplace(last_in_block_1, curr_adj_list);
    }

    if (last_in_block_2 != 0) {
      StmtNumIntList curr_adj_list = current_cfg_->at(last_in_block_2);
      curr_adj_list.push_back(stmt_num);
      current_cfg_->emplace(last_in_block_2, curr_adj_list);
    }

    last_in_block_1 = stmt_info.GetLastStmtNumOne();
    last_in_block_2 = stmt_info.GetLastStmtNumTwo();

    if (prev_stmt_num != 0) {
      StmtNumIntList curr_adj_list = current_cfg_->at(prev_stmt_num);
      curr_adj_list.push_back(stmt_num);
      current_cfg_->emplace(prev_stmt_num, curr_adj_list);
    }
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

  return ParseData(stmt_nums, used_vars, modified_vars);
}

ParseData Parser::ProcessStatement(int given_stmt_list_num) {
  int curr_stmt_num = ++stmt_num_;

  StmtNumIntList adj_list = StmtNumIntList();
  current_cfg_->emplace(curr_stmt_num, adj_list);

  VarNameSet used_vars;
  VarNameSet modified_vars;

  ReadNextToken();
  if (IsCurrentTokenKeyword() && !IsNextType(tt::kAssignment)) {
    ParseData keyword_stmt_info = ProcessKeyword(given_stmt_list_num);
    used_vars = keyword_stmt_info.GetUsedVariables();
    modified_vars = keyword_stmt_info.GetModifiedVariables();
    int last_stmt_num_1 = keyword_stmt_info.GetLastStmtNumOne();
    int last_stmt_num_2 = keyword_stmt_info.GetLastStmtNumTwo();
    return ParseData(curr_stmt_num, used_vars, modified_vars, last_stmt_num_1,
                     last_stmt_num_2);
  } else {
    ParseData assignment_stmt_info = ProcessAssignment(given_stmt_list_num);
    used_vars = assignment_stmt_info.GetUsedVariables();
    modified_vars.insert(assignment_stmt_info.GetModifiedVariable());
    return ParseData(curr_stmt_num, used_vars, modified_vars);
  }
}

ParseData Parser::ProcessKeyword(int given_stmt_list_num) {
  VarNameSet used_vars;
  VarNameSet modified_vars;

  if (IsCurrentKeywordType(ts::kIf)) {
    return ProcessIfBlock(given_stmt_list_num);
  } else if (IsCurrentKeywordType(ts::kWhile)) {
    return ProcessWhileBlock(given_stmt_list_num);
  } else if (IsCurrentKeywordType(ts::kCall)) {
    ProcessCall(given_stmt_list_num);
    // do nothing for now, DE will populate Uses and Modifies;
    return ParseData(VarNameSet(), VarNameSet());
  } else if (IsCurrentKeywordType(ts::kRead)) {
    VarName modified_var = ProcessRead(given_stmt_list_num);
    modified_vars.insert(modified_var);
    return ParseData(used_vars, modified_vars);
  } else if (IsCurrentKeywordType(ts::kPrint)) {
    VarName used_var = ProcessPrint(given_stmt_list_num);
    used_vars.insert(used_var);
    return ParseData(used_vars, modified_vars);
  }

  exit(-1);
}

VarName Parser::ProcessRead(int given_stmt_list_num) {
  VarName modified_var = ReadNextToken().value;
  pkb_->InsertReadStmt(
      &ReadStmtData(stmt_num_, given_stmt_list_num, modified_var));

  PopulatePkbModifies(stmt_num_, modified_var);

  // eat semicolon
  ReadNextToken();
  return modified_var;
}

VarName Parser::ProcessPrint(int given_stmt_list_num) {
  VarName used_var = ReadNextToken().value;
  pkb_->InsertPrintStmt(
      &PrintStmtData(stmt_num_, given_stmt_list_num, used_var));

  PopulatePkbUses(stmt_num_, used_var);

  // eat semicolon
  ReadNextToken();
  return used_var;
}

void Parser::ProcessCall(int given_stmt_list_index) {
  VarName called_proc_name = ReadNextToken().value;
  pkb_->GetCallGraph()->AddEdge(curr_proc_name_, called_proc_name);
  // todo: insert CallStmt with the stmt_num into the PKB
}

ParseData Parser::ProcessAssignment(int given_stmt_list_num) {
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
         << " added, stmtLst#: " << given_stmt_list_num << ", lhs: " << lhs_var
         << ", rhs_vars: " << rhsvar << ", rhs_consts: " << rhsconst << endl;
    //***********************************************
  }

  return ParseData(rhs_vars, lhs_var);
}

ParseData Parser::ProcessIfBlock(int given_stmt_list_num) {
  int if_stmt_num = stmt_num_;
  int then_stmt_list_num = ++stmt_list_num_;
  int else_stmt_list_num = ++stmt_list_num_;
  pair<VarNameSet, ConstValueSet> used_set_conditionals = ProcessConditional();

  if (DEBUG_FLAG) {
    //************* DEBUG******************
    string control_var_str = string();
    for (VarName var : used_set_conditionals.first) {
      control_var_str = control_var_str + " " + var;
    }
    cout << "If statement#" << if_stmt_num
         << " added, stmtLst#: " << given_stmt_list_num
         << ", control_variable: " << control_var_str << endl;
    //*************************************
  }

  // eat 'then {' tokens
  EatNumTokens(2);

  if (DEBUG_FLAG) {
    cout << "[inside then block, stmtLst# should be " << then_stmt_list_num
         << "]" << endl;
  }
  // Process everything inside the if block
  ParseData then_stmt_info = ProcessStatementList(then_stmt_list_num);

  // eat "} else {" tokens
  EatNumTokens(3);

  if (DEBUG_FLAG) {
    cout << "[inside else block, stmtLst# should be " << else_stmt_list_num
         << "]" << endl;
  }
  // Process everything inside the counterpart else block
  ParseData else_stmt_info = ProcessStatementList(else_stmt_list_num);

  // Get all child statements in the then and else block
  // Used for populating Parent relation in PKB
  StmtNumIntList then_stmt_nums = then_stmt_info.GetStmtNumList();
  StmtNumIntList else_stmt_nums = else_stmt_info.GetStmtNumList();

  // Handles cfg
  StmtNumIntList curr_adj_list = current_cfg_->at(if_stmt_num);
  curr_adj_list.push_back(then_stmt_nums.at(0));
  curr_adj_list.push_back(else_stmt_nums.at(0));
  current_cfg_->emplace(if_stmt_num, curr_adj_list);

  StmtNumIntList child_stmt_nums(then_stmt_nums);
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
  pkb_->InsertIfStmt(&IfStmtData(if_stmt_num, given_stmt_list_num,
                                 used_set_conditionals.first,
                                 used_set_conditionals.second));
  return ParseData(used_vars, modified_vars, then_stmt_nums.back(),
                   else_stmt_nums.back());
}

ParseData Parser::ProcessWhileBlock(int given_stmt_list_num) {
  int while_stmt_num = stmt_num_;
  int while_stmt_list_num = ++stmt_list_num_;
  pair<VarNameSet, ConstValueSet> used_set_conditional = ProcessConditional();

  if (DEBUG_FLAG) {
    //************* DEBUG******************
    string control_var_str = string();
    for (VarName var : used_set_conditional.first) {
      control_var_str = control_var_str + " " + var;
    }
    cout << "While statement#" << while_stmt_num
         << " added, stmtList#: " << given_stmt_list_num
         << ", control_variable: " << control_var_str << endl;
    //*************************************
  }

  // eat open brace
  ReadNextToken();

  if (DEBUG_FLAG) {
    cout << "[inside while block, stmtLst# should be " << while_stmt_list_num
         << "]" << endl;
  }

  ParseData children_stmt_info = ProcessStatementList(while_stmt_list_num);
  StmtNumIntList children_stmt_nums = children_stmt_info.GetStmtNumList();
  VarNameSet used_vars = children_stmt_info.GetUsedVariables();
  VarNameSet modified_vars = children_stmt_info.GetModifiedVariables();

  // Update variables used in conditionals
  used_vars.insert(used_set_conditional.first.begin(),
                   used_set_conditional.first.end());

  PopulatePkbParent(while_stmt_num, children_stmt_nums);
  PopulatePkbUses(while_stmt_num, used_vars);
  PopulatePkbModifies(while_stmt_num, modified_vars);

  StmtNumIntList curr_adj_list = current_cfg_->at(while_stmt_num);
  curr_adj_list.push_back(children_stmt_nums.at(0));
  current_cfg_->emplace(while_stmt_num, curr_adj_list);

  // eat close brace
  ReadNextToken();

  if (DEBUG_FLAG) {
    cout << "[leaving while block]" << endl;
  }

  // Update PKB of the 'while' block
  pkb_->InsertWhileStmt(&WhileStmtData(while_stmt_num, given_stmt_list_num,
                                       used_set_conditional.first,
                                       used_set_conditional.second));

  return ParseData(used_vars, modified_vars, children_stmt_nums.back());
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

void Parser::PopulatePkbFollows(StmtNumIntList stmt_nums) {
  for (size_t i = 0; i < stmt_nums.size(); i++) {
    for (size_t j = i + 1; j < stmt_nums.size(); j++) {
      string followee = std::to_string(stmt_nums[i]);
      string follower = std::to_string(stmt_nums[j]);
      pkb_->InsertFollows(followee, follower);
    }
  }
}

void Parser::PopulatePkbParent(int parent_num,
                               StmtNumIntList children_stmt_nums) {
  string parent_num_str = std::to_string(parent_num);
  for (size_t i = 0; i < children_stmt_nums.size(); i++) {
    string child_num_str = std::to_string(children_stmt_nums[i]);
    pkb_->InsertParent(parent_num_str, child_num_str);
  }
}

void Parser::PopulatePkbUses(int stmt_num, VarNameSet used_vars) {
  string stmt_num_str = std::to_string(stmt_num);
  for (const auto& var : used_vars) {
    pkb_->InsertUsesS(stmt_num_str, var);
  }
}

void Parser::PopulatePkbUses(ProcName proc_name, VarNameSet used_vars) {
  for (const auto& var : used_vars) {
    pkb_->InsertUsesP(proc_name, var);
  }
}

void Parser::PopulatePkbUses(int stmt_num, VarName used_var) {
  string stmt_num_str = std::to_string(stmt_num);
  pkb_->InsertUsesS(stmt_num_str, used_var);
}

void Parser::PopulatePkbModifies(int stmt_num, VarNameSet modified_vars) {
  string stmt_num_str = std::to_string(stmt_num);
  for (const auto& var : modified_vars) {
    pkb_->InsertModifiesS(stmt_num_str, var);
  }
}

void Parser::PopulatePkbModifies(ProcName proc_name, VarNameSet modified_vars) {
  for (const auto& var : modified_vars) {
    pkb_->InsertModifiesP(proc_name, var);
  }
}

void Parser::PopulatePkbModifies(int stmt_num, VarName modified_var) {
  string stmt_num_str = std::to_string(stmt_num);
  pkb_->InsertModifiesS(stmt_num_str, modified_var);
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
