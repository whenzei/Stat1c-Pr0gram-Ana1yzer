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
    bool status = const_list.InsertConstValue(kConstValue1);
    Assert::AreEqual(true, status);
  }

  TEST_METHOD(TestInsertDuplicate) {
    ConstList const_list;
    const_list.InsertConstValue(kConstValue1);
    bool status = const_list.InsertConstValue(kConstValue1);
    Assert::AreEqual(false, status);
  }

  TEST_METHOD(TestInsertMultiple) {
    ConstList const_list;
    bool status_1 = const_list.InsertConstValue(kConstValue1);
    bool status_2 = const_list.InsertConstValue(kConstValue2);
    bool status_3 = const_list.InsertConstValue(kConstValue3);
    Assert::AreEqual(true, status_1);
    Assert::AreEqual(true, status_2);
    Assert::AreEqual(true, status_3);
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
