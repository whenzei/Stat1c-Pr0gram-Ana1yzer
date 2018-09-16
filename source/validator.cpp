#include "validator.h"
#include "tokenizer.h"

#include <stack>
#include <unordered_set>

using std::cout;
using std::endl;
using std::stack;

using KeywordSet = std::unordered_set<string>;
using tt = Tokenizer::TokenType;
using ts = Tokenizer::TokenSubtype;

// Token curr_token_;
int curr_index_;
TokenList tokens_;
Token curr_token_;

const KeywordSet kKeywords({"procedure", "read", "call", "print", "if", "then",
                            "else", "while"});

Validator::Validator(TokenList tokens) {
  tokens_ = tokens;
  curr_index_ = 0;
}

// this function is mainly for testing
void Validator::ResetTokenList(TokenList tokens) {
  tokens_ = tokens;
  curr_index_ = 0;
}

// must be called after the constructor
bool Validator::ValidateProgram() {
  while (PeekNextToken().subtype == ts::kProcedure) {
    if (!IsValidProcedure()) {
      cout << "Validation failed, invalid procedure detected" << endl;
      return false;
    }
  }

  // last token should be kEOF if everything is processed
  if (!IsAtEnd()) {
    // do something here, means validation failed
    cout << "Validation failed, stream not fully consumed" << endl;
    return false;
  }

  cout << "Validation passed" << endl;
  return true;
}

bool Validator::IsValidProcedure() {
  // ['procedure'] variable '{'
  if (!MatchNext(3, {tt::kName, tt::kName, tt::kOpenBrace})) {
    cout << "[PROC SYNTAX INVALID]" << endl;
    return false;
  }
  // stmtList
  if (!IsValidStmtList()) {
    return false;
  }

  // "}"
  return Match(tt::kCloseBrace);
}

bool Validator::IsValidStmtList() {
  // must have at least 1 stmt
  do {
    if (!IsValidStatement()) {
      cout << "[STMTLIST SYNTAX INVALID]" << endl;
      return false;
    }
  } while (PeekNextToken().type != tt::kCloseBrace && !IsAtEnd());
  return true;
}

bool Validator::IsValidStatement() {
  // stmt: read | print | call | while | if | assign
  // check if first token is keyword?
  // must be kName
  if (!Match(tt::kName)) {
    return false;
  }

  // is keyword
  if (curr_token_.subtype != ts::kNone) {
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
  if (!MatchNext(2, {tt::kName, tt::kSemicolon})) {
    cout << "[CALL/READ/PRINT INVALID]" << endl;
    return false;
  }
  return true;
}

bool Validator::IsValidExpression(TokenList tokens) {
  // expr: expr ‘+’ term | expr ‘-’ term | term
  // term: term ‘*’ factor | term ‘ / ’ factor | term ‘%’ factor | factor
  // factor : var_name | const_value | ‘(’ expr ‘)’
  // expression should always start with kDigit or kName
  // in the style v o v o v, where v is a variable or digit, and o is operator
  if (tokens.empty()) {
    return false;
  }
  bool was_operator = false;
  stack<tt> parenthesis_stack;
  Token current = tokens[0];
  if (current.type != tt::kOpenParen && current.type != tt::kName &&
      current.type != tt::kDigit) {
    return false;
  }

  if (current.type == tt::kOpenParen) {
    parenthesis_stack.push(tt::kOpenParen);
    // false truth since next variable after this shouldn't be an operator
    was_operator = true;
  }

  for (int i = 1; i < tokens.size(); i++) {
    current = tokens[i];
    if (was_operator) {
      if (current.type != tt::kDigit && current.type != tt::kName &&
          current.type != tt::kOpenParen) {
        cout << "[EXPR INVALID], current token: " << current.value << endl;
        return false;
      }

      if (current.type == tt::kOpenParen) {
        parenthesis_stack.push(tt::kOpenParen);
      } else {
        was_operator = false;
      }
    } else {
      if (current.type != tt::kCloseParen && current.type != tt::kOperator) {
        cout << "[EXPR INVALID], current token: " << current.value << endl;
        return false;
      }

      if (current.type == tt::kCloseParen) {
        if (parenthesis_stack.empty()) {
          cout << "[STACK NOT EMPTY -- INVALID]" << endl;
          return false;
        }
        parenthesis_stack.pop();
      } else {
        was_operator = true;
      }
    }
  }

  if (!parenthesis_stack.empty()) {
    cout << "[STACK NOT EMPTY -- INVALID]" << endl;
    return false;
  }

  return true;
}

// Checks whether the tokens starting at curr_index_ makes up a valid assignment
// Precondition: Should be called by IsValidStatement, and expects the
// curr_token_ to be of type kName, and starts checking from kAssign token
bool Validator::IsValidAssignment() {
  // assign: var_name ‘=’ expr ‘;’
  if (!Match(tt::kAssignment)) {
    cout << "[ASSIGN SYNTAX INVALID]" << endl;
    return false;
  }

  TokenList expr;
  while (PeekNextToken().type != tt::kSemicolon && !IsAtEnd()) {
	  expr.push_back(ReadNextToken());
  }
  
  if (!IsValidExpression(expr)) {
	  return false;
  }

  return Match(tt::kSemicolon);
}

bool Validator::IsValidConditional() {
  // cond_expr: rel_expr | ‘!’ ‘(’ cond_expr ‘)’ | ‘(’ cond_expr ‘)’ ‘&&’ ‘(’
  // cond_expr ‘)’ | ‘(’ cond_expr ‘)’ ‘||’ ‘(’ cond_expr ‘)’
  // rel_expr: rel_factor ‘>’ rel_factor | rel_factor ‘>=’ rel_factor |
  // rel_factor ‘<’ rel_factor | rel_factor ‘<=’   rel_factor | rel_factor ‘==’
  // rel_factor | rel_factor ‘!=’ rel_factor
  // rel_factor: var_name | const_value | expr

  // currently only check for kName -> kRelational -> kName
  if (!(MatchNext(3, {tt::kName, tt::kRelational, tt::kName}))) {
    cout << "[CONDITIONAL INVALID]" << endl;
    return false;
  }
  /*if (ReadNextToken().value == "!") {
    return IsValidConditional();
  }*/

  return true;
}

bool Validator::IsValidIfBlock() {
  // if: ‘if’ ‘(’ cond_expr ‘)’ ‘then’ ‘{‘ stmtLst ‘}’ ‘else’ ‘{‘ stmtLst ‘}’
  // "("
  if (ReadNextToken().type != tt::kOpenParen) {
    return false;
  }

  // TODO: validate whole block, with parenthesis included
  if (!IsValidConditional()) {
    return false;
  }

  // ") then" "{"
  if (!Match(tt::kCloseParen) || ReadNextToken().subtype != ts::kThen ||
      !Match(tt::kOpenBrace)) {
    return false;
  }

  if (!IsValidStmtList()) {
    return false;
  }

  // "} else {"
  if (!Match(tt::kCloseBrace) || ReadNextToken().subtype != ts::kElse ||
      !Match(tt::kOpenBrace)) {
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
  if (!MatchNext(2, {tt::kCloseParen, tt::kOpenBrace})) {
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
bool Validator::Match(Tokenizer::TokenType type) {
  if (Check(type)) {
    ReadNextToken();
    return true;
  }
  return false;
}

bool Validator::Check(Tokenizer::TokenType type) {
  return IsAtEnd() ? false : PeekNextToken().type == type;
}

bool Validator::IsAtEnd() { return PeekNextToken().type == tt::kEOF; }

Token Validator::ReadNextToken() {
  if (!IsAtEnd()) {
    curr_index_++;
  }
  curr_token_ = tokens_[curr_index_ - 1];
  return curr_token_;
}

Token Validator::PeekNextToken() { return tokens_[curr_index_]; }

Token Validator::GetPreviousToken() { return tokens_[curr_index_ - 1]; }

bool Validator::MatchNext(int num, vector<tt> expected_types) {
  for (int i = 0; i < num; i++) {
    if (!Match(expected_types[i])) {
      return false;
    }
  }
  return true;
}
