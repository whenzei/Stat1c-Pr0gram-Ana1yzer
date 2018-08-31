#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include "parser.h"
#include "pkb.h"
#include "simple_validator.h"
#include "tokenizer.h"


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
  vector<Token> tokenized_content = Tokenizer::Tokenize(content_);

  // if we want to debug
  for (vector<Token>::const_iterator token = tokenized_content.begin();
       token != tokenized_content.end(); ++token) {
    cout << Tokenizer::Debug(*token) << endl;
  }
  vector<Pair> procedures = SplitProcedures(tokenized_content);

		for (size_t i = 0; i < procedures.size(); i++) {
    if (SimpleValidator::validateProcedure(
            tokenized_content, procedures[i].first, procedures[i].second)) {
					// for debugging
      cout << "Procedure " << tokenized_content[procedures[i].first + 1].value << " is syntactically correct" << endl;
    }
		}
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

vector<Pair> Parser::SplitProcedures(vector<Token> tokens) {
  vector<Pair> procedures;
  for (size_t i = 0; i < tokens.size(); i++) {
    string token_value = tokens[i].value;
	
    if (SimpleValidator::isKeyword(kProcedure, token_value)) {
      stack<string> bracesStack;

      // Search for first occurence of '{' after keyword 'procedure'
      size_t j;
						for (j = i + 1; j < tokens.size(); j++) {
        if (tokens[j].value == "{") {
          bracesStack.push("{");
          break;
								}
						}

						// Search for corresponding '}' of 'procedure'
						for (size_t k = j + 1; k < tokens.size(); k++) {
								if (tokens[k].value == "{") {
										bracesStack.push("{");
								}
								if (tokens[k].value == "}") {
										bracesStack.pop();
								}
								if (bracesStack.empty()) {
										Pair procedure = {i, k};
										procedures.push_back(procedure);
										i = j - 1;
										break;
								}
						}
				}
		}
  return procedures;
}
