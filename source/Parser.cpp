#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"
#include "Parser.h"
#include "Tokenizer.h"

Parser::Parser(string filepath) {
	this->content = readContentFromFile(filepath);
	parse();
}


string Parser::readContentFromFile(string filepath)
{	
	if (!isValidFile(filepath)) {
		cout << "File not found! Content is set to empty string" << endl;
		return "";
	}
	// start reading file
	ifstream ifs(filepath);
	string content((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));
	return content;
}

void Parser::parse() {
	// check whether it is procedure (as we are currently guaranteed to only have 1 procedure)
	vector<string> tokenizedContent = Tokenizer::tokenize(this->content);
	for (vector<string>::const_iterator i = tokenizedContent.begin(); i != tokenizedContent.end(); ++i)
		std::cout << *i << "<--- \n";
}

bool Parser::isValidFile(string filepath) {
	ifstream infile(filepath);
	return infile.good();
}
