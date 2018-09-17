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
  /**
  @string the raw query input in string form
  @PqlQuery* the Query object to write to
  */
  PqlParser(string, PqlQuery*);

  /* Parse the input */
  bool Parse();
  string GetErrorMessage();

  private:
  /* The input of the query */
  string queryText_;
  /* The Query object to store information */
  PqlQuery* query_;
  /* Stores error message if parsing fail at some stage */
  string errorMessage_;

  /**
  Parses a single statement. A statement is a single declaration, or the select clause 
  @string the statement in string form
  @bool true if this statement is the last
  */
  bool ParseStatement(string, bool);
  /**
  Parses the select statement
  @TokenList the list of tokens
  */
  bool ParseSelect(TokenList);
  /**
  Parses a declaration statement 
  @TokenList the list of tokens
  */
  bool ParseDeclaration(TokenList);
  /**
  Parses the such that clause of the select statement
  @TokenList the list of tokens
  @int* the pointer to the current index of the TokenList
  */
  bool ParseSuchthat(TokenList, int*);
  /**
  Parses the such that clause of the select statement
  @TokenList the list of tokens
  @int* the pointer to the current index of the TokenList
  */
  bool ParsePattern(TokenList, int*);

  bool ParsePatternAssign(TokenList, int*);
  bool ParsePatternWhile(TokenList, int*);
  bool ParsePatternIf(TokenList, int*);
  /**
  Parses the parameters in the such that/pattern clause of the select statement
  Reads synonym | '_' | '"' IDENT '"' | INTEGER
  @TokenList the list of tokens
  @int* the pointer to the current index of the TokenList
  @string* the pointer to the variable that we want to store the value in
  @PqlDeclarationEntity* the pointer to the object we want to store the type in
  */
  bool ParseParameter(TokenList, int*, string*, PqlDeclarationEntity*);
};

#endif