#pragma once

#ifndef SPA_VALIDATOR_H
#define SPA_VALIDATOR_H

#include "parser.h"

class Validator {
public:
	static void Validate(TokenList tokens);

private:
	static bool IsValidProcedure();
	static bool IsValidStatement();
	static bool IsValidExpression();
	static bool IsValidIfBlock();
	static bool IsValidWhileBlock();
	static bool IsValidNestedBlock();

	static TokenList ReadNextTokens(int num_tokens);
};

#endif // !SPA_SIMPLE_VALIDATOR_H
