#include <cctype>
#include <regex>
#include <vector>

#include "tokenizer.h"

using namespace std;

enum TokenTypes {
  kNothing = 0,
  kDigit = 1,
  kName = 2,
  kBrace = 3,
  kSemicolon = 4,
  kAssignment = 5
};

const int kNumberOfFunctions = 6;

// Uses various tokenizer functions such as SkipWhiteSpace or TokenizeDigits
// to tokenize the supplied input, returning a vector of Token
vector<Token> Tokenizer::Tokenize(string input) {
  TokenizerFunc tokenizer_functions[kNumberOfFunctions] = {
      &SkipWhitespace, &TokenizeDigits,    &TokenizeNames,
      &TokenizeBraces, &TokenizeSemicolon, &TokenizeEquals};
  size_t current_index = 0, vector_len = input.size();
  vector<Token> tokens;

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
  }

  return tokens;
}

// Debug function, returns the contents of the token as a string
string Tokenizer::Debug(Token token) {
  string type;

  switch (token.type) {
    case 1:
      type = "DIGIT     ";
      break;
    case 2:
      type = "NAME      ";
      break;
    case 3:
      type = "BRACES    ";
      break;
    case 4:
      type = "SEMICOLON ";
      break;
    case 5:
      type = "ASSIGNMENT";
      break;
    default:
      type = "UNKNOWN   ";
  }
  return "[ type: " + type + " || value: " + token.value + " ]";
}

// Checks whether the current character at input[current_index]
// matches with the supplied value.
// Returns a Result with the structure [num_consumed_characters, [type, value]]
// If matched, num_consumed_characters = 1, and the relevant type and value is
// returned, else, num_consumed_characters = 0, and returns enum kNothing with
// empty string.
Result Tokenizer::TokenizeCharacter(int type, char value, string input,
                                    int current_index) {
  Result result = {};
  if (value == input[current_index]) {
    result = {1, {type, string(1, value)}};
  } else {
    result = {0, {kNothing, string()}};
  }

  return result;
}

// Checks whether the current character at input[current_index]
// matches with the supplied regex pattern.
// Keeps checking the next character if matched until the next character stops
// matching. Returns a Result with the structure [num_consumed_characters,
// [type, value]] If matched, num_consumed_characters, and the relevant type and
// value is returned, else, num_consumed_characters = 0, and returns enum
// kNothing with empty string.
Result Tokenizer::TokenizePattern(int type, regex pattern, string input,
                                  int current_index) {
  string current_char = string(1, input[current_index]);
  size_t num_consumed_characters = 0, input_len = input.size();

  if (regex_match(current_char, pattern)) {
    string value = "";
    while (num_consumed_characters < input_len &&
           regex_match(current_char, pattern)) {
      value += current_char;
      num_consumed_characters++;
      current_char = string(1, input[current_index + num_consumed_characters]);
    }
    Result result = {num_consumed_characters, {type, value}};
    return result;
  }

  Result result = {0, {kNothing, string()}};
  return result;
}

// Checks if the current character at input[current_index] is a whitespace, and
// returns a Token with num_consumed_characters = 1 if true,
// num_consumed_characters = 0 otherwise.
Result Tokenizer::SkipWhitespace(string input, int current_index) {
  Result result = {};
  if (iswspace(input[current_index])) {
    result = {1, {kNothing, string()}};
  } else {
    result = {0, {kNothing, string()}};
  }

  return result;
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize digits,
// and returns the result as a Result struct
Result Tokenizer::TokenizeDigits(string input, int current_index) {
  return TokenizePattern(kDigit, regex{R"([0-9])"}, input, current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize names,
// and returns the result as a Result struct
Result Tokenizer::TokenizeNames(string input, int current_index) {
  return TokenizePattern(kName, regex{R"([a-zA-Z])"}, input, current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize both opening and
// closing brace, and returns the result as a Result struct
Result Tokenizer::TokenizeBraces(string input, int current_index) {
  return TokenizePattern(kBrace, regex{R"([{}])"}, input, current_index);
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
