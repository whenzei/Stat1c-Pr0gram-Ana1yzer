#pragma once

#include <map>
#include <queue>
#include <stack>
#include <string>

#include "expression_helper.h"

using std::map;
using std::queue;
using std::stack;
using std::string;
using tt = Tokenizer::TokenType;

static const map<string, int> kPrecedenceMap = {
    {"-", 1}, {"+", 1}, {"/", 2}, {"*", 2}, {"%", 2}};

TokenList ExpressionHelper::ToPostfix(TokenList tokens_to_convert) {
  TokenList result_tokens;
  stack<Token> operator_stack;

  for (Token const& curr_token : tokens_to_convert) {
    if (curr_token.type == tt::kDigit || curr_token.type == tt::kName) {
      result_tokens.push_back(curr_token);
    }
    if (curr_token.type == tt::kOperator) {
      while (!operator_stack.empty() &&
             operator_stack.top().type != tt::kOpenParen &&
             (kPrecedenceMap.at(operator_stack.top().value) >=
              kPrecedenceMap.at(curr_token.value))) {
        Token for_queue = operator_stack.top();
        operator_stack.pop();
        result_tokens.push_back(for_queue);
      }
      operator_stack.push(curr_token);
    }
    if (curr_token.type == tt::kOpenParen) {
      operator_stack.push(curr_token);
    }
    if (curr_token.type == tt::kCloseParen) {
      while (operator_stack.top().type != tt::kOpenParen) {
        Token for_queue = operator_stack.top();
        operator_stack.pop();
        result_tokens.push_back(for_queue);
      }
      operator_stack.pop();
    }
  }

  while (!operator_stack.empty()) {
    Token for_queue = operator_stack.top();
    operator_stack.pop();
    result_tokens.push_back(for_queue);
  }

  return result_tokens;
}
