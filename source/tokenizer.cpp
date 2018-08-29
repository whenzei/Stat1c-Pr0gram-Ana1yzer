#include <regex>
#include <vector>

#include "tokenizer.h"

std::vector<std::string> Tokenizer::tokenize(std::string input) {
  std::vector<std::string> tokens;
  std::regex regex{R"([{};]+)"};  // split on space and comma
  std::sregex_token_iterator it{input.begin(), input.end(), regex, -1};
  tokens = {it, {}};
  return tokens;
}
