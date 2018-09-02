#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "simple_validator.h"
#include "tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace UnitTesting {

TEST_CLASS(TestSimpleValidator) {
  const string kWordOne = "one";
  const string kWordProcedure = "procedure";
  const string kWordIf = "if";
  const string kWordElse = "else";
  const string kWordWhile = "while";
  const string kWordCall = "call";
  const string kWordRead = "read";
  const string kWordPrint = "print";

 public:
  TEST_METHOD(TestIsKeywordOneParamTrue) {
    bool resultProcedure = SimpleValidator::IsKeyword(kWordProcedure);
    Assert::AreEqual(true, resultProcedure);

    bool resultIf = SimpleValidator::IsKeyword(kWordIf);
    Assert::AreEqual(true, resultIf);

    bool resultElse = SimpleValidator::IsKeyword(kWordElse);
    Assert::AreEqual(true, resultElse);

    bool resultWhile = SimpleValidator::IsKeyword(kWordWhile);
    Assert::AreEqual(true, resultWhile);

    bool resultRead = SimpleValidator::IsKeyword(kWordRead);
    Assert::AreEqual(true, resultRead);

    bool resultPrint = SimpleValidator::IsKeyword(kWordPrint);
    Assert::AreEqual(true, resultPrint);

    bool resultCall = SimpleValidator::IsKeyword(kWordCall);
    Assert::AreEqual(true, resultCall);
  }

  TEST_METHOD(TestIsKeywordProcedureOneParamFalse) {
    bool result = SimpleValidator::IsKeyword(kWordOne);
    Assert::AreEqual(false, result);
  }

  TEST_METHOD(TestIsKeywordProcedureTwoParamsTrue) {
    bool result = SimpleValidator::IsKeyword(
        SimpleValidator::KeywordType::kProcedure, kWordProcedure);
    Assert::AreEqual(true, result);
  }

  TEST_METHOD(TestIsKeywordProcedureTwoParamsFalse) {
    bool result = SimpleValidator::IsKeyword(
        SimpleValidator::KeywordType::kProcedure, kWordOne);
    Assert::AreEqual(false, result);
  }

  TEST_METHOD(TestValidateProcedureSuccess) {
    string validProcedureString = "procedure test {\n a = b;\n b = c;\n c = d;\n}";
    TokenList validProcedureTokens = Tokenizer::Tokenize(validProcedureString);
    bool result = SimpleValidator::ValidateProcedure(
        validProcedureTokens, 0, validProcedureTokens.size() - 1);
    Assert::AreEqual(true, result);
  }

  TEST_METHOD(TestValidateProcedureFail) {
    string invalidProcedureStringOne = "procedure One {\n a = b\n c = d;\n}";
    TokenList invalidProcedureTokensOne =
        Tokenizer::Tokenize(invalidProcedureStringOne);
    bool resultOne = SimpleValidator::ValidateProcedure(
        invalidProcedureTokensOne, 0, invalidProcedureTokensOne.size() - 1);
    Assert::AreEqual(false, resultOne);

				
    string invalidProcedureStringTwo = "procedure bad {\n a = b;\n b = 3\n }";
    TokenList invalidProcedureTokensTwo =
        Tokenizer::Tokenize(invalidProcedureStringTwo);
    bool resultTwo = SimpleValidator::ValidateProcedure(
        invalidProcedureTokensTwo, 0, invalidProcedureTokensTwo.size() - 1);
    Assert::AreEqual(false, resultTwo);

    string invalidProcedureStringThree = "procedure bad {\n 3 = b;\n}";
    TokenList invalidProcedureTokensThree =
        Tokenizer::Tokenize(invalidProcedureStringOne);
    bool resultThree = SimpleValidator::ValidateProcedure(
        invalidProcedureTokensThree, 0, invalidProcedureTokensThree.size() - 1);
    Assert::AreEqual(false, resultThree);
  }
};
}  // namespace UnitTesting