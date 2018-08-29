#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "pkb.h"
#include "parser.h"
#include "tokenizer.h"

Parser::Parser(std::string filepath) {
  content_ = ReadContentFromFile(filepath);
  Parse();
}

string Parser::ReadContentFromFile(std::string filepath) {
  if (!IsValidFile(filepath)) {
    std::cout << "File not found! Content is set to empty string" << endl;
    return "";
  }
  // start reading file
  std::ifstream ifs(filepath);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                 (std::istreambuf_iterator<char>()));
  return content;
}

void Parser::Parse() {
  // check whether it is procedure (as we are currently guaranteed to only have
  // 1 procedure)
  std::vector<std::string> tokenized_content = Tokenizer::tokenize(content_);
  for (std::vector<std::string>::const_iterator i = tokenized_content.begin();
       i != tokenized_content.end(); ++i) {
    std::cout << *i << "<--- \n";
  }
}

bool Parser::IsValidFile(std::string filepath) {
  std::ifstream infile(filepath);
  return infile.good();
}
