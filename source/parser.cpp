#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "parser.h"
#include "pkb.h"
#include "simple_validator.h"
#include "tokenizer.h"

const bool DEBUG_FLAG = true;

// Constructor
Parser::Parser(PKB pkb) { pkb_ = pkb; }

// Setters
void Parser::SetPkb(PKB pkb) {}

// Getters
TokenList Parser::GetTokenList() { return TokenList(); }

PKB Parser::GetPkb() { return PKB(); }

string Parser::ReadContentFromFile(string filepath) {
  if (!IsValidFile(filepath)) {
    cout << "File not found! Content is set to empty string" << endl;
    return string();
  }
  // start reading file
  ifstream ifs(filepath);
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

void Parser::Parse(string filepath) {
  // read content from file
  string contents = ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  if (DEBUG_FLAG) {
    for (Token& token : tokens_) {
      cout << Tokenizer::Debug(token) << endl;
    }
  }

  if (SimpleValidator::ValidateProcedure(tokens_, 0, tokens_.size() - 1)) {
    if (DEBUG_FLAG) {
      cout << "Procedure " << tokens_[1].value << " is syntactically correct"
           << endl;
    }
    ProcessProcedure(0, tokens_.size() - 1);
  }
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

void Parser::ProcessProcedure(size_t start, size_t end) {
  int statementNum = 1;

  // Second index from start will always be a procedure name
  if (DEBUG_FLAG) {
    std::cout << "Procedure added: " << tokens_[start + 1].value << endl;
  }
  // TODO: add procedure name to PKB's ProcTable

  queue<Token> stmtQueue;
  // Starts at 4th index and ends at 2nd last index
  for (size_t i = start + 3; i < end; i++) {
    Token currToken = tokens_[i];

    // Check if it is type NAME and not a SIMPLE keyword
    if (currToken.type == Tokenizer::TokenType::kName &&
        !SimpleValidator::IsKeyword(currToken.value)) {
      // TODO: add to PKB's VarTable
      if (DEBUG_FLAG) {
        std::cout << "Variable added: " << currToken.value << endl;
      }
    }

    if (currToken.type == Tokenizer::TokenType::kSemicolon) {
      // TODO: update PKB's StmtTable with statement and statementNum
      string statement = "";
      while (!stmtQueue.empty()) {
        Token token = stmtQueue.front();
        statement += token.value;
        stmtQueue.pop();
      }
      statement += currToken.value;
      if (DEBUG_FLAG) {
        std::cout << "Statement " << statementNum << " added: " << statement
                  << endl;
      }
      statementNum++;
      continue;
    }
    stmtQueue.push(currToken);
  }
}
