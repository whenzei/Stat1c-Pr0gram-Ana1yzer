#pragma once

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <iostream>
#include <unordered_set>

#include "pkb.h"
#include "tokenizer.h"

using std::string;
using std::tuple;
using std::unordered_set;
using StmtNumListInt = vector<int>;

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
  tuple<StmtNumListInt, VarNameSet, VarNameSet> ProcessStatementList(
      int given_stmt_list_index);
  tuple<int, VarNameSet, VarNameSet> ProcessStatement(
      int given_stmt_list_index);
  pair<VarNameSet, VarNameSet> ProcessKeyword(int given_stmt_list_index);

  // @returns tuple consisting of set of Used variable names, and a Modified
  // variable name
  pair<VarNameSet, VarName> ProcessAssignment(int given_stmt_list_index);

  // Process the if block with its counterpart else block
  // @returns pair consisting of set of Used variable names, and a set of
  // Modified variable names
  pair<VarNameSet, VarNameSet> ProcessIfBlock(int given_stmt_list_index);

  // Process the while block with its counterpart else block
  // @returns pair consisting of set of Used variable names, and a set of
  // Modified variable names
  pair<VarNameSet, VarNameSet> ProcessWhileBlock(int given_stmt_list_index);

		// @returns the variable name modified
  VarName ProcessRead(int given_stmt_list_index);

		// @returns the variable name used
  VarName ProcessPrint(int given_stmt_list_index);

  pair<VarNameSet, ConstValueSet> ProcessConditional();

  void PopulatePkbFollows(StmtNumListInt stmt_nums);
  void PopulatePkbParent(int stmt, StmtNumListInt stmt_nums);
  void PopulatePkbUses(int stmt_num, VarNameSet used_vars);
  void PopulatePkbUses(int stmt_num, VarName used_var);
  void PopulatePkbModifies(int stmt_num, VarNameSet modified_vars);
  void PopulatePkbModifies(int stmt_num, VarName modified_var);

  void ParseProgram();

 public:
  Parser(PKB* pkb);
  void Parse(string filepath);

  // For testing purposes
  void Parse(TokenList program_tokenized);
};

#endif  //! SPA_PARSER_H