#pragma once

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <iostream>
#include <unordered_set>

#include "pkb.h"
#include "tokenizer.h"

using std::string;
using std::unordered_set;

class Parser {
 private:
  PKB* pkb_;
  TokenList tokens_;
  Token current_token_;
  int current_index_;
  int stmt_num_;
  int stmt_list_num_;

  bool IsAtEnd();
  Token ReadNextToken();
  Token PeekNextToken();
  void EatNumTokens(int num);
  bool IsNextType(Tokenizer::TokenType type);
  bool IsCurrentType(Tokenizer::TokenType type);
  bool IsNextKeywordType(Tokenizer::TokenSubtype type);
  bool IsCurrentKeywordType(Tokenizer::TokenSubtype type);
  bool IsNextTokenKeyword();
  bool IsCurrentTokenKeyword();

  void ProcessProcedure(int given_stmt_list_index);
  void ProcessStatementList(int given_stmt_list_index);
  int ProcessStatement(int given_stmt_list_index);
  void ProcessKeyword(int given_stmt_list_index);
  void ProcessAssignment(int given_stmt_list_index);

  // Process the if block with its counterpart else block
  void ProcessIfBlock(int given_stmt_list_index);
  void ProcessWhileBlock(int given_stmt_list_index);

  void ProcessRead(int given_stmt_list_index);
  void ProcessPrint(int given_stmt_list_index);

  pair<VarNameSet, ConstValueSet> ProcessConditional();


		void PopulatePkbFollows(vector<int> stmt_num);

  void ParseProgram();

 public:
  Parser(PKB* pkb);
  void Parse(string filepath);

		//For testing purposes
  void Parse(TokenList program_tokenized);
};

#endif  //! SPA_PARSER_H