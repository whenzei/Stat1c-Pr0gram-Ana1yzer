#include "stdafx.h"
#include "CppUnitTest.h"
#include "validator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using tt = Tokenizer::TokenType;
using ts = Tokenizer::TokenSubtype;

namespace FrontendTests {
TEST_CLASS(TestValidator) {
public:
  // This test only accounts for positive test cases, the negative tests
  // will be tested in the more specific functions like IsValidStatement
  TEST_METHOD(TestValidateProgram) {
	// can validate a suitably sophisicated program with a single procedure
	TokenList tokens = Tokenizer::Tokenize(
	  "procedure One{\nif (a == b) then {\nif = while;\n} else {\nif "
	  "(while == if) then {\ne = if;\ng = h;\n} else { while = b;}b = c;}g "
	  "= f;\nwhile (a < b) {then = a + 1;\nif (s == t) then {s = s + 1;} "
	  "else {\ncall = t + 1;\n}b = b + 2;\n}s = (f / e) * e / (d - e) + "
	  "1;}\n");
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.ValidateProgram());

	// can validate multiple procedures
	tokens = Tokenizer::Tokenize("procedure one {a = b;\n b=c;}\n procedure two {f = g;\n}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.ValidateProgram());
  }

  TEST_METHOD(TestIsValidProcedure) {
	// procedure: "procedure" var_name "{" stmtList "}"
	// pass with correct syntax
	TokenList tokens = Tokenizer::Tokenize("procedure one { a = b;\n b=c;}");
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidProcedure());

	// fail if procedure does not start with correct keyword
	tokens = Tokenizer::Tokenize("notprocedure 2345 {a = b;\n b=c;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidProcedure());

	// fail if procedure name is not a var_name
	tokens = Tokenizer::Tokenize("procedure 2345 {a = b;\n b=c;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidProcedure());

	// fail if empty procedure
	tokens = Tokenizer::Tokenize("procedure 2345 {}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidProcedure());

	// fail if procedure does not have opening brace
	tokens = Tokenizer::Tokenize("procedure 2345 a = b;\n b=c;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidProcedure());

	// fail if procedure does not have closing brace
	tokens = Tokenizer::Tokenize("procedure 2345 {a = b;\n b=c;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidProcedure());
  }

  TEST_METHOD(TestIsValidStmtList) {
	// stmtList: stmt+
	// pass with single statement
	TokenList tokens = Tokenizer::Tokenize("a = b;");
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidStmtList());

	// pass with multiple statements
	tokens = Tokenizer::Tokenize("a = b;\n b=c;");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidStmtList());
	
	// fail with no statements
	tokens = Tokenizer::Tokenize("");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidStmtList());
  }

  TEST_METHOD(TestIsValidStatement) {
	// stmt: read | print | call | while | if | assign
	// pass with standard assignment (detailed test in TestIsValidAssignment)
	TokenList tokens = Tokenizer::Tokenize("a = b;");
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidStatement());

	// pass even with variables that are keywords
	tokens = Tokenizer::Tokenize("if = while;");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidStatement());

	// passes correct call (and read and print, assumed) statements
	tokens = Tokenizer::Tokenize("call one;");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidStatement());

	// passes correct while statement (will check fail cases in TestIsValidWhileBlock)
	tokens = Tokenizer::Tokenize("while(a > b) { a = a - 1; }");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidStatement());

	// fails if not valid assignment statement
	tokens = Tokenizer::Tokenize("a += 1;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidStatement());

	// fails if begins with digit
	tokens = Tokenizer::Tokenize("34 = a + b;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidStatement());

	// fails if not valid statement
	tokens = Tokenizer::Tokenize("notcall b;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidStatement());
  }

  TEST_METHOD(TestIsValidAssignment) {
	// assign: var_name ‘=’ expr ‘;’
	// pass complex assignment (simple is already checked)
	TokenList tokens = Tokenizer::Tokenize("= a + ( b / c ) * d / e;");
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidAssignment());

	// fails if assignment has repeating operators between variables
	tokens = Tokenizer::Tokenize("= a -- b;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidAssignment());

	// fails if assignment has two variables in a row
	tokens = Tokenizer::Tokenize("= a b -;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidAssignment());

	// fails if missing semicolon
	tokens = Tokenizer::Tokenize("= a - b");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidAssignment());

	// fails if missing equals sign
	tokens = Tokenizer::Tokenize("a - b;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidAssignment());

	// fails if missing operator
	tokens = Tokenizer::Tokenize("= a b;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidAssignment());
  }

  TEST_METHOD(TestIsValidIfBlock) {
	// if: ‘if’ ‘(’ cond_expr ‘)’ ‘then’ ‘{‘ stmtLst ‘}’ ‘else’ ‘{‘ stmtLst ‘}’
	// passes standard ifthen...else block
	TokenList tokens = Tokenizer::Tokenize("(a > b) then { a = b;} else { c = d;}");
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidIfBlock());

	// passes if multiple statements
	tokens = Tokenizer::Tokenize("(a > b) then {a = b; b = c;} else {c = d; d = e;}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidIfBlock());

	// passes even with nested while block
	tokens = Tokenizer::Tokenize("(a > b) then {a = b; while(d > e) { d=e;}} else {c = d;}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidIfBlock());

	tokens = Tokenizer::Tokenize("(a > b) then {while(d > e) { d=e;} a = b; } else {c = d;}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidIfBlock());

	// passes even with nested ifelse block
	tokens = Tokenizer::Tokenize("(a > b) then {a = b;} else {c = d; if(d > e) then {d = e;} else { e=f;}}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidIfBlock());

	// fails if else block is empty
	tokens = Tokenizer::Tokenize("(a > b) then { a = b;} else {}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());

	// fails if if block is empty
	tokens = Tokenizer::Tokenize("(a > b) then {} else { a = b;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());

	// fails if if block has invalid stmt
	tokens = Tokenizer::Tokenize("(a > b) then { 234 = a + b; } else { a = b;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());

	// fails if missing else block
	tokens = Tokenizer::Tokenize("(a > b) then { a = b;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());

	// fails if missing then keyword
	tokens = Tokenizer::Tokenize("(a > b) { a = b;} else { c = d;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());

	// fails if condition is missing
	// i.e. " if then { a = b; } else { c = d;}
	tokens = Tokenizer::Tokenize("then { a = b;} else { c = d;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());

	// fails if condition is empty
	tokens = Tokenizer::Tokenize("() then { a = b;} else { c = d;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());

	// fails if closing brace is missing
	tokens = Tokenizer::Tokenize("(a > b) then { a = b;} else { c = d;");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());
  }

  TEST_METHOD(TestIsValidWhileBlock) {
	// while: ‘while’ ‘(’ cond_expr ‘)’ ‘ { ‘ stmtLst ‘ }’
	// passes standard while block
	TokenList tokens = Tokenizer::Tokenize("(a > b) { a = b; }");
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	// passes if multiple statements
	tokens = Tokenizer::Tokenize("(a > b) {a = b; b = c;}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	// passes even with nested while block
	tokens = Tokenizer::Tokenize("(a > b) {a = b; while (c > d) {c = d;}}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	tokens = Tokenizer::Tokenize("(a > b) {while (c > d) {c = d;} a = b;}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	tokens = Tokenizer::Tokenize("(a > b) {while (c > d) {c = d;}}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	// passes even with nested if..else block
	tokens = Tokenizer::Tokenize("(a > b) {a = b; if(c > d) then { c = d;} else {d = c;}}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	tokens = Tokenizer::Tokenize("(a > b) {if(c > d) then { c = d;} else {d = c;} a = b; }");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	tokens = Tokenizer::Tokenize("(a > b) {if(c > d) then { c = d;} else {d = c;}}");
	validator.ResetTokenList(tokens);
	Assert::IsTrue(validator.IsValidWhileBlock());

	// fails if stmtList is empty
	tokens = Tokenizer::Tokenize("(a > b) {}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidWhileBlock());

	// fails if condition is empty
	tokens = Tokenizer::Tokenize("() {a = b;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidWhileBlock());

	// fails if condition is missing
	tokens = Tokenizer::Tokenize("{a = b;}");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidWhileBlock());

	// fails if while block has invalid stmt
	tokens = Tokenizer::Tokenize("(a > b) { 234 = a + b; }");
	validator.ResetTokenList(tokens);
	Assert::IsFalse(validator.IsValidIfBlock());
  }

  // Since this is a method that takes in a TokenList, if TokenList is created
  // directly it will generate a kEOF token at the end which will cause it to fail,
  // thus remove the final token before passing it into the function
  TEST_METHOD(TestIsValidConditional) {
	// cond_expr: rel_expr | ‘!’ ‘(’ cond_expr ‘)’ | ‘(’ cond_expr ‘)’ ‘&&’ ‘(’
    // cond_expr ‘)’ | ‘(’ cond_expr ‘)’ ‘||’ ‘(’ cond_expr ‘)’

	// passes standard cond_expr that is just a rel_expr
	TokenList tokens = Tokenizer::Tokenize("a > b");
	tokens.pop_back();
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidConditional(tokens));

	// passes complex cond_expr
	tokens = Tokenizer::Tokenize("((a != b) && (b > c)) || (d == e)");
	tokens.pop_back();
	Assert::IsTrue(validator.IsValidConditional(tokens));

	// passes negation of conditional
	tokens = Tokenizer::Tokenize("!(a > b)");
	tokens.pop_back();
	Assert::IsTrue(validator.IsValidConditional(tokens));

	// passes nested negation of conditional
	tokens = Tokenizer::Tokenize("!(!(a > b))");
	tokens.pop_back();
	Assert::IsTrue(validator.IsValidConditional(tokens));

	// fails nested negation of conditional if inner cond_expr is invalid
	tokens = Tokenizer::Tokenize("!(!(a = b))");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidConditional(tokens));

	// fails cond_expr with more than 1 conditional on the same level
	tokens = Tokenizer::Tokenize("(a>b) && (b > c) || (d > e)");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidConditional(tokens));

	// fails cond_expr if conditional is not surrounded by parenthesis
	tokens = Tokenizer::Tokenize("a>b && b > c");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidConditional(tokens));

	// fails cond_expr if parenthesis are in wrong locations
	tokens = Tokenizer::Tokenize("(a>b && )(b > c)");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidConditional(tokens));

	// fails cond_expr if lhs and rhs of a statement with a conditional is invalid
	tokens = Tokenizer::Tokenize("(a) || (b)");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidConditional(tokens));
  }

  // Since this is a method that takes in a TokenList, if TokenList is created
  // directly it will generate a kEOF token at the end which will cause it to fail,
  // thus remove the final token before passing it into the function
  TEST_METHOD(TestIsValidExpression) {
    // expr: expr ‘ + ’ term | expr ‘ - ’ term | term
	// term : term ‘*’ factor | term ‘ / ’ factor | term ‘%’ factor | factor
	// factor : var_name | const_value | ‘(’ expr ‘)’

	// passes standard expr
	TokenList tokens = Tokenizer::Tokenize("a + b - 1");
	tokens.pop_back();
	Validator validator = Validator(tokens);
	Assert::IsTrue(validator.IsValidExpression(tokens));

	// passes single variable
	tokens = Tokenizer::Tokenize("a");
	tokens.pop_back();
	Assert::IsTrue(validator.IsValidExpression(tokens));

	// passes single digit
	tokens = Tokenizer::Tokenize("123");
	tokens.pop_back();
	Assert::IsTrue(validator.IsValidExpression(tokens));

	// passes complex expression with parenthesis
	tokens = Tokenizer::Tokenize("(a + b / (c - d) * (e * f))");
	tokens.pop_back();
	Assert::IsTrue(validator.IsValidExpression(tokens));

	// fails imbalanced parenthesis
	tokens = Tokenizer::Tokenize("a + (b - (c * d)");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidExpression(tokens));

	// fails invalid parenthesis placement
	tokens = Tokenizer::Tokenize("a ( + b - (c * d)");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidExpression(tokens));
	
	// fails invalid var_name
	tokens = Tokenizer::Tokenize("355d + c");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidExpression(tokens));

	// fails invalid operator placement
	tokens = Tokenizer::Tokenize("a b +");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidExpression(tokens));

	// fails invalid expression
	tokens = Tokenizer::Tokenize("a > b");
	tokens.pop_back();
	Assert::IsFalse(validator.IsValidExpression(tokens));
  }
};
}  // namespace FrontendTests
