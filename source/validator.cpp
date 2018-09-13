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
  while (curr_token_.type == tt::kName && curr_token_.value == "procedure") {
    ValidateProcedure();
  }

  if (curr_token_.type != tt::kEOF) {
    // do something here, means validation failed
    cout << "Validation failed, stream not fully consumed" << endl;
  }
}

TokenList Validator::ReadNextTokens(int num_tokens) {
  TokenList list;
  for (int i = 0; i < num_tokens; i++) {
    list.push_back(tokens_[curr_index_++]);
  }

  return list;
}

bool Validator::ValidateProcedure() {
  // 'procedure' variable '{'
  TokenList syntax_block = ReadNextTokens(3);
  for (Token tok : syntax_block) {
    cout << Tokenizer::Debug(tok) << endl;
  }

  return false;
}

bool Validator::ValidateExpression() { return false; }

bool Validator::ValidateIfBlock() { return false; }

bool Validator::ValidateWhileBlock() { return false; }

bool Validator::ValidateNestedBlock() { return false; }
