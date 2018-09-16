#pragma once

#ifndef EXPRESSION_HELPER_H
#define EXPRESSION_HELPER_H

#include "tokenizer.h"

class ExpressionHelper {
 public:
  // Implementation of shunting-yard algorithm
  // @params: TokenList of an expression in the infix order
  // @return: returns TokenList of an expression in the postfix order
  static TokenList ToPostfix(TokenList tokens_to_convert);

  /* Checks if the primary list of tokens begin with the secondary list of
   * tokens
   *
   * Used by PQL for pattern a(_, "expr"_)
   *
   * @return: returns true if the sequence of tokens_primary begins with the
   * sequence of tokens in tokens_secondary.
   *
   * Example: "ab*3+" (tokens_primary) ,
   * "ab*" (tokens_secondary) returns true
   */
  static bool IsPatternStartWith(TokenList tokens_primary,
                                 TokenList tokens_secondary);

  /* Checks if the primary list of tokens end with the secondary list of
   * tokens.
   *
   * Used by PQL for pattern a(_, _"expr")
   *
   * @return: returns true if the sequence of tokens_primary ends with the
   * sequence of tokens in tokens_secondary.
   *
   * Example: "ab*cxy+*+k+d+" (tokens_primary), "kd+" (tokens_secondary) returns
   * true.
   */
  static bool IsPatternEndWith(TokenList tokens_primary,
                               TokenList tokens_secondary);

  /* Checks if the primary list of tokens matches exactly the secondary list of
   * tokens.
			*
   * Used by PQL for pattern a(_,"expr")
			*
   * @return: returns true if the sequence of tokens_primary is the exact
   * sequence of tokens in tokens_secondary.
   *
   * Example: "ab*c+" (tokens_primary), "ab*c+" (tokens_secondary) returns
   * true.
   */
  static bool IsPatternExact(TokenList tokens_primary,
                             TokenList tokens_secondary);
		
		 /* Checks if the secondary list of tokens is a subset sequence of the primary list of tokens
  * 
  * Used by PQL for pattern a(_, _"expr"_)
  *
  * @return: returns true if secondary list of tokens is a subset sequence of the primary list of tokens. 
  *
  * Example: "ab*c+cdas" (tokens_primary), "b*c+c" (tokens_secondary) returns
  * true.
  */
		static bool IsPatternSubset(TokenList tokens_primary,
                              TokenList tokens_secondary);
 private:
  static bool Matches(TokenList tokens_first, TokenList tokens_second,
                      size_t first_offset);
};
#endif