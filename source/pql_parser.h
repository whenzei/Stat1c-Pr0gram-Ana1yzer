#ifndef PQL_PARSER_H
#define PQL_PARSER_H

#include <string>
#include "query.h"
using std::string;

class PqlParser {
 private:
  Query query;
 public:
  PqlParser(string filepath);

  Query GetQuery();
};

#endif
