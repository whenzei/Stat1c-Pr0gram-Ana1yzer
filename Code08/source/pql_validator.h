#pragma once

#ifndef PQL_VALIDATOR_H
#define PQL_VALIDATOR_H

#include <string>
#include "pql_global.h"
#include "tokenizer.h"
using std::string;

class PqlValidator {
 public:
  // Validates if the given string is a valid IDENT
  // @params: string the string to validate
  // @returns true if the given string is a valid IDENT, false otherwise
  static bool ValidateIdent(string);

  // Validates if the given string is a valid INTEGER
  // @params: string the string to validate
  // @returns true if the given string is a valid INTEGER, false otherwise
  static bool ValidateInteger(string);

  // Validates if the given TokenList is a valid expression
  // @params: TokenList the tokens to validate
  // @returns true if the given TokenList is a valid expression, false otherwise
  static bool ValidateExpression(TokenList);

  // Validates if the given string can have the given PqlDeclarationEntity type
  // @params: PqlDeclarationEntity the entity type to be checked
  // @params: string the string to validate
  // @returns true if the given string can have the given PqlDeclarationEntity
  // type, false otherwise
  static bool ValidateAttribute(PqlDeclarationEntity, string);
};

#endif