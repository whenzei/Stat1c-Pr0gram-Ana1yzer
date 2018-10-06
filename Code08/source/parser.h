#pragma once

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <iostream>
#include <unordered_set>

#include "parse_data.h"
#include "pkb.h"
#include "tokenizer.h"

using std::string;
using std::unordered_set;
using ProcName = std::string;

class Parser {
 private:
  PKB* pkb_;
  TokenList tokens_;
  Token current_token_;
  CFG* current_cfg_;
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

  // @returns ParseData consisting of list of statement numbers, set of Used
  // variable names, and set of Modified variable names
  ParseData ProcessStatementList(int given_stmt_list_index);

  // @returns ParseData consisting of statement num, set of Used variable names,
  // and set of Modified variable names
  ParseData ProcessStatement(int given_stmt_list_index);

  // @returns ParseData consisting of set of Used variable names, and set of
  // Modified variable names
  ParseData ProcessKeyword(int given_stmt_list_index);

  // @returns ParseData consisting of set of Used variable names, and a Modified
  // variable name
  ParseData ProcessAssignment(int given_stmt_list_index);

  // Process the if block with its counterpart else block
  // @returns ParseData consisting of set of Used variable names, and a set of
  // Modified variable names
  ParseData ProcessIfBlock(int given_stmt_list_index);

  // Process the while block with its counterpart else block
  // @returns ParseData consisting of set of Used variable names, and a set of
  // Modified variable names
  ParseData ProcessWhileBlock(int given_stmt_list_index);

  // @returns the modified variable name
  VarName ProcessRead(int given_stmt_list_index);

  // @returns the used variable name
  VarName ProcessPrint(int given_stmt_list_index);

  pair<VarNameSet, ConstValueSet> ProcessConditional();

  void PopulatePkbFollows(StmtNumIntList stmt_nums);
  void PopulatePkbParent(int stmt, StmtNumIntList stmt_nums);
  void PopulatePkbUses(int stmt_num, VarNameSet used_vars);
  void PopulatePkbUses(ProcName proc_name, VarNameSet used_vars);
  void PopulatePkbUses(int stmt_num, VarName used_var);
  void PopulatePkbModifies(int stmt_num, VarNameSet modified_vars);
  void PopulatePkbModifies(ProcName proc_name, VarNameSet modified_vars);
  void PopulatePkbModifies(int stmt_num, VarName modified_var);

  void ParseProgram();

 public:
  // constructor for Parser, takes in a PKB object
  Parser(PKB* pkb);

  // parses the string contents of the file located at given filepath
  // @param filepath file to be parsed
  // @return true if parse successful (meaning no syntax errors), or false if
  // validation failed
  bool Parse(string filepath);

  // For testing purposes
  bool Parse(TokenList program_tokenized);
};

#endif  //! SPA_PARSER_H