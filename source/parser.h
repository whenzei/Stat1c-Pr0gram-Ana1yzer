
#pragma once

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <tokenizer.h>
#include <vector>

#include "pkb.h" 

using std::string;
using std::vector;

/**
 * The parser class for the front end component
 * Used to parse file contents and pass the parsed content to the PKB component
 */
class Parser {
  string content_;

 public:
  /* Constructor */
  Parser(string filepath, PKB pkb);

  ///* Destructor */
  //~Parser();

  /**
   * Reads the content of the file at the location specified by the file path
   * and returns the file contents if available
   * @param filepath the filepath of the file to be parsed
   * @returns file content as a string if file exists, or empty string otherwise
   */
  string ReadContentFromFile(string filepath);

  void Parse(PKB pkb);

 private:
  /**
   * Checks if file can be located at given file path
   * @param filepath the filepath of the file to be located
   * @returns true if file exists, false otherwise
   */
  bool IsValidFile(string filepath);

  /**
   * Reads the list of tokens representing a procedure and updates PKB
   */
  void ProcessProcedure(TokenList tokens, size_t start, size_t end, PKB pkb);
};

#endif  // !SPA_PARSER_H
