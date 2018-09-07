#pragma once

#include <fstream>
#include <iostream>

#include "parser2.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;

using tt = Tokenizer::TokenType;

Parser2::Parser2(PKB* pkb) {
  pkb_ = pkb;
  current_index_ = 0;
}

bool Parser2::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

string Parser2::ReadContentFromFile(string filepath) {
  if (!IsValidFile(filepath)) {
    cout << "File not found! Content is set to empty string" << endl;
    return string();
  }
  // start reading file
  ifstream ifs(filepath);
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

Token Parser2::ReadNextToken() {
  current_token_ = tokens_[current_index_++];
  return current_token_;
}

void Parser2::ProcessKeyword() {
  while (current_token_.type != tt::kBrace) {
    ReadNextToken();
  }
  /*if (current_token_.value == "if") {
    while (current_token_.type != tt::kBrace) {
      ReadNextToken();
    }
  } else if (current_token_.value == "procedure") {
          ReadNextToken();
  }*/
}

void Parser2::ProcessAssignment() {
  // must be a kName since this function is called
  string lhs_var = current_token_.value;
  VariableSet rhs_vars;
  VariableSet rhs_consts;
  while (current_token_.type != tt::kSemicolon) {
    ReadNextToken();
    if (current_token_.type == tt::kName) {
      rhs_vars.insert(current_token_.value);
    }
    if (current_token_.type == tt::kDigit) {
      rhs_consts.insert(current_token_.value);
    }
  }

  string rhsvar = string();
  for (Variable var : rhs_vars) {
    rhsvar = rhsvar + " " + var;
  }
  string rhsconst = string();
  for (Variable var : rhs_consts) {
    rhsconst = rhsconst + " " + var;
  }

  cout << "Assignment statement "
       << " added, indent_lvl: "
       << ", lhs: " << lhs_var << ", rhs_vars: " << rhsvar
       << ", rhs_consts: " << rhsconst << endl;
}

void Parser2::Parse(string filepath) {
  // read content from file
  string contents = ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  ReadNextToken();
  while (current_token_.type != tt::kEOF) {
    if (current_token_.type == tt::kKeyword) {
      ProcessKeyword();
    } else if (current_token_.type == tt::kBrace) {
    } else if (current_token_.type == tt::kName) {
      ProcessAssignment();
    }
    ReadNextToken();
  }
}
