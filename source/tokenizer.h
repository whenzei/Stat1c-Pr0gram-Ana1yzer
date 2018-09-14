#pragma once

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

#include <regex>
#include <tuple>
#include <vector>

using std::regex;
using std::string;
using std::vector;

struct Token {
  int type;
  string value;

  bool operator==(const Token& other) const {
    return std::tie(type, value) == std::tie(other.type, other.value);
  }
};

struct Result {
  size_t num_consumed_characters;
  struct Token token;

  bool operator==(const Result& other) const {
    return std::tie(num_consumed_characters, token) ==
           std::tie(other.num_consumed_characters, other.token);
  }
};

using TokenList = vector<Token>;
using TokenizerFunc = Result (*)(string, int);

class Tokenizer {
 public:
  enum TokenType {
    kNothing,
    kDigit,
    kName,
    kOpenBrace,
    kCloseBrace,
    kSemicolon,
    kAssignment,
    kOperator,
    kOpenParen,
    kCloseParen,
    kConditional,
    kRelational,
    kUnknown,
	kEOF,
  };

  // Uses various tokenizer functions such as SkipWhiteSpace or TokenizeDigits
  // to tokenize the supplied input, returning a list of tokens
  static TokenList Tokenize(string input);

  // Debug function, returns the contents of the token as a string
  static string Debug(Token);

  // Checks whether the current character at input[current_index]
  // matches with the supplied value.
  // Returns a Result with the structure [num_consumed_characters, [type,
  // value]] If matched, num_consumed_characters = 1, and the relevant type and
  // value is returned, else, num_consumed_characters = 0, and returns enum
  // kNothing with empty string.
  static Result TokenizeCharacter(TokenType type, char value, string input,
                                  int current_index = 0);

  // Checks whether the current character at input[current_index]
  // matches with the supplied regex pattern.
  // Keeps checking the next character if matched until the next character stops
  // matching. Returns a Result with the structure [num_consumed_characters,
  // [type, value]] If matched, num_consumed_characters, and the relevant type
  // and value is returned, else, num_consumed_characters = 0, and returns enum
  // kNothing with empty string.
  static Result TokenizePattern(TokenType type, regex pattern, string input,
                                int current_index = 0);

  // Checks if the current character at input[current_index] is a whitespace,
  // and returns a Token with num_consumed_characters = 1 if true,
  // num_consumed_characters = 0 otherwise.
  static Result SkipWhitespace(string input, int current_index = 0);

  // Checks if input is a comment using regex and
  // returns a Result if true, with the num_consumed_characters to skip,
  // Tokenizer::EmptyResult() otherwise
  static Result SkipComments(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize digits,
  // and returns the result as a Result struct
  static Result TokenizeDigits(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize names,
  // and returns the result as a Result struct
  static Result TokenizeNames(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize both opening
  // and closing brace, and returns the result as a Result struct.
  // Note that it will only tokenize a single brace at a time, e.g. "{{" will be
  // returned as two separate results
  static Result TokenizeBraces(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize both opening
  // and closing parenthesises ("(" and ")"), and returns the result as a Result
  // struct. Note that it will only tokenize a single bracket at a time, e.g.
  // "((" will be returned as two separate results
  static Result TokenizeParenthesis(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize  operators
  // and returns the result as a Result struct
  static Result TokenizeOperators(string input, int current_index = 0);

  // Uses Tokenizer::TokenizeCharacter(...) with ';' as the supplied value to
  // tokenize semicolons, and returns the result as a Result struct
  static Result TokenizeSemicolon(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to
  // tokenize the equals symbol, returns a result with kConditional type if "=="
  // matches, and kAssignment type if a single '=' is matched.
  static Result TokenizeEquals(string input, int current_index = 0);

  // Uses Tokenizer::TokenizePattern(...) with regex to tokenize relational
  // expressions (">", "<", "!=", ">=", "<=") and returns the result as a Result
  // struct
  static Result Tokenizer::TokenizeRelationals(string input, int current_index);

  // Helper function to return empty result, meaning tokenization did not find a
  // match
  static Result EmptyResult();
};

#endif  // !SPA_TOKENIZER_H
