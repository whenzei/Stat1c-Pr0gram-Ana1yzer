#pragma once

#include <string>

#include "simple_validator.h"

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

const static string kKeywords[7] = {"procedure", "if",   "else", "while",
                                    "call",      "read", "print"};

bool SimpleValidator::ValidateProcedure(TokenList tokens, size_t start,
                                        size_t end) {
  size_t numTokens = end - start + 1;

  if (numTokens < kMinimalTokenSizeForPocedure) {
    return false;
  }

  // Check if first token is 'procedure', second token is of type NAME, third
  // token is '{', last token is '}'
  if (tokens[start].type != tt::kKeyword ||
      tokens[start + 1].type != tt::kName ||
      (tokens[start + 2].value).compare(kOpenBraceSymbol) != 0 ||
      (tokens[end].value).compare(kCloseBraceSymbol) != 0) {
    return false;
  }

  for (size_t i = start + 3; i < end; i++) {
    if (tokens[i].type == tt::kAssignment && !IsValidAssignmentPos(tokens, i)) {
      return false;
    }
    if (tokens[i].type == tt::kName && !IsValidNamePos(tokens, i)) {
						return false;
    }
    if (tokens[i].type == tt::kDigit && !IsValidDigitPos(tokens, i)) {
      return false;
    }
  }
  return true;
}

bool SimpleValidator::IsValidAssignmentPos(TokenList tokens, size_t currIndex) {
  if (tokens[currIndex - 1].type != tt::kName ||
      (tokens[currIndex + 1].type != tt::kDigit && tokens[currIndex + 1].type != tt::kName)) {
    return false;
  }
  return true;
}

bool SimpleValidator::IsValidNamePos(TokenList tokens, size_t currIndex) {
  // Check if a name or digit is followed by another name
  if (tokens[currIndex - 1].type == tt::kName || tokens[currIndex - 1].type == tt::kDigit) {
    return false;
  }
  // Check if name is right before '}'
  if (tokens[currIndex + 1].value.compare(kCloseBraceSymbol) == 0) {
    return false;
  }
  return true;
}

bool SimpleValidator::IsValidDigitPos(TokenList tokens, size_t currIndex) {
  // Check if a digit or name is followed by another digit
  if (tokens[currIndex - 1].type == tt::kDigit || tokens[currIndex - 1].type == tt::kName) {
    return false;
  }
  // Check if digit is right before '}'
  if (tokens[currIndex + 1].value.compare(kCloseBraceSymbol) == 0) {
    return false;
  }
}
