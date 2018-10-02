#pragma once

#ifndef PQL_VALIDATOR_H
#define PQL_VALIDATOR_H

#include <string>
#include "tokenizer.h"
#include "pql_global.h"
using std::string;

class PqlValidator {
public:
  static bool ValidateIdent(string);
  static bool ValidateInteger(string);
  static bool ValidateExpression(TokenList);
  static bool ValidateAttribute(PqlDeclarationEntity, string);
};

#endif