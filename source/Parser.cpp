#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "Parser.h"
#include "TNode.h"
#include "Tokenizer.h"

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
  // check whether it is procedure (as we are currently guaranteed to only have
  // 1 procedure)
  vector<string> tokenized_content = Tokenizer::tokenize(content_);
  for (vector<string>::const_iterator i = tokenized_content.begin();
       i != tokenized_content.end(); ++i) {
    std::cout << *i << "<--- \n";
  }
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}
