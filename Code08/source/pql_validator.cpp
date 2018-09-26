#include "pql_validator.h"

#include <regex>
#include <stack>
#include <iostream>
using std::regex;
using std::regex_match;
using std::stack;
using std::cout;
using std::endl;
using tt = Tokenizer::TokenType;

bool PqlValidator::ValidateIdent(string content) {
  return regex_match(content, regex("[a-zA-Z][a-zA-Z0-9]*"));
}

bool PqlValidator::ValidateInteger(string content) {
  return regex_match(content, regex("[0-9]*"));
}

bool PqlValidator::ValidateExpression(TokenList tokens) {
  // expr: expr �+� term | expr �-� term | term
  // term: term �*� factor | term � / � factor | term �%� factor | factor
  // factor : var_name | const_value | �(� expr �)�
  // expression should always start with kDigit or kName
  // in the style v o v o v, where v is a variable or digit, and o is operator
  if (tokens.empty()) {
    return false;
  }
  bool was_operator = false;
  stack<tt> parenthesis_stack;
  Token current = tokens[0];
  if (current.type != tt::kOpenParen && current.type != tt::kName &&
    current.type != tt::kDigit) {
    return false;
  }

  if (current.type == tt::kOpenParen) {
    parenthesis_stack.push(tt::kOpenParen);
    // false truth since next variable after this shouldn't be an operator
    was_operator = true;
  }

  for (size_t i = 1; i < tokens.size(); i++) {
    current = tokens[i];
    if (was_operator) {
      if (current.type != tt::kDigit && current.type != tt::kName &&
        current.type != tt::kOpenParen) {
        cout << "[EXPR INVALID], current token: " << current.value << endl;
        return false;
      }

      if (current.type == tt::kOpenParen) {
        parenthesis_stack.push(tt::kOpenParen);
      }
      else {
        was_operator = false;
      }
    }
    else {
      if (current.type != tt::kCloseParen && current.type != tt::kOperator) {
        cout << "[EXPR INVALID], current token: " << current.value << endl;
        return false;
      }

      if (current.type == tt::kCloseParen) {
        if (parenthesis_stack.empty()) {
          cout << "[IMBALANCED PARENTHESIS -- INVALID]" << endl;
          return false;
        }
        parenthesis_stack.pop();
      }
      else {
        was_operator = true;
      }
    }
  }

  if (!parenthesis_stack.empty()) {
    cout << "[STACK NOT EMPTY -- INVALID]" << endl;
    return false;
  }

  return true;
}