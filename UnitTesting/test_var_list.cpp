#include "stdafx.h"
#include "CppUnitTest.h"
#include "var_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestVarList) {
  const string kSampleVar1 = "a";
  const string kSampleVar2 = "b";

  TEST_METHOD(TestInsertSingleVar) {
    VarList var_list;
    bool result = var_list.InsertVarName(kSampleVar1);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestInsertDuplicate) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    bool result = var_list.InsertVarName(kSampleVar1);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetAllVarSingle) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    VarNameList result_list = var_list.GetAllVarName();
    Assert::AreEqual(kSampleVar1, result_list.front());
  }

  TEST_METHOD(TestGetAllVarMultiple) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    VarNameList result_list = var_list.GetAllVarName();
    Assert::AreEqual(kSampleVar1, result_list.front());
    Assert::AreEqual(kSampleVar2, result_list.back());
  }
};
}
