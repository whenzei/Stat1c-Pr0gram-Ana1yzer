#pragma once

#ifndef PQL_PARSER_H
#define PQL_PARSER_H

#include <string>
#include <vector>
#include "pql_query.h"
#include "tokenizer.h"
using std::string;
using std::vector;

/* A class that parses a query in PQL */
class PqlParser {
 public:
  // @params: string the raw query input in string form
  // @params: PqlQuery* the Query object to write to
  PqlParser(string, PqlQuery*);

  // Parse the input using the raw query input passed in during construction
  // @returns true if parsing encountered no errors, false otherwise
  bool Parse();

  // @returns the error message (if any)
  string GetErrorMessage();

 private:
  // The input of the query
  string query_text_;
  // The Query object to store information
  PqlQuery* query_;
  // Stores error message if parsing fail at some stage
  string error_message_;

  /**
  Parses a single statement. A statement is a single declaration, or the select clause
  @string the statement in string form
  @bool true if this statement is the last
  */
  bool ParseStatement(string, bool);

  // Parses the select statement provided
  // @params: TokenList the list of tokens
  // @returns true if parse success, false otherwise
  bool ParseSelect(TokenList);

  // Parses a declaration statement provided
  // @params: TokenList the list of tokens
  // @returns true if parse success, false otherwise
  bool ParseDeclaration(TokenList);

  // Parses the such that clause of the select statement provided
  // @params: TokenList the list of tokens
  // @paramsL int* the pointer to the current index of the TokenList
  // @returns true if parse success, false otherwise
  bool ParseSuchthat(TokenList, int*);

  // Parses the such that clause of the select statement provided
  // @params: TokenList the list of tokens
  // @params: int* the pointer to the current index of the TokenList
  // @returns true if parse success, false otherwise
  bool ParsePattern(TokenList, int*);

  // Parses the assign pattern clause provided
  // @params: TokenList the list of tokens
  // @params: int* the pointer to the current index of the TokenList
  // @params: string the type name of the pattern clause
  // @returns true if parse success, false otherwise
  bool ParsePatternAssign(TokenList, int*, string);

  // Parses the while pattern clause provided
  // @params: TokenList the list of tokens
  // @params: int* the pointer to the current index of the TokenList
  // @params: string the type name of the pattern clause
  // @returns true if parse success, false otherwise
  bool ParsePatternWhile(TokenList, int*, string);

  // Parses the if pattern clause provided
  // @params: TokenList the list of tokens
  // @params: int* the pointer to the current index of the TokenList
  // @params: string the type name of the pattern clause
  // @returns true if parse success, false otherwise
  bool ParsePatternIf(TokenList, int*, string);

  // Parses the with clause of the select statement provided
  // @params: TokenList the list of tokens
  // @params: int* the pointer to the current index of the TokenList
  // @returns true if parse success, false otherwise
  bool ParseWith(TokenList, int*);

  // Parses synonym | '_' | '"' IDENT '"' | INTEGER
  // @params: TokenList the list of tokens
  // @params: int* the pointer to the current index of the TokenList
  // @params: string* the pointer to the variable that we want to store the
  // value in
  // @params: PqlDeclarationEntity* the pointer to the object we want to store
  // the type in
  // @returns true if parse success, false otherwise
  bool ParseParameter(TokenList, int*, string*, PqlDeclarationEntity*,
                      string = "");

  // Parses attribute names
  // @params: TokenList the list of tokens
  // @params: int* the pointer to the current index of the TokenList
  // @params: string* the pointer to the variable that we want to store the
  // value in
  // @params: PqlAttrName* the pointer to the object we want to store the
  // attribute type in
  // @returns true if parse success, false otherwise
  bool ParseAttribute(TokenList, int*, PqlDeclarationEntity*);
};

#endif