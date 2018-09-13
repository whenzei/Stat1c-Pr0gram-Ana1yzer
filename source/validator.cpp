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

  if (curr_token_.type != tt::kEOF) {
    // do something here, means validation failed
    cout << "Validation failed, stream not fully consumed" << endl;
  }
}

TokenList Validator::ReadNextTokens(int num_tokens) {
  TokenList list;
  for (int i = 0; i < num_tokens; i++) {
    curr_token_ = tokens_[curr_index_++];
    cout << Tokenizer::Debug(curr_token_) << " ";
    list.push_back(curr_token_);
  }

  cout << endl;

  return list;
}

Token Validator::ReadNextToken() {
  curr_token_ = tokens_[curr_index_++];
  cout << Tokenizer::Debug(curr_token_) << " ";
  return curr_token_;
}

Token Validator::PeekNextToken() { return tokens_[curr_index_]; }

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
  if (!MatchTypes(syntax_block, expected_types)) {
    cout << "[INVALID]" << endl;
    return false;
  }
  cout << "[PROC SYNTAX VALID]" << endl;
  // "{" stmtList "}"
  if (!IsValidStmtList()) {
    cout << "[STMTLIST INVALID]" << endl;
    return false;
  }

  return ReadNextToken().type == tt::kCloseBrace;
}

bool Validator::IsValidStmtList() {
  while (PeekNextToken().type != tt::kCloseBrace) {
    if (!IsValidStatement()) {
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

  return IsValidAssignment();
}

bool Validator::IsValidCallReadPrint() {
  // [call] variable ";"
  TokenList syntax_block = ReadNextTokens(2);
  vector<tt> expected_types = {tt::kName, tt::kSemicolon};

  return MatchTypes(syntax_block, expected_types);
}

bool Validator::IsValidAssignment() {
  // TODO: Add expression checking with Shunting Yard Alg
  if (ReadNextToken().type != tt::kAssignment) {
    return false;
  }
  TokenList syntax_block;
  while (curr_token_.type != tt::kSemicolon) {
    syntax_block.push_back(ReadNextToken());
  }

  // always return true for now
  cout << "[ASSIGN VALID]" << endl;
  return true;
}

bool Validator::IsValidExpression() {
  cout << "[EXPR INVALID]" << endl;
  return false;
}

bool Validator::IsValidConditional() {
  // currently only check for kName -> kConditional -> kName
  TokenList syntax_block = ReadNextTokens(3);
  vector<tt> expected_types = {tt::kName, tt::kConditional, tt::kName};

  return MatchTypes(syntax_block, expected_types);
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
  cout << "[CONDITIONAL VALID]" << endl;

  // ") then" "{"
  TokenList syntax_block = ReadNextTokens(3);
  if (syntax_block[0].type != tt::kCloseParen ||
      syntax_block[1].subtype != ts::kThen ||
      syntax_block[2].type != tt::kOpenBrace) {
    return false;
  }
  cout << "[)_THEN_{ VALID]" << endl;

  if (!IsValidStmtList()) {
    return false;
  }
  cout << "[STMTLIST VALID]" << endl;

  // "} else {"
  syntax_block = ReadNextTokens(3);
  if (syntax_block[0].type != tt::kCloseBrace ||
      syntax_block[1].subtype != ts::kElse ||
      syntax_block[2].type != tt::kOpenBrace) {
    return false;
  }
  cout << "[}_ELSE_{ VALID]" << endl;

  if (!IsValidStmtList()) {
    return false;
  }
  cout << "[STMTLIST VALID]" << endl;

  if (ReadNextToken().type != tt::kCloseBrace) {
    cout << "[IF_BLOCK INVALID]" << endl;
    return false;
  }

  cout << "[IF_BLOCK VALID]" << endl;
  return true;
}

bool Validator::IsValidWhileBlock() {
  // while: ‘while’ ‘(’ cond_expr ‘)’ ‘ { ‘ stmtLst ‘ }’
  cout << "[WHILE_BLOCK INVALID]" << endl;
  return false;
}

bool Validator::IsValidNestedBlock() {
  cout << "[NESTED_INVALID]" << endl;
  return false;
}
