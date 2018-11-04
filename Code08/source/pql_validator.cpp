#include "pql_validator.h"

#include <iostream>
#include <regex>
#include <stack>
using std::cout;
using std::endl;
using std::regex;
using std::regex_match;
using std::stack;
using tt = Tokenizer::TokenType;

/* Use Regex to check if a given string is of IDENT format. */
bool PqlValidator::ValidateIdent(string content) {
  return regex_match(content, regex("[a-zA-Z][a-zA-Z0-9]*"));
}

/* Use Regex to check if a given string is of INTEGER format. */
bool PqlValidator::ValidateInteger(string content) {
  return regex_match(content, regex("[0-9]*"));
}

/* Check if a given list of tokens is an expression. */
bool PqlValidator::ValidateExpression(TokenList tokens) {
  // expr: expr ‘+’ term | expr ‘-’ term | term
  // term: term ‘*’ factor | term ‘ / ’ factor | term ‘%’ factor | factor
  // factor : var_name | const_value | ‘(’ expr ‘)’
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
      } else {
        was_operator = false;
      }
    } else {
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
      } else {
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

/* Check if an attribute is compatible with a certain entity type. */
bool PqlValidator::ValidateAttribute(PqlDeclarationEntity type, string attr) {
  switch (type) {
    case PqlDeclarationEntity::kStmt:
    case PqlDeclarationEntity::kRead:
    case PqlDeclarationEntity::kPrint:
    case PqlDeclarationEntity::kWhile:
    case PqlDeclarationEntity::kIf:
    case PqlDeclarationEntity::kAssign:
      if (attr == "stmt") return true;
      break;
    case PqlDeclarationEntity::kVariable:
      if (attr == "varName") return true;
      break;
    case PqlDeclarationEntity::kConstant:
      if (attr == "value") return true;
      break;
    case PqlDeclarationEntity::kProcedure:
      if (attr == "procName") return true;
      break;
    case PqlDeclarationEntity::kCall:
      if (attr == "stmt" || attr == "procName") return true;
      break;
  }

  return false;
}

/* Check if a PqlDeclarationEntity is actually a real entity type. */
bool PqlValidator::IsSynonym(PqlDeclarationEntity type) {
  switch(type) {
      case PqlDeclarationEntity::kStmt:
      case PqlDeclarationEntity::kRead:
      case PqlDeclarationEntity::kPrint:
      case PqlDeclarationEntity::kCall:
      case PqlDeclarationEntity::kCallName:
      case PqlDeclarationEntity::kWhile:
      case PqlDeclarationEntity::kIf:
      case PqlDeclarationEntity::kAssign:
      case PqlDeclarationEntity::kVariable:
      case PqlDeclarationEntity::kConstant:
      case PqlDeclarationEntity::kProgline:
      case PqlDeclarationEntity::kProcedure:
        return true;
      default: 
        return false;
  }
}