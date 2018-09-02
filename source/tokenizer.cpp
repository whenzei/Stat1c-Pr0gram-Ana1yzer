#include <cctype>
#include <regex>
#include <vector>

#include "tokenizer.h"

enum TokenType {
  kNothing = 0,
  kDigit = 1,
  kName = 2,
  kBrace = 3,
  kSemicolon = 4,
  kAssignment = 5,
  kOperator = 6,
  kUnknown = 999,
};

const int kNumberOfFunctions = 8;

// Uses various tokenizer functions such as SkipWhiteSpace or TokenizeDigits
// to tokenize the supplied input, returning a list of Tokens
TokenList Tokenizer::Tokenize(string input) {
  TokenizerFunc tokenizer_functions[kNumberOfFunctions] = {
      &SkipComments,   &SkipWhitespace,    &TokenizeDigits, &TokenizeNames,
      &TokenizeBraces, &TokenizeSemicolon, &TokenizeEquals, &TokenizeOperators};
  size_t current_index = 0, vector_len = input.size();
  TokenList tokens;

  while (current_index < vector_len) {
    bool is_done = false;

    // iterate through all the functions in the function pointer array
    for (int i = 0; i < kNumberOfFunctions; i++) {
      if (is_done) {
        break;
      }

      Result result = tokenizer_functions[i](input, current_index);
      if (result.num_consumed_characters != 0) {
        is_done = true;
        current_index += result.num_consumed_characters;
      }

      if (result.token.type != kNothing) {
        tokens.push_back(result.token);
      }
    }

    // unknown token, since none of the tokenizers can recognize it
    // TODO: decide if we throw an exception here
    if (!is_done) {
      tokens.push_back(Token({kUnknown, string(1, input[current_index++])}));
    }
  }

  return tokens;
}

// Checks whether the current character at input[current_index]
// matches with the supplied value.
// Returns a Result with the structure [num_consumed_characters, [type, value]]
// If matched, num_consumed_characters = 1, and the relevant type and value is
// returned, else, num_consumed_characters = 0, and returns enum kNothing with
// empty string.
Result Tokenizer::TokenizeCharacter(TokenType type, char value, string input,
                                    int current_index) {
  return (value == input[current_index]) ? Result({1, {type, string(1, value)}})
                                         : EmptyResult();
}

// Checks whether the current value at input[current_index]
// matches with the supplied regex pattern.
// Keeps appending the next character to the value until it stops matching.
// Returns a Result with the structure [num_consumed_characters,
// [type, value]] if matched and returns an empty result with
// Tokenizer::EmptyResult() if no matches.
Result Tokenizer::TokenizePattern(TokenType type, regex pattern, string input,
                                  int current_index) {
  string value = string(1, input[current_index]);
  size_t num_consumed_characters = 0, input_len = input.size();

  while (num_consumed_characters < input_len && regex_match(value, pattern)) {
    num_consumed_characters++;
    value += input[current_index + num_consumed_characters];
  }

  if (num_consumed_characters > 0) {
    value.pop_back();  // remove last added character
    return Result({num_consumed_characters, {type, value}});
  } else {
    return EmptyResult();
  }
}

// Checks if the current character at input[current_index] is a whitespace, and
// returns a Result with num_consumed_characters = 1 if true,
// Tokenizer::EmptyResult() otherwise.
Result Tokenizer::SkipWhitespace(string input, int current_index) {
  return (iswspace(input[current_index])) ? Result({1, {kNothing}})
                                          : EmptyResult();
}

// Checks if input is a comment using regex and
// returns a Result if true, with the num_consumed_characters to skip,
// Tokenizer::EmptyResult() otherwise
Result Tokenizer::SkipComments(string input, int current_index) {
  return TokenizePattern(kNothing, regex{R"(\\{1}.*)"}, input, current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize digits,
// and returns the result as a Result struct
Result Tokenizer::TokenizeDigits(string input, int current_index) {
  return TokenizePattern(kDigit, regex{R"([0-9]+)"}, input, current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize names,
// and returns the result as a Result struct
Result Tokenizer::TokenizeNames(string input, int current_index) {
  return TokenizePattern(kName, regex{R"([a-zA-Z][a-zA-Z0-9]*)"}, input,
                         current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize both opening and
// closing brace, and returns the result as a Result struct
Result Tokenizer::TokenizeBraces(string input, int current_index) {
  return TokenizePattern(kBrace, regex{R"([{}])"}, input, current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize  operators
// and returns the result as a Result struct
Result Tokenizer::TokenizeOperators(string input, int current_index) {
  return TokenizePattern(kOperator, regex{R"([+-/%*])"}, input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with ';' as the supplied value to
// tokenize semicolons, and returns the result as a Result struct
Result Tokenizer::TokenizeSemicolon(string input, int current_index) {
  return TokenizeCharacter(kSemicolon, ';', input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with '=' as the supplied value to
// tokenize the equals sign, and returns the result as a Result struct
Result Tokenizer::TokenizeEquals(string input, int current_index) {
  return TokenizeCharacter(kAssignment, '=', input, current_index);
}

// Helper function to return empty result, meaning tokenization did not find a
// match
Result Tokenizer::EmptyResult() { return Result({0, {kNothing}}); }

// Debug function, returns the contents of the token as a string
string Tokenizer::Debug(Token token) {
  string type;

  switch (token.type) {
    case 1:
      type = "<DIGIT>";
      break;
    case 2:
      type = "<NAME>";
      break;
    case 3:
      type = "<BRACE>";
      break;
    case 4:
      type = "<SEMICOLON>";
      break;
    case 5:
      type = "<ASSIGNMENT>";
      break;
    case 6:
      type = "<OPERATOR>";
      break;
    default:
      type = "<UNKNOWN>";
  }
  return type + token.value;
}