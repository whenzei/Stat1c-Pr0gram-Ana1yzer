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

// Validates the tokenized source code
// Precondition: must be called after the constructor
bool Validator::ValidateProgram() {
  do {
    if (!IsValidProcedure()) {
      cout << "Validation failed, invalid procedure detected" << endl;
      return false;
    }
  } while (PeekNextToken().subtype == ts::kProcedure);

  // last token should be kEOF if everything is processed
  if (!IsAtEnd()) {
    cout << "Validation failed, stream not fully consumed" << endl;
    return false;
  }

  cout << "Validation passed" << endl;
  return true;
}

bool Validator::IsValidProcedure() {
  // 'procedure' var_name '{'
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

  for (size_t i = 1; i < tokens.size(); i++) {
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
          cout << "[IMBALANCED PARENTHESIS -- INVALID]" << endl;
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

// Validates the given cond_expr
bool Validator::IsValidConditional(TokenList cond_expr) {
  // cond_expr: rel_expr | ‘!’ ‘(’ cond_expr ‘)’ | ‘(’ cond_expr ‘)’ ‘&&’ ‘(’
  // cond_expr ‘)’ | ‘(’ cond_expr ‘)’ ‘||’ ‘(’ cond_expr ‘)’

  // minimum size should be 3: "a<b"
  if (cond_expr.size() < 3) {
    return false;
  }

  if (cond_expr[0].value == "!") {
    // should be of the form ‘!’ ‘(’ cond_expr ‘)’
    if (cond_expr[1].type != tt::kOpenParen ||
        cond_expr.back().type != tt::kCloseParen) {
      return false;
    }
    TokenList spliced_expr(cond_expr.begin() + 2, cond_expr.end() - 1);
    return IsValidConditional(spliced_expr);
  }

  // check if contains conditionals ("||" or "&&")
  if (ContainsConditionalType(cond_expr)) {
    // format should be ‘(’ cond_expr ‘)’ ‘||’ ‘(’ cond_expr ‘)’
    if (cond_expr[0].type != tt::kOpenParen) {
      return false;
    }
    stack<tt> paren_stack;
    paren_stack.push(tt::kOpenParen);
    TokenList lhs;

    size_t index = 1;
    for (index; index < cond_expr.size(); index++) {
      if (cond_expr[index].type == tt::kCloseParen) {
        paren_stack.pop();
        if (paren_stack.empty()) break;
      } else if (cond_expr[index].type == tt::kOpenParen) {
        paren_stack.push(tt::kOpenParen);
      }
      lhs.push_back(cond_expr[index]);
    }

    // next token should be at a conditional symbol
    if (cond_expr[++index].type != tt::kConditional) {
      return false;
    }

    // check rhs conditional syntax
    if (cond_expr[index + 1].type != tt::kOpenParen ||
        cond_expr.back().type != tt::kCloseParen) {
      return false;
    }

    TokenList rhs(cond_expr.begin() + index + 2, cond_expr.end() - 1);

    return IsValidConditional(lhs) && IsValidConditional(rhs);

  } else {
    // is relational expression
    int delimit = GetIndexOfRelational(cond_expr);
    if (delimit == -1) {
      return IsValidExpression(cond_expr);
    }
    TokenList lhs(cond_expr.begin(), cond_expr.begin() + delimit);
    TokenList rhs(cond_expr.begin() + delimit + 1, cond_expr.end());

    return IsValidExpression(lhs) && IsValidExpression(rhs);
  }
}

bool Validator::IsValidIfBlock() {
  // if: ‘if’ ‘(’ cond_expr ‘)’ ‘then’ ‘{‘ stmtLst ‘}’ ‘else’ ‘{‘ stmtLst ‘}’

  TokenList cond_expr;
  while (PeekNextToken().subtype != ts::kThen && !IsAtEnd()) {
    cond_expr.push_back(ReadNextToken());
  }

  // must be minimum of size 5: "(a<b)"
  if (cond_expr.size() < 5) {
    return false;
  }

  // cond_expr must start and end with parenthesis
  if (cond_expr.front().type != tt::kOpenParen ||
      cond_expr.back().type != tt::kCloseParen) {
    return false;
  }

  TokenList spliced_expr(cond_expr.begin() + 1, cond_expr.end() - 1);

  if (!IsValidConditional(spliced_expr)) {
    return false;
  }

  // "then" "{"
  if (ReadNextToken().subtype != ts::kThen || !Match(tt::kOpenBrace)) {
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

  // final closing brace
  return Match(tt::kCloseBrace);
}

bool Validator::IsValidWhileBlock() {
  // while: ‘while’ ‘(’ cond_expr ‘)’ ‘ { ‘ stmtLst ‘ }’
  TokenList cond_expr;
  while (PeekNextToken().type != tt::kOpenBrace && !IsAtEnd()) {
    cond_expr.push_back(ReadNextToken());
  }

  // must be minimum of size 5: "(a<b)"
  if (cond_expr.size() < 5) {
    return false;
  }

  // cond_expr must start and end with parenthesis
  if (cond_expr.front().type != tt::kOpenParen ||
      cond_expr.back().type != tt::kCloseParen) {
    return false;
  }

  TokenList spliced_expr(cond_expr.begin() + 1, cond_expr.end() - 1);

  if (!IsValidConditional(spliced_expr)) {
    return false;
  }

  // "{"
  if (!Match(tt::kOpenBrace)) {
    return false;
  }

  if (!IsValidStmtList()) {
    return false;
  }

  // final closing brace
  return Match(tt::kCloseBrace);
}

/**********************
BEGIN HELPER FUNCTIONS
**********************/
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

// Returns index of relational symbols in given TokenList if exists.
// Else returns -1;
int Validator::GetIndexOfRelational(TokenList tokens) {
  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i].type == tt::kRelational) {
      return i;
    }
  }
  return -1;
}

bool Validator::ContainsConditionalType(TokenList tokens) {
  for (auto token : tokens) {
    if (token.type == tt::kConditional) {
      return true;
    }
  }
  return false;
}