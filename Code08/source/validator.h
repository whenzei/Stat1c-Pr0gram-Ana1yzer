#pragma once

#ifndef SPA_VALIDATOR_H
#define SPA_VALIDATOR_H

#include "parser.h"

using tt = Tokenizer::TokenType;
using ProcNameSet = unordered_set<string>;

class Validator {
 private:
  ProcNameSet all_proc_names_;  // set to check for duplicate proc names

  // Read the next token in the internal TokenList and advance the index counter
  // @returns the next token in the interally stored TokenList
  Token ReadNextToken();

  // Read the next token in the internal TokenList without advancing the index
  // counter
  // @returns the next token in the interally stored TokenList
  Token PeekNextToken();

  // Read the previous token in the internal TokenList without advancing the
  // index counter
  // @returns the previous token in the interally stored TokenList
  Token GetPreviousToken();

  // Check if the next n tokens match with the given list of expected types and
  // advance the index counter by n
  // @params: int num the number of next tokens to match,
  // @params: vector<tt> expected_types to match
  // @returns true if next n tokens match given types, false otherwise
  bool MatchNext(int num, vector<tt> expected_types);

  // Check if the next token match with the given TokenType and advance the
  // index counter
  // @params: TokenType type to match
  // @returns true if next token matches the given type
  bool Match(Tokenizer::TokenType type);

  // Check if the next token match with the given TokenType without advancing
  // the index counter
  // @params: TokenType type to match
  // @returns true if next token matches the given type
  bool Check(Tokenizer::TokenType type);

  // Check if index counter is pointing at the end of the internal TokenList
  // @returns true if end of TokenList, false otherwise
  bool IsAtEnd();

  // Get the first index of any relational symbols in the given TokenListkd
  // @params: TokenList tokens the list of tokens to retrieve the index from
  // @returns the index of the first relational symbol in given TokenList if
  // exists, -1 otherwise
  int GetIndexOfRelational(TokenList tokens);

  // Check if the given TokenList contains any conditional token types
  // @params: TokenList tokens to check
  // @returns true if given TokenList contains any conditional token types,
  // false otherwise
  bool ContainsConditionalType(TokenList tokens);

 public:
  // Constructor
  // @params TokenList tokens to be validated
  Validator(TokenList tokens);

  // Validate the program with the provided token list
  // @returns true if program is valid, false otherwise
  bool ValidateProgram();

  /*SETTERS*/
  // Resets the token list of the validator with the given tokens
  // @params TokenList of tokens to replace
  void ResetTokenList(TokenList tokens);

  /*MAIN FUNCTIONS*/

  // Check if the procedure starting at the current index of the internally
  // stored TokenList is valid
  // @returns true if procedure is valid, false otherwise
  bool IsValidProcedure();

  // Check if the statement list starting at the current index of the internally
  // stored TokenList is valid
  // @returns true if statement list is valid, false otherwise
  bool IsValidStmtList();

  // Check if the statement starting at the current index of the internally
  // stored TokenList is valid
  // @returns true if statement is valid, false otherwise
  bool IsValidStatement();

  // Check if the assignment statement starting at the current index of the
  // internally stored TokenList is valid
  // @returns true if assignment statement is valid, false otherwise
  bool IsValidAssignment();

  // Check if the TokenList of expressions given is valid
  // @params TokenList expression to validate
  // @returns true if expression is valid, false otherwise
  bool IsValidExpression(TokenList expr);

  // Check if the TokenList of conditional expression given is valid
  // @params TokenList conditional expression to validate
  // @returns true if conditional expression is valid, false otherwise
  bool IsValidConditional(TokenList cond_expr);

  // Check if the if block starting at the current index of the internally
  // stored TokenList is valid
  // @returns true if the if block is valid, false otherwise
  bool IsValidIfBlock();

  // Check if the while block starting at the current index of the internally
  // stored TokenList is valid
  // @returns true if while block is valid, false otherwise
  bool IsValidWhileBlock();

  // Check if the statement starting at the current index of the internally
  // stored TokenList is a valid call or read or print statement
  // @returns true if statement is valid, false otherwise
  bool IsValidCallReadPrint();
};

#endif  // !SPA_SIMPLE_VALIDATOR_H
