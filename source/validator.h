#pragma once

#ifndef SPA_VALIDATOR_H
#define SPA_VALIDATOR_H

#include "parser.h"

using tt = Tokenizer::TokenType;

class Validator {

public:
	Validator(TokenList tokens);
	bool ValidateProgram();

	// setters
	void ResetTokenList(TokenList tokens);

	// main functions
	bool IsValidProcedure();
	bool IsValidStmtList();
	bool IsValidStatement();
	bool IsValidAssignment();
	bool IsValidExpression(TokenList expr);
	bool IsValidConditional(TokenList cond_expr);
	bool IsValidIfBlock();
	bool IsValidWhileBlock();
	bool IsValidCallReadPrint();

private:
	//static TokenList ReadNextTokens(int num_tokens);
	Token ReadNextToken();
	Token PeekNextToken();
	Token GetPreviousToken();
	
	bool MatchNext(int num, vector<tt> expected_types);
	bool Match(Tokenizer::TokenType type);
	bool Check(Tokenizer::TokenType type);
	bool IsAtEnd();

	int GetIndexOfRelational(TokenList tokens);
	bool ContainsConditionalType(TokenList tokens);
};

#endif // !SPA_SIMPLE_VALIDATOR_H
