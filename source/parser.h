#pragma once

#ifndef SPA_PARSER2_H
#define SPA_PARSER2_H

#include <iostream>
#include <unordered_set>

#include "pkb.h"
#include "tokenizer.h"

using std::string;
using std::unordered_set;

using Variable = string;
using VariableSet = unordered_set<string>;

class Parser {
 private:
  PKB* pkb_;
  TokenList tokens_;
  Token current_token_;
  int current_index_;
  int stmt_index_;
  int stmt_list_index_;

  bool IsValidFile(string filepath);
  string ReadContentFromFile(string filepath);
  Token ReadNextToken();
  Token PeekNextToken();
  void EatNumTokens(int num);
  void ProcessKeyword(int curr_stmt_list_index);
  void ProcessAssignment(int curr_stmt_list_index);

  // Process the if block with its counterpart else block
  void ProcessIfBlock(int curr_stmt_list_index);
  void ProcessWhileBlock(int curr_stmt_list_index);

  // Process the contents in a block
  // Contents in between '{'  and '}'
  // Used in ProcessIfBlock, ProcessWhileBlock
  void ProcessBlockContent(int curr_stmt_list_index);

  void ProcessProcedure();

 public:
  Parser(PKB* pkb);
  void Parse(string filepath);
};

#endif  //! SPA_PARSER2_H