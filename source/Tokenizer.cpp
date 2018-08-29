#include <regex>
#include <vector>

#include "Tokenizer.h"

using namespace std;

vector<string> Tokenizer::tokenize(string input) {
  vector<string> tokens;
  regex regex{R"([\r\n]+)"};  // split on space and comma
  sregex_token_iterator it{input.begin(), input.end(), regex, -1};
  tokens = {it, {}};
  return tokens;
}
