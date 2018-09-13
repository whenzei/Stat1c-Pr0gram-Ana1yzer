#include <unordered_set>

#include "tokenizer.h"
#include "validator.h"

using std::cout;
using std::endl;

using tt = Tokenizer::TokenType;
using ts = Tokenizer::TokenSubtype;

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

  if (ReadNextToken().type != tt::kEOF) {
    // do something here, means validation failed
    cout << "Validation failed, stream not fully consumed" << endl;
  }

  cout << "Validation passed" << endl;
}

bool Validator::IsValidProcedure() {
  // 'procedure' variable '{'
  TokenList syntax_block = ReadNextTokens(3);
  // don't have to check if syntax_block[0] == "procedure", since it must be to
  // invoke this function
  vector<tt> expected_types = {tt::kName, tt::kName, tt::kOpenBrace};
  // matches syntax, now check stmtList
  if (!MatchTypes(syntax_block, expected_types)) {
    cout << "[PROC SYNTAX INVALID]" << endl;
    return false;
  }
  // "{" stmtList "}"
  if (!IsValidStmtList()) {
    return false;
  }

  // last closing brace "}"
  return ReadNextToken().type == tt::kCloseBrace;
}

bool Validator::IsValidStmtList() {
  while (PeekNextToken().type != tt::kCloseBrace) {
    if (!IsValidStatement()) {
      cout << "[STMTLIST SYNTAX INVALID]" << endl;
      return false;
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
    // else must be keywords, can use subtype
    if (curr_token_.subtype == ts::kIf) {
      return IsValidIfBlock();
    }
    if (curr_token_.subtype == ts::kWhile) {
      return IsValidWhileBlock();
    }
    // if not "if" or "while", must be "call", "read" or "print"
    return IsValidCallReadPrint();
  }

  // must be assignment
  return IsValidAssignment();
}

bool Validator::IsValidCallReadPrint() {
  // [call] variable ";"
  TokenList syntax_block = ReadNextTokens(2);
  vector<tt> expected_types = {tt::kName, tt::kSemicolon};

  if (!MatchTypes(syntax_block, expected_types)) {
    cout << "[CALL/READ/PRINT INVALID]" << endl;
    return false;
  }
  return true;
}

bool Validator::IsValidAssignment() {
  // TODO: Add expression checking with Shunting Yard Alg
  if (ReadNextToken().type != tt::kAssignment) {
    cout << "[ASSIGN SYNTAX INVALID]" << endl;
    return false;
  }
  TokenList syntax_block;
  while (curr_token_.type != tt::kSemicolon) {
    syntax_block.push_back(ReadNextToken());
  }

  // always return true if fit syntax for now
  return true;
}

bool Validator::IsValidConditional() {
  // currently only check for kName -> kConditional -> kName
  TokenList syntax_block = ReadNextTokens(3);
  vector<tt> expected_types = {tt::kName, tt::kConditional, tt::kName};

  if (!MatchTypes(syntax_block, expected_types)) {
    cout << "[CONDITIONAL INVALID]" << endl;
    return false;
  }

  return true;
}

bool Validator::IsValidIfBlock() {
  // if: ‘if’ ‘(’ cond_expr ‘)’ ‘then’ ‘{‘ stmtLst ‘}’ ‘else’ ‘{‘ stmtLst ‘}’
  // "("
  if (ReadNextToken().type != tt::kOpenParen) {
    return false;
  }

  if (!IsValidConditional()) {
    return false;
  }

  // ") then" "{"
  TokenList syntax_block = ReadNextTokens(3);
  if (syntax_block[0].type != tt::kCloseParen ||
      syntax_block[1].subtype != ts::kThen ||
      syntax_block[2].type != tt::kOpenBrace) {
    return false;
  }

  if (!IsValidStmtList()) {
    return false;
  }

  // "} else {"
  syntax_block = ReadNextTokens(3);
  if (syntax_block[0].type != tt::kCloseBrace ||
      syntax_block[1].subtype != ts::kElse ||
      syntax_block[2].type != tt::kOpenBrace) {
    return false;
  }

  if (!IsValidStmtList()) {
    return false;
  }

  if (ReadNextToken().type != tt::kCloseBrace) {
    return false;
  }

  // passed all hurdles
  return true;
}

bool Validator::IsValidWhileBlock() {
  // while: ‘while’ ‘(’ cond_expr ‘)’ ‘ { ‘ stmtLst ‘ }’
  if (ReadNextToken().type != tt::kOpenParen) {
    return false;
  }

  if (!IsValidConditional()) {
    return false;
  }

  // ") {"
  TokenList syntax_block = ReadNextTokens(2);
  if (syntax_block[0].type != tt::kCloseParen ||
      syntax_block[1].type != tt::kOpenBrace) {
    return false;
  }

  if (!IsValidStmtList()) {
    return false;
  }

  if (ReadNextToken().type != tt::kCloseBrace) {
    return false;
  }

  // passed all hurdles
  return true;
}

// Helper functions

TokenList Validator::ReadNextTokens(int num_tokens) {
  TokenList list;
  for (int i = 0; i < num_tokens; i++) {
    curr_token_ = tokens_[curr_index_++];
    list.push_back(curr_token_);
  }
  return list;
}

Token Validator::ReadNextToken() {
  curr_token_ = tokens_[curr_index_++];
  return curr_token_;
}

Token Validator::PeekNextToken() { return tokens_[curr_index_]; }

bool Validator::MatchTypes(TokenList syntax_block, vector<tt> expected_types) {
  for (int i = 0; i < syntax_block.size(); i++) {
    if (syntax_block[i].type != expected_types[i]) {
      return false;
    }
  }

  return true;
}
