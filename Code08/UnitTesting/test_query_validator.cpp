#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_validator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace PQLTests {
TEST_CLASS(TestQueryValidator) {
public:
  TEST_METHOD(TestValidIdent) {
    string content = "abc123";
    bool result = PqlValidator::ValidateIdent(content);
    Assert::IsTrue(result);

    content = "abcde";
    result = PqlValidator::ValidateIdent(content);
    Assert::IsTrue(result);

    content = "a1b2c3";
    result = PqlValidator::ValidateIdent(content);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestInvalidIdent) {
    string content = "abc$123";
    bool result = PqlValidator::ValidateIdent(content);
    Assert::IsTrue(!result);

    content = "%abc";
    result = PqlValidator::ValidateIdent(content);
    Assert::IsTrue(!result);

    content = "123abc";
    result = PqlValidator::ValidateIdent(content);
    Assert::IsTrue(!result);
  }

  TEST_METHOD(TestValidInteger) {
    string content = "1";
    bool result = PqlValidator::ValidateInteger(content);
    Assert::IsTrue(result);

    content = "12";
    result = PqlValidator::ValidateInteger(content);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestInvalidInteger) {
    string content = "a";
    bool result = PqlValidator::ValidateInteger(content);
    Assert::IsTrue(!result);

    content = "a1";
    result = PqlValidator::ValidateInteger(content);
    Assert::IsTrue(!result);

    content = "1a";
    result = PqlValidator::ValidateInteger(content);
    Assert::IsTrue(!result);
  }
  };
}