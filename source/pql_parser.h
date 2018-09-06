#pragma once

#ifndef PQL_PARSER_H
#define PQL_PARSER_H

#include <string>
#include "pql_query.h"
using std::string;

class PqlParser {
  public:
  static void Parse(string, PqlQuery*);
};

#endif