#pragma once
#ifndef SPA_SIMPLE_VALIDATOR_H
#define SPA_SIMPLE_VALIDATOR_H

#include "tokenizer.h"

using std::string;

enum KeywordType {
  kProcedure = 0,
  kIf = 1,
  kElse = 2,
  kWhile = 3,
  kCall = 4,
  kRead = 5,
  kPrint = 6
};

class SimpleValidator {
 public:
  /**
   * Checks if string input is a SIMPLE keyword
   * @returns true if it is a SIMPLE keyword, false otherwise
   */
  static bool isKeyword(string input);

  /**
   * Checks if string input is a SIMPLE keyword of given type
   * @returns true if it is a SIMPLE keyword of given type, false otherwise
   */
  static bool isKeyword(int type, string input);

  /**
   * Checks if list of tokens is syntactically correct in SIMPLE language
   * Precondition: First token should have value -> 'procedure' and last token
   * with value -> '}'. Number of tokens should be >= 4 ('procedure',
   * procedure_name, '{' , .... , '}')
   * @returns true if list of tokens is syntactically correct, false otherwise.
   */
  static bool validateProcedure(TokenList tokens, size_t start, size_t end);
};

#endif