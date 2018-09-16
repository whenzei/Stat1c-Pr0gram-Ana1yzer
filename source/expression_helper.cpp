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

bool ExpressionHelper::IsPatternStartWith(TokenList tokens_primary,
                                          TokenList tokens_secondary) {
  // tokens_primary should be >= tokens_secondary
  if (tokens_primary.size() < tokens_secondary.size()) {
    return false;
  }

  return Matches(tokens_primary, tokens_secondary, 0);
}

bool ExpressionHelper::IsPatternEndWith(TokenList tokens_primary,
                                        TokenList tokens_secondary) {
  // tokens_primary should be >= tokens_secondary
  if (tokens_primary.size() < tokens_secondary.size()) {
    return false;
  }

  if (tokens_primary.size() == tokens_secondary.size()) {
    return Matches(tokens_primary, tokens_secondary, 0);
  }

  size_t pri_offset = tokens_primary.size() - tokens_secondary.size();

  return Matches(tokens_primary, tokens_secondary, pri_offset);
}

bool ExpressionHelper::IsPatternExact(TokenList tokens_primary,
                                      TokenList tokens_secondary) {
  if (tokens_primary.size() != tokens_secondary.size()) {
    return false;
  }

  return Matches(tokens_primary, tokens_secondary, 0);
}

bool ExpressionHelper::IsPatternSubset(TokenList tokens_primary,
                                       TokenList tokens_secondary) {
  if (tokens_primary.size() < tokens_secondary.size()) {
    return false;
  }

  size_t num_frames = tokens_primary.size() - tokens_secondary.size() + 1;
  for (size_t i = 0; i < num_frames; i++) {
    bool match_status = Matches(tokens_primary, tokens_secondary, i);
    // Check if there is a matching frame
    if (match_status) {
      return true;
    }
  }

  return false;
}

bool ExpressionHelper::Matches(TokenList tokens_first, TokenList tokens_second,
                               size_t first_offset) {
  size_t first_index;
  size_t second_index;
  for (first_index = first_offset, second_index = 0;
       second_index <= tokens_second.size() - 1;
       first_index++, second_index++) {
    if (tokens_first[first_index].value != tokens_second[second_index].value ||
        tokens_first[first_index].type != tokens_second[second_index].type) {
      return false;
    }
  }
  return true;
}
