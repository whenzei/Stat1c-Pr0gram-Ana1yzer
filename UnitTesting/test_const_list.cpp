#include "stdafx.h"
#include "CppUnitTest.h"
#include "const_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestConstList) {
  const ConstValue kConstValue1 = 0;
  const ConstValue kConstValue2 = 5;
  const ConstValue kConstValue3 = 26;
  const string kConstString1 = "0";
  const string kConstString2 = "5";
  const string kConstString3 = "26";

 public:
  TEST_METHOD(TestInsertSuccess) {
    ConstList const_list;
    bool result = const_list.InsertConstValue(kConstValue1);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestInsertDuplicate) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    bool result = const_list.InsertConstValue(kConstValue1);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestInsertMultiple) {
    ConstList const_list;
    bool result_1 = const_list.InsertConstValue(kConstValue1);
    bool result_2 = const_list.InsertConstValue(kConstValue2);
    bool result_3 = const_list.InsertConstValue(kConstValue3);
    Assert::IsTrue(result_1);
    Assert::IsTrue(result_2);
    Assert::IsTrue(result_3);
  }

  TEST_METHOD(TestGetConstValueList) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    ConstValueList const_list_result = const_list.GetAllConstValue();
    Assert::AreEqual(kConstString1, const_list_result.front());
  }
  TEST_METHOD(TestGetConstListMultiple) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    const_list.InsertConstValue(kConstValue2);
    const_list.InsertConstValue(kConstValue3);
    ConstValueList const_list_result = const_list.GetAllConstValue();
    Assert::AreEqual(kConstString1, const_list_result.front());
    ConstValueList::iterator iter = const_list_result.begin();
    iter++;
    Assert::AreEqual(kConstString2, *iter);
    Assert::AreEqual(kConstString3, const_list_result.back());
  }
};
}  // namespace UnitTesting
