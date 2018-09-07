#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>

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

using tt = Tokenizer::TokenType;

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
  StmtNum stmt_num = 1;
  int stmt_list_index = 0;

  // Second index from start will always be a procedure name
  string procedure_name = tokens_[start + 1].value;
  if (DEBUG_FLAG) {
    std::cout << "Procedure added: " << procedure_name << endl;
  }

  pkb_->InsertProc(procedure_name);

  vector<Token> stmt_list;

  // Starts at 4th index and ends at 2nd last index
  for (size_t i = start + 3; i < end; i++) {
    Token curr_token = tokens_[i];
    stmt_list.push_back(curr_token);

    if (curr_token.type == tt::kSemicolon || curr_token.value == "{") {
      Token first_token = stmt_list[0];
      string statement = GetStmtFromList(stmt_list);

	  // clean stmt_list again
	  stmt_list.clear();

      switch (first_token.type) {
        // if it starts with a name, it must be an assignment
        case tt::kName:
          pkb_->InsertAssignStmt(stmt_num, statement);
          if (DEBUG_FLAG) {
            std::cout << "Statement " << stmt_num << " added: " << statement
                      << endl;
          }
          stmt_num++;
          break;
        case tt::kKeyword:
          // todo: add to relevant tables like IfTable, etc
          if (DEBUG_FLAG) {
            cout << "Keyword statement " << stmt_num
                      << " added: " << statement << endl;
          }
          stmt_num++;
          break;
        // if it starts with a brace, it must be "}" and an else block
        case tt::kBrace:
          // don't do anything
          if (DEBUG_FLAG) {
            cout << "Else statement IGNORED: " << statement << endl;
          }
          break;
      }
      continue;
    }
  }
}

string Parser::GetStmtFromList(vector<Token> list) {
  string stmt = string();
  for (Token token : list) {
    stmt += token.value;
  }

  return stmt;
}