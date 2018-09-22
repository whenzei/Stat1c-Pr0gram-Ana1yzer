#pragma once

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <iostream>
#include <unordered_set>

#include "pkb.h"
#include "tokenizer.h"

using std::string;
using std::unordered_set;
using std::tuple;

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
  tuple<StmtNumListInt ,VarNameSet, VarNameSet> ProcessStatementList(int given_stmt_list_index);
  tuple<int, VarNameSet, VarNameSet> ProcessStatement(int given_stmt_list_index);
  pair<VarNameSet, VarNameSet> ProcessKeyword(int given_stmt_list_index);
  pair<VarName, VarNameSet> ProcessAssignment(int given_stmt_list_index);

  // Process the if block with its counterpart else block
  pair<VarNameSet, VarNameSet> ProcessIfBlock(int given_stmt_list_index);
  pair<VarNameSet, VarNameSet> ProcessWhileBlock(int given_stmt_list_index);

  VarName ProcessRead(int given_stmt_list_index);
  VarName ProcessPrint(int given_stmt_list_index);

  pair<VarNameSet, ConstValueSet> ProcessConditional();

  void PopulatePkbFollows(StmtNumListInt stmt_nums);
  void PopulatePkbParent(int stmt, StmtNumListInt stmt_nums);

  void ParseProgram();

 public:
  Parser(PKB* pkb);
  void Parse(string filepath);

  // For testing purposes
  void Parse(TokenList program_tokenized);
};

#endif  //! SPA_PARSER_H