#pragma once

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

#include <regex>
#include <vector>

using std::regex;
using std::string;
using std::vector;

struct Token {
  int type;
  string value;
};

struct Result {
  size_t num_consumed_characters;
  struct Token token;
};

class Tokenizer {
  typedef Result (*TokenizerFunc)(string, int);
  int num_functions;

 public:
  static vector<Token> Tokenize(string input);

  /// Debug function, prints out the contents of the token
  /// @returns formatted string of token type and value
  static string Debug(Token);

 private:
  static Result TokenizeCharacter(int type, char value, string input,
                                  int current_index = 0);

  static Result TokenizePattern(int type, regex pattern, string input,
                                int current_index = 0);

  static Result SkipWhitespace(string input, int current_index = 0);

  static Result TokenizeDigits(string input, int current_index = 0);

  static Result TokenizeNames(string input, int current_index = 0);

  static Result TokenizeBraces(string input, int current_index = 0);

  static Result TokenizeSemicolon(string input, int current_index = 0);

  static Result TokenizeEquals(string input, int current_index = 0);
};

#endif  // !SPA_TOKENIZER_H
