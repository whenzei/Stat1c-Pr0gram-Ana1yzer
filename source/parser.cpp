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
#include "statement.h"
#include "tokenizer.h"

const bool DEBUG_FLAG = true;

using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::queue;

// Constructor
Parser::Parser() {}
Parser::Parser(PKB* pkb) { pkb_ = pkb; }

// Setters
void Parser::SetPkb(PKB* pkb) { pkb_ = pkb; }

// Getters
TokenList Parser::GetTokenList() { return tokens_; }

PKB Parser::GetPkb() { return *pkb_; }

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
  string procedure_name = tokens_[start + 1].value;
  if (DEBUG_FLAG) {
    std::cout << "Procedure added: " << procedure_name << endl;
  }

  pkb_->InsertProc(procedure_name);

  queue<Token> stmtQueue;

  // Starts at 4th index and ends at 2nd last index
  for (size_t i = start + 3; i < end; i++) {
    Token currToken = tokens_[i];

    // Check if it is type NAME and not a SIMPLE keyword
    if (currToken.type == Tokenizer::TokenType::kName) {
      // TODO: add to PKB's VarTable
      if (DEBUG_FLAG) {
        std::cout << "Variable added: " << currToken.value << endl;
      }
    }

    if (currToken.type == Tokenizer::TokenType::kSemicolon) {
      string statement = "";
      while (!stmtQueue.empty()) {
        Token token = stmtQueue.front();
        statement += token.value;
        stmtQueue.pop();
      }
      statement += currToken.value;

      pkb_->InsertAssignStmt(statementNum, statement);

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
