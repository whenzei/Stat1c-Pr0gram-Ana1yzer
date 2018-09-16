#pragma once

#include <string>
#include <unordered_set>

#include "simple_validator2.h"

using tt = Tokenizer::TokenType;

enum KeywordType {
  kProcedure = 0,
  kIf = 1,
  kElse = 2,
  kWhile = 3,
  kCall = 4,
  kRead = 5,
  kPrint = 6
};

const string kOpenBraceSymbol = "{";
const string kCloseBraceSymbol = "}";

const size_t kMinimalTokenSizeForPocedure = 4;

const static std::unordered_set<string> kKeywords = {
    "procedure", "if", "else", "while", "call", "read", "print"};

bool SimpleValidator2::ValidateProcedure(TokenList tokens, size_t start,
                                         size_t end) {
  size_t numTokens = end - start + 1;

  if (numTokens < kMinimalTokenSizeForPocedure) {
    return false;
  }

  // Check if first token is 'procedure', second token is of type NAME, third
  // token is '{', last token is '}'
  if (tokens[start].value != "procedure" ||
      tokens[start + 1].type != tt::kName ||
      tokens[start + 2].type != tt::kOpenBrace ||
      tokens[end].type != tt::kCloseBrace) {
    return false;
  }

  for (size_t i = start + 3; i < end; i++) {
    if (tokens[i].type == tt::kAssignment && !IsValidAssignmentPos(tokens, i)) {
      return false;
    } else if (tokens[i].type == tt::kName &&
               kKeywords.count(tokens[i].value)) {
      if (tokens[i].value.compare("if") == 0 && !IsValidIfPos(tokens, i)) {
        return false;
      }
      if (tokens[i].value.compare("while") == 0 &&
          !IsValidWhilePos(tokens, i)) {
        return false;
      }
    } else if (tokens[i].type == tt::kName && !IsValidNamePos(tokens, i)) {
      return false;
    } else if (tokens[i].type == tt::kDigit && !IsValidDigitPos(tokens, i)) {
      return false;
    } else if (tokens[i].type == tt::kSemicolon &&
               !IsValidSemicolonPos(tokens, i)) {
      return false;
    }
  }
  return true;
}

bool SimpleValidator2::IsValidAssignmentPos(TokenList tokens,
                                            size_t currIndex) {
  int tokenLeftType = tokens[currIndex - 1].type;
  int tokenRightType = tokens[currIndex + 1].type;

  if (tokenLeftType == tt::kName &&
      (tokenRightType == tt::kDigit || tokenRightType == tt::kName)) {
    return true;
  }

  return false;
}

bool SimpleValidator2::IsValidNamePos(TokenList tokens, size_t currIndex) {
  int tokenLeftType = tokens[currIndex - 1].type;
  int tokenRightType = tokens[currIndex + 1].type;

  string tokenLeftValue = tokens[currIndex - 1].value;
  string tokenRightValue = tokens[currIndex + 1].value;

  // Check if NAME (current token) appears after another NAME or DIGIT
  if (tokenLeftType == tt::kName || tokenLeftType == tt::kDigit) {
    return false;
  }

  // Checks if NAME is right after ')'
  if (tokenLeftType == tt::kCloseParen) {
    return false;
  }

  // Checks if NAME does not belong to procedure, and is before '{'
  if (tokenLeftValue != "procedure" && tokenRightType == tt::kOpenBrace) {
    return false;
  }

  // Checks if NAME is right before '}'
  if (tokenRightType == tt::kCloseBrace) {
    return false;
  }
  return true;
}

bool SimpleValidator2::IsValidDigitPos(TokenList tokens, size_t currIndex) {
  int tokenLeftType = tokens[currIndex - 1].type;
  int tokenRightType = tokens[currIndex + 1].type;

  // Check if DIGIT (current token) appears after another NAME or DIGIT
  if (tokenLeftType == tt::kDigit || tokenLeftType == tt::kName) {
    return false;
  }

  // Checks if DIGIT appears right after ')'
  if (tokenLeftType == tt::kCloseParen) {
    return false;
  }

  // Check if DIGIT is right before '}'
  if (tokenRightType == tt::kCloseBrace) {
    return false;
  }

  return true;
}

bool SimpleValidator2::IsValidIfPos(TokenList tokens, size_t currIndex) {
  int tokenLeftType = tokens[currIndex - 1].type;
  int tokenRightType = tokens[currIndex + 1].type;

  // Check that the 'if' keyword appears in between '}' and '(', or ';' and '('
  if ((tokenLeftType == tt::kCloseBrace || tokenLeftType == tt::kSemicolon) &&
      tokenRightType == tt::kOpenBrace) {
    return true;
  }

  return false;
}

bool SimpleValidator2::IsValidWhilePos(TokenList tokens, size_t currIndex) {
  int tokenLeftType = tokens[currIndex - 1].type;
  int tokenRightType = tokens[currIndex + 1].type;

  // Check that the 'while' keyword appears in between '}' and '(', or ';' and
  // '('
  if ((tokenLeftType == tt::kCloseBrace || tokenLeftType == tt::kSemicolon) &&
      tokenRightType == tt::kOpenBrace) {
    return true;
  }

  return false;
}

bool SimpleValidator2::IsValidSemicolonPos(TokenList tokens, size_t currIndex) {
  int tokenLeftType = tokens[currIndex - 1].type;
  int tokenRightType = tokens[currIndex + 1].type;

  if (tokenLeftType != tt::kName || tokenLeftType != tt::kDigit) {
    return false;
  }

  if (tokenRightType == tt::kSemicolon || tokenRightType == tt::kOpenParen ||
      tokenRightType == tt::kDigit || tokenRightType == tt::kOpenBrace) {
    return false;
  }
  return true;
}