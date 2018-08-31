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
  // Uses various tokenizer functions such as SkipWhiteSpace or TokenizeDigits
  // to tokenize the supplied input, returning a vector of Token
  static vector<Token> Tokenize(string input);

  // Debug function, returns the contents of the token as a string
  static string Debug(Token);

 private:
  // Checks whether the current character at input[current_index]
  // matches with the supplied value.
  // Returns a Result with the structure [num_consumed_characters, [type,
  // value]] If matched, num_consumed_characters = 1, and the relevant type and
  // value is returned, else, num_consumed_characters = 0, and returns enum
  // kNothing with empty string.
  static Result TokenizeCharacter(int type, char value, string input,
                                  int current_index = 0);

  // Checks whether the current character at input[current_index]
  // matches with the supplied regex pattern.
  // Keeps checking the next character if matched until the next character stops
  // matching. Returns a Result with the structure [num_consumed_characters,
  // [type, value]] If matched, num_consumed_characters, and the relevant type
  // and value is returned, else, num_consumed_characters = 0, and returns enum
  // kNothing with empty string.
  static Result TokenizePattern(int type, regex pattern, string input,
                                int current_index = 0);

  // Checks if the current character at input[current_index] is a whitespace,
  // and returns a Token with num_consumed_characters = 1 if true,
  // num_consumed_characters = 0 otherwise.
  static Result SkipWhitespace(string input, int current_index = 0);

  // Checks if input is a comment using regex and
  // returns a Result if true, with the num_consumed_characters to skip,
  // Tokenizer::EmptyResult() otherwise
  static Result Tokenizer::SkipComments(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize digits,
  // and returns the result as a Result struct
  static Result TokenizeDigits(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize names,
  // and returns the result as a Result struct
  static Result TokenizeNames(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize both opening
  // and closing brace, and returns the result as a Result struct
  static Result TokenizeBraces(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize  operators
  // and returns the result as a Result struct
  static Result TokenizeOperators(string input, int current_index = 0);

  // Uses Tokenizer::TokenizeCharacter(...) with ';' as the supplied value to
  // tokenize semicolons, and returns the result as a Result struct
  static Result TokenizeSemicolon(string input, int current_index = 0);

  // Uses Tokenizer::TokenizeCharacter(...) with '=' as the supplied value to
  // tokenize the equals sign, and returns the result as a Result struct
  static Result TokenizeEquals(string input, int current_index = 0);

  // Helper function to return empty result, meaning tokenization did not find a
  // match
  static Result EmptyResult();
};

#endif  // !SPA_TOKENIZER_H
