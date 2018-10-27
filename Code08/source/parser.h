#pragma once

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <iostream>
#include <unordered_set>

#include "parse_data.h"
#include "pkb.h"
#include "tokenizer.h"

class Parser {
 private:
  PKB* pkb_;
  TokenList tokens_;
  Token current_token_;
  CFG* current_cfg_;
  ProcName current_proc_name_;
  ProcIndex current_proc_index_;
  int current_index_;
  int stmt_num_;

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

  void ProcessProcedure();

  // @returns ParseData consisting of list of statement numbers, set of Used
  // variable names, and set of Modified variable names
  ParseData ProcessStatementList();

  // @returns ParseData consisting of statement num, set of Used variable names,
  // and set of Modified variable names
  ParseData ProcessStatement();

  // @returns ParseData consisting of set of Used variable names, and set of
  // Modified variable names
  ParseData ProcessKeyword();

  // @returns ParseData consisting of set of Used variable names, and a Modified
  // variable name
  ParseData ProcessAssignment();

  // Process the if block with its counterpart else block
  // @returns ParseData consisting of set of Used variable names, and a set of
  // Modified variable names
  ParseData ProcessIfBlock();

  // Process the while block with its counterpart else block
  // @returns ParseData consisting of set of Used variable names, and a set of
  // Modified variable names
  ParseData ProcessWhileBlock();

  // @returns parse data with the modified variable name
  ParseData ProcessRead();

  // @returns parse data with the used variable name
  ParseData ProcessPrint();

  // @returns empty parse data, since DE will populate uses and modifies later
  ParseData ProcessCall();

  pair<VarNameSet, ConstValueSet> ProcessConditional();

  void PopulatePkbFollows(StmtNumList stmt_nums);
  void PopulatePkbParent(int stmt, StmtNumList stmt_nums);
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
  // @throws SyntacticErrorException if parse issues encountered
  // @return true if parse successful (meaning no syntax errors), or false if
  // validation failed
  bool Parse(string filepath);

  // For testing purposes
  bool Parse(TokenList program_tokenized);
};

#endif  //! SPA_PARSER_H