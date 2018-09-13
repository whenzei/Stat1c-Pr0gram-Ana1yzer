#pragma once

#ifndef SPA_VALIDATOR_H
#define SPA_VALIDATOR_H

#include "parser.h"

class Validator {
public:
	static void Validate(TokenList tokens);

private:
	static bool ValidateProcedure();
	static bool ValidateExpression();
	static bool ValidateIfBlock();
	static bool ValidateWhileBlock();
	static bool ValidateNestedBlock();

	static TokenList ReadNextTokens(int num_tokens);
};

#endif // !SPA_SIMPLE_VALIDATOR_H
