#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "tokenizer.h"
#include "parser.h"
#include "pkb.h"
#include "simple_validator.h"


// Constructor
Parser::Parser(string filepath) {
  content_ = ReadContentFromFile(filepath);
  Parse();
}

string Parser::ReadContentFromFile(string filepath) {
  if (!IsValidFile(filepath)) {
    cout << "File not found! Content is set to empty string" << endl;
    return "";
  }
  // start reading file
  ifstream ifs(filepath);
  string content((istreambuf_iterator<char>(ifs)),
                 (istreambuf_iterator<char>()));
  return content;
}

void Parser::Parse() {
  // retrieve vector of tokens
  TokenList tokenized_content = Tokenizer::Tokenize(content_);

  // if we want to debug
  for (TokenList::const_iterator token = tokenized_content.begin();
       token != tokenized_content.end(); ++token) {
    cout << Tokenizer::Debug(*token) << endl;
  }

  if (SimpleValidator::ValidateProcedure(tokenized_content, 0,
                                         tokenized_content.size() - 1)) {
    // for debugging
    cout << "Procedure " << tokenized_content[1].value
         << " is syntactically correct" << endl;
  }
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

void Parser::ProcessProcedure(TokenList tokens, size_t start, size_t end) {
  int statementNum = 1;
  queue<Token> stmtQueue;
  for (int i = 0; i <= end; i++) {
    Token currToken = tokens[i];
    // Check if it is type NAME and not a SIMPLE keyword
    if (currToken.type == Tokenizer::TokenType::kName &&
        !SimpleValidator::IsKeyword(currToken.value)) {
      // TODO: add to PKB VarTable
      stmtQueue.push(currToken);
    }
    if (currToken.type == Tokenizer::TokenType::kSemicolon) {
      // TODO: pop token out to form statement
      // TODO: update PKB with statement and statementNum
      statementNum++;
    }
  }
}
