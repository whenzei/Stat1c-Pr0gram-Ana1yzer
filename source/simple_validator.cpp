#pragma once

#include <string>

#include "simple_validator.h"

const string kOpenBraceSymbol = "{";
const string kCloseBraceSymbol = "}";

const size_t minimalTokenSizeForPocedure = 4;

const static string kKeywords[7] = {"procedure", "if", "else", "while",
                                   "call", "read", "print"};

bool SimpleValidator::isKeyword(string input) { 
		for (string keyword : kKeywords) {
    if (keyword.compare(input) == 0) {
      return true;
				}
		}
  return false;
}

bool SimpleValidator::isKeyword(int type, string input) {
  return kKeywords[type].compare(input) == 0;
}

bool SimpleValidator::validateProcedure(vector<Token> tokens, size_t start, size_t end) {
  size_t numTokens = end - start + 1;
		
		if (numTokens < minimalTokenSizeForPocedure) {
    return false;
		}
 
		// Check if first token is 'procedure', second token is of type NAME, third token is '{', last token is '}'
		if ((tokens[start].value).compare(kKeywords[kProcedure]) != 0 || tokens[start + 1].type != kName ||
						(tokens[start + 2].value).compare(kOpenBraceSymbol) != 0 || (tokens[end].value).compare(kCloseBraceSymbol) != 0) {
    return false;
		}

  for (size_t i = start + 3; i < end; i++) {
				
    if (tokens[i].type == kAssignment) {
     // Check if the left of '=' is a name and the right of '=' is a digit or name 
						if (tokens[i - 1].type != kName ||
          (tokens[i + 1].type != kDigit && tokens[i + 1].type != kName)) {
        return false;
						}         
				}
    if (tokens[i].type == kName) {
					// Check if a name or digit is followed by another name
						if (tokens[i-1].type == kName || tokens[i-1].type == kDigit) {
        return false;
						}
    }
    if (tokens[i].type == kDigit) {
					// Check if a digit or name is followed by another digit
      if (tokens[i-1].type == kDigit || tokens[i-1].type == kName) {
        return false;
						}
    }
		}
  return true;
}


