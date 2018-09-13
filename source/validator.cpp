#include <unordered_set>

#include "tokenizer.h"
#include "validator.h"

using std::cout;
using std::endl;

using tt = Tokenizer::TokenType;

Token curr_token_;
int curr_index_;
TokenList tokens_;

const std::unordered_set<string> kKeywords({"procedure", "read", "call",
                                            "print", "if", "then", "else",
                                            "while"});

void Validator::Validate(TokenList tokens) {
  tokens_ = tokens;
  curr_token_ = tokens_.front();
  while (curr_token_.type == tt::kName && curr_token_.value == "procedure") {
    if (!IsValidProcedure()) {
      break;
    }
  }

  if (curr_token_.type != tt::kEOF) {
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

  cout << endl;

  return list;
}

Token Validator::ReadNextToken() {
  curr_token_ = tokens_[curr_index_++];
  return curr_token_;
}

Token Validator::PeekNextToken() { return tokens_[curr_index_ + 1]; }

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
    // "{" stmtList "}"
    while (curr_token_.type != tt::kCloseBrace) {
      if (!IsValidStatement()) {
        return false;
      }
    }
  }

  return true;
}

bool Validator::IsValidStatement() {
  // stmt: read | print | call | while | if | assign
  // check if first token is keyword?
  ReadNextToken();
  // must be kName
  if (curr_token_.type != tt::kName) {
    return false;
  }

  if (kKeywords.count(curr_token_.value)) {
    // check next token, if it is "=" means assignment
    if (PeekNextToken().type == tt::kAssignment) {
      return IsValidAssignment();
    }

    if (curr_token_.value == "if") {
      return IsValidIfBlock();
    }

    if (curr_token_.value == "while") {
      return IsValidWhileBlock();
    }

    // if not "if" or "while", must be "call", "read" or "print"
    return IsValidCallReadPrint();
  }

  return IsValidAssignment();
}

bool Validator::IsValidCallReadPrint() { return false; }

bool Validator::IsValidAssignment() { return false; }

bool Validator::IsValidExpression() { return false; }

bool Validator::IsValidIfBlock() { return false; }

bool Validator::IsValidWhileBlock() { return false; }

bool Validator::IsValidNestedBlock() { return false; }
