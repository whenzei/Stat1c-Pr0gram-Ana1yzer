#include "CppUnitTest.h"
#include "stdafx.h"
#include "tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestTokenizer) {
public:

TEST_METHOD(TestEmptyResult) {
  Result expected_result({0, {Tokenizer::TokenType::kNothing}});
  Assert::IsTrue(expected_result == Tokenizer::EmptyResult());
}
TEST_METHOD(TestTokenizeCharacter) {
  string kTestInput("hello world");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kName);

  // check tokenize matching character 'h' at index 0
  Result expected_result({1, {kTestType, "h"}});
  Result actual_result =
      Tokenizer::TokenizeCharacter(kTestType, 'h', kTestInput, 0);
  Assert::IsTrue(actual_result == expected_result);

  // check tokenize matching character 'o' at index 4
  expected_result = Result({1, {kTestType, "o"}});
  actual_result = Tokenizer::TokenizeCharacter(kTestType, 'o', kTestInput, 4);
  Assert::IsTrue(actual_result == expected_result);

  // check no tokenizing of non matching character 'z' at index 4
  actual_result = Tokenizer::TokenizeCharacter(kTestType, 'z', kTestInput, 4);
  Assert::IsFalse(actual_result == expected_result);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());
}

TEST_METHOD(TestTokenizePattern) {
  string kTestInput("hello world");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kName);

  // check tokenize names (basically same call as TokenizeNames(...) starting at
  // index 0
  Result expected_result({5, {kTestType, "hello"}});
  Result actual_result = Tokenizer::TokenizePattern(
      kTestType, regex{R"([a-zA-Z][a-zA-Z0-9]*)"}, kTestInput, 0);
  Assert::IsTrue(actual_result == expected_result);

  // check tokenize names starting at index 2
  expected_result = Result({3, {kTestType, "llo"}});
  actual_result = Tokenizer::TokenizePattern(
      kTestType, regex{R"([a-zA-Z][a-zA-Z0-9]*)"}, kTestInput, 2);
  Assert::IsTrue(actual_result == expected_result);

  // check no tokenizing of digits (using digit regex)
  actual_result =
      Tokenizer::TokenizePattern(kTestType, regex{R"([0-9]+)"}, kTestInput, 2);
  Assert::IsFalse(actual_result == expected_result);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());
}

TEST_METHOD(TestSkipWhitespace) {
  string kTestInput("hello world");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kNothing);

  // check first character in input is not whitespace
  Result actual_result = Tokenizer::SkipWhitespace(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // check 5th character in input is whitespace
  actual_result = Tokenizer::SkipWhitespace(kTestInput, 5);
  Result expected_result({1, {kTestType}});
  Assert::IsTrue(actual_result == expected_result);
}

TEST_METHOD(TestSkipComments) {
  string kTestInput("something");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kNothing);

  // check input is not a comment, should return empty result
  Result actual_result = Tokenizer::SkipComments(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // if input is a comment, should return result with correct numbers of
  // consumed characters
  kTestInput = "\\comment until newline\n this should not be counted";
  actual_result = Tokenizer::SkipComments(kTestInput, 0);
  // skip 22 more characters until new line character
  Result expected_result({22, {kTestType, "\\comment until newline"}});
  Assert::IsTrue(actual_result == expected_result);
}

TEST_METHOD(TestTokenizeDigit) {
  string kTestInput("no digits until: 6695abc");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kDigit);

  // should not tokenize this at all
  Result actual_result = Tokenizer::TokenizeDigits(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // change index to start at [6]695, should ignore the abc
  actual_result = Tokenizer::TokenizeDigits(kTestInput, 17);
  Result expected_result({4, {kTestType, "6695"}});
  Assert::IsTrue(actual_result == expected_result);
}

TEST_METHOD(TestTokenizeNames) {
  string kTestInput("nonumbers");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kName);

  // should tokenize first part, without whitespace and semicolon
  Result actual_result = Tokenizer::TokenizeNames(kTestInput, 0);
  Result expected_result({9, {kTestType, "nonumbers"}});
  Assert::IsTrue(actual_result == expected_result);

  // should tokenize mixture of characters and numbers,
  // provided it begins with a character
  kTestInput = "mix3dl3tt3rs";
  actual_result = Tokenizer::TokenizeNames(kTestInput, 0);
  expected_result = Result({12, {kTestType, "mix3dl3tt3rs"}});
  Assert::IsTrue(actual_result == expected_result);

  // should return empty result if given something that starts with a digit
  kTestInput = "3startswithdigit";
  actual_result = Tokenizer::TokenizeNames(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // keywords are detected
  kTestInput = "procedure";
  kTestType = Tokenizer::TokenType::kKeyword;
  actual_result = Tokenizer::TokenizeNames(kTestInput, 0);
  expected_result = Result({ 9, {kTestType, "procedure"} });
  Assert::IsTrue(actual_result == expected_result);

}

TEST_METHOD(TestTokenizeBraces) {
  string kTestInput("{}}m}}");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kBrace);

  // should only detect the first brace and stop
  Result actual_result = Tokenizer::TokenizeBraces(kTestInput, 0);
  Result expected_result({1, {kTestType, "{"}});
  Assert::IsTrue(actual_result == expected_result);

  // should detect closing brace too
  actual_result = Tokenizer::TokenizeBraces(kTestInput, 1);
  expected_result = Result({1, {kTestType, "}"}});
  Assert::IsTrue(actual_result == expected_result);

  // returns empty result for non braces (testing only characters)
  actual_result = Tokenizer::TokenizeBraces(kTestInput, 3);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());
}

TEST_METHOD(TestTokenizeBrackets) {
  string kTestInput("())m))");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kBracket);

  // should only detect the first bracket and stop
  Result actual_result = Tokenizer::TokenizeBrackets(kTestInput, 0);
  Result expected_result({1, {kTestType, "("}});
  Assert::IsTrue(actual_result == expected_result);

  // should detect closing bracket too
  actual_result = Tokenizer::TokenizeBrackets(kTestInput, 1);
  expected_result = Result({1, {kTestType, ")"}});
  Assert::IsTrue(actual_result == expected_result);

  // returns empty result for non brackets (testing only characters)
  actual_result = Tokenizer::TokenizeBrackets(kTestInput, 3);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());
}

TEST_METHOD(TestTokenizeRelationals) {
  string kTestInput("a >=== b");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kRelational);

  // returns empty result for non rel_expr (testing only characters)
  Result actual_result = Tokenizer::TokenizeRelationals(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // should detect the ">=" relatonal
  actual_result = Tokenizer::TokenizeRelationals(kTestInput, 2);
  Result expected_result({2, {kTestType, ">="}});
  Assert::IsTrue(actual_result == expected_result);

  // should detect the "<=" relational
  kTestInput = "a <=== b";
  actual_result = Tokenizer::TokenizeRelationals(kTestInput, 2);
  expected_result = Result({2, {kTestType, "<="}});
  Assert::IsTrue(actual_result == expected_result);

  // should detect just a single "<" relational character
  kTestInput = "a <<< b";
  actual_result = Tokenizer::TokenizeRelationals(kTestInput, 2);
  expected_result = Result({1, {kTestType, "<"}});
  Assert::IsTrue(actual_result == expected_result);

  // should detect just a single ">" relational character
  kTestInput = "a >>> b";
  actual_result = Tokenizer::TokenizeRelationals(kTestInput, 2);
  expected_result = Result({1, {kTestType, ">"}});
  Assert::IsTrue(actual_result == expected_result);

  // should detect just the "!=" relational
  kTestInput = "a !== b";
  actual_result = Tokenizer::TokenizeRelationals(kTestInput, 2);
  expected_result = Result({2, {kTestType, "!="}});
  Assert::IsTrue(actual_result == expected_result);

  // should detect a "==" relational (even if TokenizeEquals already does it)
  kTestInput = "a === b";
  actual_result = Tokenizer::TokenizeRelationals(kTestInput, 2);
  expected_result = Result({2, {kTestType, "=="}});
  Assert::IsTrue(actual_result == expected_result);
}

TEST_METHOD(TestTokenizeOperators) {
  string kTestInput("a +- b");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kOperator);

  // should return empty result if given a name
  Result actual_result = Tokenizer::TokenizeOperators(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // tokenizes a single operator at a time
  actual_result = Tokenizer::TokenizeOperators(kTestInput, 2);
  Result expected_result({1, {kTestType, "+"}});
  Assert::IsTrue(actual_result == expected_result);
}

TEST_METHOD(TestTokenizeSemicolon) {
  string kTestInput("a;; b;");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kSemicolon);

  // should return empty result if given a name
  Result actual_result = Tokenizer::TokenizeSemicolon(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // tokenizes a semicolon at a time
  actual_result = Tokenizer::TokenizeSemicolon(kTestInput, 1);
  Result expected_result({1, {kTestType, ";"}});
  Assert::IsTrue(actual_result == expected_result);
}

TEST_METHOD(TestTokenizeEquals) {
  string kTestInput("a===b=c;");
  Tokenizer::TokenType kTestType(Tokenizer::TokenType::kConditional);

  // should return empty result if given a name
  Result actual_result = Tokenizer::TokenizeEquals(kTestInput, 0);
  Assert::IsTrue(actual_result == Tokenizer::EmptyResult());

  // tokenizes an conditional, and at most 2 equals signs - a[==]=b=c
  actual_result = Tokenizer::TokenizeEquals(kTestInput, 1);
  Result expected_result({2, {kTestType, "=="}});
  Assert::IsTrue(actual_result == expected_result);

  // tokenizes an assignment - a===b[=]c
  kTestType = Tokenizer::TokenType::kAssignment;
  actual_result = Tokenizer::TokenizeEquals(kTestInput, 5);
  expected_result = Result({1, {kTestType, "="}});
  Assert::IsTrue(actual_result == expected_result);
}

TEST_METHOD(TestTokenize) {
  string kTestInput(
      "procedure one {\na= b;\n c  = d; \\comments here\n }"
      "procedure two { if(a!=b) then { d \t\n= e;}else { e =\t "
      "\n	 f;}}");

  // to make expected_tokens easier to read, abbreviate to tt;
  using tt = Tokenizer::TokenType;

  TokenList actual_tokens = Tokenizer::Tokenize(kTestInput);
  Assert::IsTrue(actual_tokens.size() == 36);
  TokenList expected_tokens({{tt::kKeyword, "procedure"},
                             {tt::kName, "one"},
                             {tt::kBrace, "{"},
                             {tt::kName, "a"},
                             {tt::kAssignment, "="},
                             {tt::kName, "b"},
                             {tt::kSemicolon, ";"},
                             {tt::kName, "c"},
                             {tt::kAssignment, "="},
                             {tt::kName, "d"},
                             {tt::kSemicolon, ";"},
                             {tt::kBrace, "}"},
                             {tt::kKeyword, "procedure"},
                             {tt::kName, "two"},
                             {tt::kBrace, "{"},
                             {tt::kKeyword, "if"},
                             {tt::kBracket, "("},
                             {tt::kName, "a"},
                             {tt::kRelational, "!="},
                             {tt::kName, "b"},
                             {tt::kBracket, ")"},
                             {tt::kKeyword, "then"},
                             {tt::kBrace, "{"},
                             {tt::kName, "d"},
                             {tt::kAssignment, "="},
                             {tt::kName, "e"},
                             {tt::kSemicolon, ";"},
                             {tt::kBrace, "}"},
                             {tt::kKeyword, "else"},
                             {tt::kBrace, "{"},
                             {tt::kName, "e"},
                             {tt::kAssignment, "="},
                             {tt::kName, "f"},
                             {tt::kSemicolon, ";"},
                             {tt::kBrace, "}"},
                             {tt::kBrace, "}"}});

  Assert::IsTrue(actual_tokens == expected_tokens);
}
}
;
}  // namespace UnitTesting