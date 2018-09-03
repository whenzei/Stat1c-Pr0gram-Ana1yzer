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

// Constructor
Parser::Parser(string filepath, PKB pkb) {
  content_ = ReadContentFromFile(filepath);
  Parse(pkb);
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

void Parser::Parse(PKB pkb) {
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
    ProcessProcedure(tokenized_content, 0, tokenized_content.size() - 1, pkb);
  }
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

void Parser::ProcessProcedure(TokenList tokens, size_t start, size_t end,
                              PKB pkb) {
  int statementNum = 1;

		// Second index from start will always be a procedure name 
  std::cout << "Procedure added: " << tokens[start + 1].value << endl;
		// TODO: add procedure name to PKB's ProcTable

  queue<Token> stmtQueue;
		// Starts at 4th index and ends at 2nd last index
  for (size_t i = start + 3; i < end; i++) {
    Token currToken = tokens[i];

    // Check if it is type NAME and not a SIMPLE keyword
    if (currToken.type == Tokenizer::TokenType::kName &&
        !SimpleValidator::IsKeyword(currToken.value)) {
      // TODO: add to PKB's VarTable
      std::cout << "Variable added: " << currToken.value << endl;
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
      std::cout << "Statment " << statementNum << " added: "<< statement << endl;
      statementNum++;
						continue;
    }
    stmtQueue.push(currToken);
  }
}
