#pragma once

#ifndef SPA_VALIDATOR_H
#define SPA_VALIDATOR_H

#include "parser.h"

class Validator {
public:
	static void Validate(TokenList tokens);

private:
	static bool IsValidProcedure();
	static bool IsValidStmtList();
	static bool IsValidStatement();
	static bool IsValidAssignment();
	static bool IsValidExpression();
	static bool IsValidConditional();
	static bool IsValidIfBlock();
	static bool IsValidWhileBlock();
	static bool IsValidNestedBlock();
	static bool IsValidCallReadPrint();

	static TokenList ReadNextTokens(int num_tokens);
	static Token ReadNextToken();
	static Token PeekNextToken();
};

#endif // !SPA_SIMPLE_VALIDATOR_H
