#include <cctype>
#include <regex>
#include <vector>

#include "tokenizer.h"

using namespace std;

enum tokenTypes {
  NOTHING = 0,
  DIGIT = 1,
  NAME = 2,
  BRACES = 3,
  SEMICOLON = 4,
  ASSIGNMENT = 5
};

const int numberOfFunctions = 6;

vector<Token> Tokenizer::Tokenize(string input) {
  TokenizerFunc tokenizer_functions[numberOfFunctions] = {
      &SkipWhitespace, &TokenizeDigits,    &TokenizeNames,
      &TokenizeBraces, &TokenizeSemicolon, &TokenizeEquals};
  size_t current_index = 0, vector_len = input.size();
  vector<Token> tokens;

  while (current_index < vector_len) {
    bool is_done = false;
    for (int i = 0; i < numberOfFunctions; i++) {
      if (is_done) {
        break;
      }

      Result result = tokenizer_functions[i](input, current_index);
      if (result.num_consumed_characters != 0) {
        is_done = true;
        current_index += result.num_consumed_characters;
      }

      if (result.token.type != NOTHING) {
        tokens.push_back(result.token);
      }
    }
  }

  return tokens;
}

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

Result Tokenizer::TokenizeCharacter(int type, char value, string input,
                                    int current_index) {
  Result result = {};
  if (value == input[current_index]) {
    result = {1, {type, string(1, value)}};
  } else {
    result = {0, {NOTHING, string()}};
  }

  return result;
}

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

  Result result = {0, {NOTHING, string()}};
  return result;
}

Result Tokenizer::SkipWhitespace(string input, int current_index) {
  Result result = {};
  if (iswspace(input[current_index])) {
    result = {1, {NOTHING, string()}};
  } else {
    result = {0, {NOTHING, string()}};
  }

  return result;
}

Result Tokenizer::TokenizeDigits(string input, int current_index) {
  return TokenizePattern(DIGIT, regex{R"([0-9])"}, input, current_index);
}

Result Tokenizer::TokenizeNames(string input, int current_index) {
  return TokenizePattern(NAME, regex{R"([a-zA-Z])"}, input, current_index);
}

Result Tokenizer::TokenizeBraces(string input, int current_index) {
  return TokenizePattern(BRACES, regex{R"([{}])"}, input, current_index);
}

Result Tokenizer::TokenizeSemicolon(string input, int current_index) {
  return TokenizeCharacter(SEMICOLON, ';', input, current_index);
}

Result Tokenizer::TokenizeEquals(string input, int current_index) {
  return TokenizeCharacter(ASSIGNMENT, '=', input, current_index);
}
