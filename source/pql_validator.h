#pragma once

#ifndef PQL_VALIDATOR_H
#define PQL_VALIDATOR_H

#include <string>
using std::string;

class PqlValidator {
public:
  static bool ValidateIdent(string);
  static bool ValidateInteger(string);
};

#endif