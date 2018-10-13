#include "stdafx.h"
#include "CppUnitTest.h"
#include "const_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestConstList) {
  const ConstValue kConstValue1 = 0;
  const ConstValue kConstValue2 = 5;
  const ConstValue kConstValue3 = 26;
  const string kConstString1 = "0";
  const string kConstString2 = "5";
  const string kConstString3 = "26";

 public:

  TEST_METHOD(TestGetAllConstValue) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    const_list.InsertConstValue(kConstValue2);
    const_list.InsertConstValue(kConstValue3);
	// duplicate
    const_list.InsertConstValue(kConstValue1);
    ConstValueList const_list_result = const_list.GetAllConstValue();
    Assert::IsTrue(const_list_result.size() == 3);
    Assert::AreEqual(kConstString1, const_list_result.front());
    ConstValueList::iterator iter = const_list_result.begin();
    iter++;
    Assert::AreEqual(kConstString2, *iter);
    Assert::AreEqual(kConstString3, const_list_result.back());
  }

  TEST_METHOD(TestGetAllConstValueTwin) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    const_list.InsertConstValue(kConstValue2);
    // duplicate
    const_list.InsertConstValue(kConstValue1);
    ConstValuePairList const_list_result = const_list.GetAllConstValueTwin();
    Assert::IsTrue(const_list_result.size() == 2);
    Assert::AreEqual(kConstString1, const_list_result.front().first);
    Assert::AreEqual(kConstString1, const_list_result.front().second);
    Assert::AreEqual(kConstString2, const_list_result.back().first);
    Assert::AreEqual(kConstString2, const_list_result.back().second);
  }
};
}  // namespace UnitTesting
