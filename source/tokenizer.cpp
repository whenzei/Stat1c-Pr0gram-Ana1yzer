#include <cctype>
#include <regex>
#include <vector>

#include "tokenizer.h"

enum TOKEN_TYPES {
  NOTHING = 0,
  DIGIT = 1,
  NAME = 2,
  BRACES = 3,
  SEMICOLON = 4,
  ASSIGNMENT = 5
};

const int NUM_FUNCTIONS = 6;

std::vector<Token> Tokenizer::Tokenize(std::string input) {
  TokenizerFunc tokenizer_functions[NUM_FUNCTIONS] = {
      &SkipWhitespace, &TokenizeDigits,    &TokenizeNames,
      &TokenizeBraces, &TokenizeSemicolon, &TokenizeEquals};
  size_t current_index = 0, vector_len = input.size();
  std::vector<Token> tokens;

  while (current_index < vector_len) {
    bool is_done = false;
    for (int i = 0; i < NUM_FUNCTIONS; i++) {
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

std::string Tokenizer::Debug(Token token) {
  std::string type;

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

Result Tokenizer::TokenizeCharacter(int type, char value, std::string input,
                                    int current_index) {
  Result result = {};
  if (value == input[current_index]) {
    result = {1, {type, std::string(1, value)}};
  } else {
    result = {0, {NOTHING, std::string()}};
  }

  return result;
}

Result Tokenizer::TokenizePattern(int type, std::regex pattern,
                                  std::string input, int current_index) {
  std::string current_char = std::string(1, input[current_index]);
  size_t num_consumed_characters = 0, input_len = input.size();

  if (std::regex_match(current_char, pattern)) {
    std::string value = "";
    while (num_consumed_characters < input_len &&
           std::regex_match(current_char, pattern)) {
      value += current_char;
      num_consumed_characters++;
      current_char =
          std::string(1, input[current_index + num_consumed_characters]);
    }
    Result result = {num_consumed_characters, {type, value}};
    return result;
  }

  Result result = {0, {NOTHING, std::string()}};
  return result;
}

Result Tokenizer::SkipWhitespace(std::string input, int current_index) {
  Result result = {};
  if (iswspace(input[current_index])) {
    result = {1, {NOTHING, std::string()}};
  } else {
    result = {0, {NOTHING, std::string()}};
  }

  return result;
}

Result Tokenizer::TokenizeDigits(std::string input, int current_index) {
  return TokenizePattern(DIGIT, std::regex{R"([0-9])"}, input, current_index);
}

Result Tokenizer::TokenizeNames(std::string input, int current_index) {
  return TokenizePattern(NAME, std::regex{R"([a-zA-Z])"}, input, current_index);
}

Result Tokenizer::TokenizeBraces(std::string input, int current_index) {
  return TokenizePattern(BRACES, std::regex{R"([{}])"}, input, current_index);
}

Result Tokenizer::TokenizeSemicolon(std::string input, int current_index) {
  return TokenizeCharacter(SEMICOLON, ';', input, current_index);
}

Result Tokenizer::TokenizeEquals(std::string input, int current_index) {
  return TokenizeCharacter(ASSIGNMENT, '=', input, current_index);
}
