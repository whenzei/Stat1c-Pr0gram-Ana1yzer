
#pragma once

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <string>
#include <vector>
#include <unordered_set>

#include "pkb.h"
#include "tokenizer.h"

using std::string;
using std::vector;
using std::unordered_set;

struct StmtAssignInfo {
	string lhs_variable;
	unordered_set<string> rhs_variables;
	unordered_set<string> rhs_constants;
};

// The parser class for the front end component
// Used to parse file contents and pass the parsed content to the PKB component
class Parser {
  PKB* pkb_;
  TokenList tokens_;

 public:
  // Constructor
  Parser();
  Parser(PKB* pkb);

  // Destructor
  //~Parser();

  // Setters
  void SetPkb(PKB* pkb);

  // Getters
  TokenList GetTokenList();
  PKB GetPkb();

  // Main function, parses the file located at filepath and stores the TokenList
  // into member variable tokens_
  void Parse(string filepath);

 private:
  // Reads the content of the file at the location specified by the file path
  // and returns the file contents if available
  // @param filepath the filepath of the file to be parsed
  // @returns file content as a string if file exists, or empty string otherwise
  string ReadContentFromFile(string filepath);

  // Checks if file can be located at given file path
  // @param filepath the filepath of the file to be located
  // @returns true if file exists, false otherwise
  bool IsValidFile(string filepath);

  // Reads the list of tokens representing a procedure and updates PKB
  void ProcessProcedure(size_t start, size_t end);

  StmtAssignInfo Parser::GetAssignmentInfo(vector<Token> stmt);
};

#endif  // !SPA_PARSER_H
