#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "simple_validator.h"
#include "tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace UnitTesting {

TEST_CLASS(TestSimpleValidator) {
 public:
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