#pragma once

#ifndef SPA_PARSER2_H
#define SPA_PARSER2_H

#include <iostream>
#include <unordered_set>

#include "tokenizer.h"
#include "pkb.h"

using std::string;
using std::unordered_set;

using Variable = string;
using VariableSet = unordered_set<string>;

class Parser2 {
private:
	PKB* pkb_;
	TokenList tokens_;
	Token current_token_;
	int current_index_;

	bool IsValidFile(string filepath);
	string ReadContentFromFile(string filepath);
	Token ReadNextToken();
	void ProcessKeyword();
	void ProcessAssignment();

public:
	Parser2(PKB* pkb);
	void Parse(string filepath);
};

#endif //!SPA_PARSER2_H