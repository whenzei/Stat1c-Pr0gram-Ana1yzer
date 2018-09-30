#include <cctype>
#include <regex>
#include <unordered_map>
#include <vector>

#include "tokenizer.h"

using ts = Tokenizer::TokenSubtype;

const std::unordered_map<string, Tokenizer::TokenSubtype> kKeywordsToEnum = {
    {"procedure", ts::kProcedure}, {"if", ts::kIf},       {"then", ts::kThen},
    {"else", ts::kElse},           {"while", ts::kWhile}, {"print", ts::kPrint},
    {"call", ts::kCall},           {"read", ts::kRead}};

static const string kTokenTypeNames[] = {
    "nothing",    "digit",     "name",      "word",       "openbrace",
    "closebrace", "semicolon", "comma",     "underscore", "quotation",
    "assignment", "operator",  "openparen", "closeparen", "period", 
    "hash",       "conditional", "relational", "keyword", "unknown",
    "EOF"};

static const string kTokenSubtypeNames[] = {
    "NONE", "PROC", "IF", "THEN", "ELSE", "WHILE", "PRINT", "CALL", "PRINT"};

const int kNumberOfFunctions = 11;

// Uses various tokenizer functions such as SkipWhiteSpace or TokenizeDigits
// to tokenize the supplied input, returning a list of Tokens
TokenList Tokenizer::Tokenize(string input) {
  TokenizerFunc tokenizer_functions[kNumberOfFunctions] = {
      &SkipComments,        &SkipWhitespace,      &TokenizeDigits,
      &TokenizeNames,       &TokenizeBraces,      &TokenizeSemicolon,
      &TokenizeEquals,      &TokenizeOperators,   &TokenizeParenthesis,
      &TokenizeRelationals, &TokenizeConditionals};
  return Tokenize(input, tokenizer_functions);
}

// Take in an array of tokenizer functions such as SkipWhiteSpace or
// TokenizeDigits to tokenize the supplied input, returning a list of tokens
TokenList Tokenizer::Tokenize(string input,
                              TokenizerFunc tokenizer_functions[]) {
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

  // ADD END OF FILE TOKEN
  tokens.push_back(Token({kEOF, string()}));

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
  Result result = TokenizePattern(kName, regex{R"([a-zA-Z][a-zA-Z0-9]*)"},
                                  input, current_index);
  auto search = kKeywordsToEnum.find(result.token.value);
  if (search != kKeywordsToEnum.end()) {
    result.token.subtype = search->second;
  }
  return result;
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize words (can be
// alphanumeric or symbols), and returns the result as a Result struct
Result Tokenizer::TokenizeWords(string input, int current_index) {
  Result result =
      TokenizePattern(kWord, regex{R"(\w+)"}, input, current_index);

  return result;
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize both opening and
// closing brace, and returns the result as a Result struct.
// Note that it will only tokenize a single brace at a time, e.g. "{{" will be
// returned as two separate results
Result Tokenizer::TokenizeBraces(string input, int current_index) {
  Result result =
      TokenizePattern(kOpenBrace, regex{R"([{}])"}, input, current_index);

  if (result.token.value == "}") {
    result.token.type = kCloseBrace;
  }

  return result;
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize both opening
// and closing brackets ("(" and ")"), and returns the result as a Result
// struct. Note that it will only tokenize a single bracket at a time, e.g. "(("
// will be returned as two separate results
Result Tokenizer::TokenizeParenthesis(string input, int current_index) {
  Result result =
      TokenizePattern(kOpenParen, regex{R"([()])"}, input, current_index);

  if (result.token.value == ")") {
    result.token.type = kCloseParen;
  }

  return result;
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize operators
// and returns the result as a Result struct
Result Tokenizer::TokenizeOperators(string input, int current_index) {
  return TokenizePattern(kOperator, regex{R"([+\-/%*])"}, input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with ';' as the supplied value to
// tokenize semicolons, and returns the result as a Result struct
Result Tokenizer::TokenizeSemicolon(string input, int current_index) {
  return TokenizeCharacter(kSemicolon, ';', input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with ',' as the supplied value to
// tokenize commas, and returns the result as a Result struct
Result Tokenizer::TokenizeComma(string input, int current_index) {
  return TokenizeCharacter(kComma, ',', input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with '_' as the supplied value to
// tokenize underscores, and returns the result as a Result struct
Result Tokenizer::TokenizeUnderscore(string input, int current_index) {
  return TokenizeCharacter(kUnderscore, '_', input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with '"' as the supplied value to
// tokenize double quotation marks, and returns the result as a Result struct
Result Tokenizer::TokenizeQuotation(string input, int current_index) {
  return TokenizeCharacter(kQuotation, '"', input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with '.' as the supplied value to
// tokenize period, and returns the result as a Result struct
Result Tokenizer::TokenizePeriod(string input, int current_index) {
  return TokenizeCharacter(kPeriod, '\.', input, current_index);
}

// Uses Tokenizer::TokenizeCharacter(...) with '#' as the supplied value to
// tokenize hash, and returns the result as a Result struct
Result Tokenizer::TokenizeHash(string input, int current_index) {
  return TokenizeCharacter(kHash, '#', input, current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to
// tokenize the equals symbol, returns a result with kRelational type if "=="
// matches, and kAssignment type if a single '=' is matched.
Result Tokenizer::TokenizeEquals(string input, int current_index) {
  Result result(
      TokenizePattern(kAssignment, regex{R"(==?)"}, input, current_index));
  if (result.num_consumed_characters == 2) {
    result.token.type = kRelational;
  }

  return result;
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize relational
// expressions (">", "<", "!=", ">=", "<=") and returns the result as a Result
// struct
// Precondition: This function must be after Tokenizer::TokenizeEquals if
// used in a function pointer array, or "=" will be classified as a kConditional
// type instead of kAssignment type due to regex.
Result Tokenizer::TokenizeRelationals(string input, int current_index) {
  // check if it is of the form !=
  Result result(
      TokenizePattern(kRelational, regex("[!]=?"), input, current_index));
  if (result.num_consumed_characters == 2) {
    // must be exactly of the form !=, if not try to tokenize with [><]
    return result;
  }
  return TokenizePattern(kRelational, regex{R"([><=]=?)"}, input,
                         current_index);
}

// Uses Tokenizer::TokenizePattern(...) with regex to tokenize conditional
// expressions ("!", "&&", "||") and returns the result as a Result struct
// Precondition: This function must be after Tokenizer::TokenizeRelationals if
// used in a function pointer array, or "!=" will not be tokenized properly due
// to this function eating the "!" symbol
Result Tokenizer::TokenizeConditionals(string input, int current_index) {
  // check if it is of the form !=
  Result result(TokenizeCharacter(kConditional, '!', input, current_index));
  if (result.num_consumed_characters == 1) {
    // must be exactly of the form !, if not try to tokenize with the other
    // regex
    return result;
  }
  result = TokenizePattern(kConditional, regex{R"([&|][&|]?)"}, input,
                           current_index);
  // must be of the form "||" or "&&"
  return (result.num_consumed_characters == 2 &&
          (result.token.value == "&&" || result.token.value == "||"))
             ? result
             : EmptyResult();
}

// Helper function to return empty result, meaning
// tokenization did not find a match
Result Tokenizer::EmptyResult() { return Result({0, {kNothing}}); }

// Debug function, returns the contents of the token as a string
string Tokenizer::Debug(Token token) {
  return "<" + kTokenTypeNames[token.type] + "> " + token.value;
  // return token.value;
}
