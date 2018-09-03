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
    bool result_procedure = SimpleValidator::IsKeyword(kWordProcedure);
    Assert::AreEqual(true, result_procedure);

    bool result_if = SimpleValidator::IsKeyword(kWordIf);
    Assert::AreEqual(true, result_if);

    bool result_else = SimpleValidator::IsKeyword(kWordElse);
    Assert::AreEqual(true, result_else);

    bool result_while = SimpleValidator::IsKeyword(kWordWhile);
    Assert::AreEqual(true, result_while);

    bool result_read = SimpleValidator::IsKeyword(kWordRead);
    Assert::AreEqual(true, result_read);

    bool result_print = SimpleValidator::IsKeyword(kWordPrint);
    Assert::AreEqual(true, result_print);

    bool result_call = SimpleValidator::IsKeyword(kWordCall);
    Assert::AreEqual(true, result_call);
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
    string valid_procedure_string = "procedure test {\n a = b;\n b = c;\n c = d;\n}";
    TokenList valid_procedure_tokens = Tokenizer::Tokenize(valid_procedure_string);
    bool result = SimpleValidator::ValidateProcedure(
        valid_procedure_tokens, 0, valid_procedure_tokens.size() - 1);
    Assert::AreEqual(true, result);
  }

  TEST_METHOD(TestValidateProcedureFail) {
    string invalid_procedure_string_one = "procedure One {\n a = b\n c = d;\n}";
    TokenList invalid_procedure_tokens_one =
        Tokenizer::Tokenize(invalid_procedure_string_one);
    bool result_one = SimpleValidator::ValidateProcedure(
        invalid_procedure_tokens_one, 0, invalid_procedure_tokens_one.size() - 1);
    Assert::AreEqual(false, result_one);

				
    string invalid_procedure_string_two = "procedure bad {\n a = b;\n b = 3\n }";
    TokenList invalid_procedure_tokens_two =
        Tokenizer::Tokenize(invalid_procedure_string_two);
    bool resultTwo = SimpleValidator::ValidateProcedure(
        invalid_procedure_tokens_two, 0, invalid_procedure_tokens_two.size() - 1);
    Assert::AreEqual(false, resultTwo);

    string invalid_procedure_string_three = "procedure bad {\n 3 = b;\n}";
    TokenList invalid_procedure_tokens_three =
        Tokenizer::Tokenize(invalid_procedure_string_one);
    bool resultThree = SimpleValidator::ValidateProcedure(
        invalid_procedure_tokens_three, 0, invalid_procedure_tokens_three.size() - 1);
    Assert::AreEqual(false, resultThree);
  }
};
}  // namespace UnitTesting