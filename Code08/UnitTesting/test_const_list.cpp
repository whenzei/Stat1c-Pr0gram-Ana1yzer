#include "stdafx.h"
#include "CppUnitTest.h"
#include "const_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestConstList) {
  const ConstValue kConstValue1 = 0;
  const ConstValue kConstValue2 = 5;
  const ConstValue kConstValue3 = 26;

 public:
  TEST_METHOD(TestGetAllConstValue) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    const_list.InsertConstValue(kConstValue2);
    const_list.InsertConstValue(kConstValue3);
    // duplicate
    const_list.InsertConstValue(kConstValue1);
    ConstValueSet const_list_result = const_list.GetAllConstValue();
    Assert::IsTrue(const_list_result.size() == 3);
    Assert::IsTrue(const_list_result.count(kConstValue1));
    Assert::IsTrue(const_list_result.count(kConstValue2));
    Assert::IsTrue(const_list_result.count(kConstValue3));
  }

  TEST_METHOD(TestIsConstValue) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    bool result = const_list.IsConstValue(kConstValue1);
    Assert::IsTrue(result);
    result = const_list.IsConstValue(kConstValue2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetAllConstValueTwin) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    const_list.InsertConstValue(kConstValue2);
    // duplicate
    const_list.InsertConstValue(kConstValue1);
    ConstValuePairSet const_list_result = const_list.GetAllConstValueTwin();
    Assert::IsTrue(const_list_result.size() == 2);
    Assert::IsTrue(const_list_result.count(make_pair(kConstValue1, kConstValue1)));
	Assert::IsTrue(const_list_result.count(make_pair(kConstValue2, kConstValue2)));
  }
};
}  // namespace PKBTests
