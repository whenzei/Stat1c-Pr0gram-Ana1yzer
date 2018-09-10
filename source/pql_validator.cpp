#include "pql_validator.h"

#include <regex>
using std::regex;
using std::regex_match;

bool PqlValidator::ValidateIdent(string content) {
  return regex_match(content, regex("[a-zA-Z][a-zA-Z0-9]*"));
}