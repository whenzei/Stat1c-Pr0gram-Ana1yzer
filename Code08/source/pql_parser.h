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
  string query_text_;
  /* The Query object to store information */
  PqlQuery* query_;
  /* Stores error message if parsing fail at some stage */
  string error_message_;

  unordered_map<string, int> synonym_group_;
  unordered_map<int, int> group_ref_;

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
  bool ParsePatternAssign(TokenList, int*, string);
  bool ParsePatternWhile(TokenList, int*, string);
  bool ParsePatternIf(TokenList, int*, string);
  /**
  Parses the with clause of the select statement
  @TokenList the list of tokens
  @int* the pointer to the current index of the TokenList
  */
  bool ParseWith(TokenList, int*);
  /**
  Parses synonym | '_' | '"' IDENT '"' | INTEGER
  @TokenList the list of tokens
  @int* the pointer to the current index of the TokenList
  @string* the pointer to the variable that we want to store the value in
  @PqlDeclarationEntity* the pointer to the object we want to store the type in
  */
  bool ParseParameter(TokenList, int*, string*, PqlDeclarationEntity*, string = "");
  /**
  Parses attribute names
  @TokenList the list of tokens
  @int* the pointer to the current index of the TokenList
  @string* the pointer to the variable that we want to store the value in
  @PqlAttrName* the pointer to the object we want to store the attribute type in
  */
  bool ParseAttribute(TokenList, int*, PqlDeclarationEntity*);

  void PreprocessGroup(PqlClause*, string, PqlDeclarationEntity, string = "", PqlDeclarationEntity = PqlDeclarationEntity::kNone);
  void GenerateGroups();
};

#endif