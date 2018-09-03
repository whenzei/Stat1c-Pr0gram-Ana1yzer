#pragma once

#include "tokenizer.h"
#include "parser.h"
#include "pkb.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;

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
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}
