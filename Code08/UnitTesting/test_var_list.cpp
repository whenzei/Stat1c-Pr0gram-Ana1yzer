#include "stdafx.h" 
#include "CppUnitTest.h"
#include "var_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestVarList) {
  const string kSampleVar1 = "a";
  const string kSampleVar2 = "b";

  TEST_METHOD(TestGetAllVarName) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    // duplicate
    var_list.InsertVarName(kSampleVar1);
    VarNameList result_list = var_list.GetAllVarName();
    Assert::IsTrue(result_list.size() == 2);
    Assert::AreEqual(kSampleVar1, result_list.front());
    Assert::AreEqual(kSampleVar2, result_list.back());
  }

  TEST_METHOD(TestIsVarName) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    bool result = var_list.IsVarName(kSampleVar1);
    Assert::IsTrue(result);
    result = var_list.IsVarName(kSampleVar2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetAllVarNameTwin) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    // duplicate
    var_list.InsertVarName(kSampleVar1);
    VarNamePairList result_list = var_list.GetAllVarNameTwin();
    Assert::IsTrue(result_list.size() == 2);
    Assert::AreEqual(kSampleVar1, result_list.front().first);
    Assert::AreEqual(kSampleVar1, result_list.front().second);
    Assert::AreEqual(kSampleVar2, result_list.back().first);
    Assert::AreEqual(kSampleVar2, result_list.back().second);
  }

  TEST_METHOD(TestGetIndexForVar) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    int var_index1 = var_list.GetIndexForVar(kSampleVar1);
    Assert::AreEqual(1, var_index1);
    int var_index2 = var_list.GetIndexForVar(kSampleVar2);
    Assert::AreEqual(2, var_index2);
    // Duplicate. Shouldn't affect index
    var_list.InsertVarName(kSampleVar2);
    var_index2 = var_list.GetIndexForVar(kSampleVar2);
    Assert::AreEqual(2, var_index2);
  }

  TEST_METHOD(TestGetVarForIndex) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    VarName var1 = var_list.GetVarForIndex(1);
    Assert::AreEqual(kSampleVar1, var1);
    VarName var2 = var_list.GetVarForIndex(2);
    Assert::AreEqual(kSampleVar2, var2);
    // Duplicate. Shouldn't affect var
    var_list.InsertVarName(kSampleVar2);
    var2 = var_list.GetVarForIndex(2);
    Assert::AreEqual(kSampleVar2, var2);
  }
};
}  // namespace PKBTests
