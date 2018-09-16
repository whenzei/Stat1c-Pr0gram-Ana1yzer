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
	// assign: var_name = expr ;
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
  }

  TEST_METHOD(TestIsValidIfBlock) {
	// if: if ( cond_expr ) then { stmtLst } else { stmtLst }
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
	// while: while ( cond_expr )  {  stmtLst  }
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

  // TODO
  TEST_METHOD(TestIsValidConditional) {

  }
};
}  // namespace FrontendTests
