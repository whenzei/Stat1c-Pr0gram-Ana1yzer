#pragma once
#ifndef SPA_SIMPLE_VALIDATOR_2_H
#define SPA_SIMPLE_VALIDATOR_2_H

#include "tokenizer.h"

using std::string;

class SimpleValidator2 {
 public:
  enum KeywordType { kProcedure, kIf, kElse, kWhile, kCall, kRead, kPrint };

  /**
   * Checks if list of tokens is syntactically correct in SIMPLE language
   * Precondition: First token should have value -> 'procedure' and last token
   * with value -> '}'. Number of tokens should be >= 4 ('procedure',
   * procedure_name, '{' , .... , '}')
   * @returns true if list of tokens is syntactically correct, false otherwise.
   */
  static bool ValidateProcedure(TokenList tokens, size_t start, size_t end);

 private:
  /**
   *	Checks if the '=' (current index of token list) only has LHS as kName,
   *and RHS as kNAME | kDigit
   */
  static bool IsValidAssignmentPos(TokenList tokens, size_t currIndex);

  /**
   * Checks if the NAME (current index of token list) causes syntax error
   */
  static bool IsValidNamePos(TokenList tokens, size_t currIndex);

  /**
   * Checks if the DIGIT (current index of token list) causes syntax error
   */
  static bool IsValidDigitPos(TokenList tokens, size_t currIndex);

  /**
   * Checks if the 'if' keyword (current index of token list) causes syntax
   * error
   */
  static bool IsValidIfPos(TokenList tokens, size_t currIndex);

  /**
   * Checks if the 'while' keyword (current index of token list) causes syntax
   * error
   */
  static bool IsValidWhilePos(TokenList tokens, size_t currIndex);

  /**
   * Checks if the semicolon (current index of token list) causes
   * syntax error
   */
  static bool IsValidSemicolonPos(TokenList tokens, size_t currIndex);
};

#endif