#ifndef PQL_PARSER_H
#define PQL_PARSER_H

#include <string>
#include <vector>
#include "query.h"
using std::string;
using std::vector;

class PqlParser {
 private:
  Query query;

  void Parse(string);
  void ParseStatement(string);

 public:
  /* Constructor */
  PqlParser(string);

  /* Getter */
  Query GetQuery();
};

#endif
