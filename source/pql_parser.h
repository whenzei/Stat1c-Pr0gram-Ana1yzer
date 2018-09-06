#pragma once

#ifndef PQL_PARSER_H
#define PQL_PARSER_H

#include <string>
#include <vector>
#include "pql_query.h"
using std::string;
using std::vector;

class PqlParser {
  public:
  static void Parse(string, PqlQuery*);

  private:
  static void ParseStatement(string, PqlQuery*, bool);
  static void ParseSynonym(string, PqlQuery*);
  static void ParseSelect(vector<string>, PqlQuery*);
  static void ParseDeclaration(vector<string>, PqlQuery*);
};

#endif