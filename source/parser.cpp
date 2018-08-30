#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
#include "pkb.h"
#include "tokenizer.h"

// Constructor
Parser::Parser(std::string filepath) {
  content_ = ReadContentFromFile(filepath);
  Parse();
}

string Parser::ReadContentFromFile(std::string filepath) {
  if (!IsValidFile(filepath)) {
    std::cout << "File not found! Content is set to empty string" << std::endl;
    return "";
  }
  // start reading file
  std::ifstream ifs(filepath);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  return content;
}

void Parser::Parse() {
  // retrieve vector of tokens
  std::vector<Token> tokenized_content = Tokenizer::Tokenize(content_);

  // if we want to debug
  for (std::vector<Token>::const_iterator token = tokenized_content.begin();
       token != tokenized_content.end(); ++token) {
    std::cout << Tokenizer::Debug(*token) << std::endl;
  }
}

bool Parser::IsValidFile(std::string filepath) {
  std::ifstream infile(filepath);
  return infile.good();
}
