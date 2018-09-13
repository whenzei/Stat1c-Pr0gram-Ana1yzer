#include "validator.h"
#include "tokenizer.h"

using std::cout;
using std::endl;

using tt = Tokenizer::TokenType;

Token curr_token_;
int curr_index_;
TokenList tokens_;

void Validator::Validate(TokenList tokens) {
  tokens_ = tokens;
  curr_token_ = tokens_.front();
  bool error_flag = false;
  while (curr_token_.type == tt::kName && curr_token_.value == "procedure") {
    if (!IsValidProcedure()) {
      break;
    }
  }

  if (curr_token_.type != tt::kEOF || error_flag) {
    // do something here, means validation failed
    cout << "Validation failed, stream not fully consumed" << endl;
  }
}

TokenList Validator::ReadNextTokens(int num_tokens) {
  TokenList list;
  for (int i = 0; i < num_tokens; i++) {
    curr_token_ = tokens_[curr_index_++];
    list.push_back(curr_token_);
  }

  return list;
}

bool MatchTypes(TokenList syntax_block, vector<tt> expected_types) {
  for (int i = 0; i < syntax_block.size(); i++) {
    if (syntax_block[i].type != expected_types[i]) {
      return false;
    }
  }

  return true;
}

bool Validator::IsValidProcedure() {
  // 'procedure' variable '{'
  TokenList syntax_block = ReadNextTokens(3);
  // don't have to check if syntax_block[0] == "procedure", since it must be to
  // invoke this function
  vector<tt> expected_types = {tt::kName, tt::kName, tt::kOpenBrace};
  // matches syntax, now check stmtList
  if (MatchTypes(syntax_block, expected_types)) {
    // "}" stmtList "{"
    while (curr_token_.type != tt::kCloseBrace) {
      if (!IsValidStatement()) {
        return false;
      }
    }
  }

  return true;
}

bool Validator::IsValidStatement() { return false; }

bool Validator::IsValidExpression() { return false; }

bool Validator::IsValidIfBlock() { return false; }

bool Validator::IsValidWhileBlock() { return false; }

bool Validator::IsValidNestedBlock() { return false; }
