#pragma once

#include <regex>
#include <vector>

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

struct Token {
  int type;
  std::string value;
};

struct Result {
  size_t num_consumed_characters;
  struct Token token;
};

class Tokenizer {
  typedef Result (*TokenizerFunc)(std::string, int);
  int num_functions;

 public:
  static std::vector<Token> Tokenize(std::string input);

  /// Debug function, prints out the contents of the token
  /// @returns formatted string of token type and value
  static std::string Debug(Token);

 private:
  static Result TokenizeCharacter(int type, char value, std::string input,
                                  int current_index = 0);

  static Result TokenizePattern(int type, std::regex pattern, std::string input,
                                int current_index = 0);

  static Result SkipWhitespace(std::string input, int current_index = 0);

  static Result TokenizeDigits(std::string input, int current_index = 0);

  static Result TokenizeNames(std::string input, int current_index = 0);

  static Result TokenizeBraces(std::string input, int current_index = 0);

  static Result TokenizeSemicolon(std::string input, int current_index = 0);

  static Result TokenizeEquals(std::string input, int current_index = 0);
};

#endif  // !SPA_TOKENIZER_H
