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

  TEST_METHOD(TestGetIndexToVarMapping) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    IndexVarMap index_var_map = var_list.GetIndexToVarMapping();
    Assert::AreEqual(kSampleVar1, index_var_map[1]);
    index_var_map = var_list.GetIndexToVarMapping();
    Assert::AreEqual(kSampleVar2, index_var_map[2]);
    // Duplicate. Shouldn't affect index
    var_list.InsertVarName(kSampleVar2);
    index_var_map = var_list.GetIndexToVarMapping();
    Assert::AreEqual(kSampleVar2, index_var_map[2]);
  }

  TEST_METHOD(TestGetVarToIndexMapping) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    VarIndexMap var_index_map = var_list.GetVarToIndexMapping();
    Assert::AreEqual(1, var_index_map[kSampleVar1]);
    VarIndexMap var_index_map = var_list.GetVarToIndexMapping();
    Assert::AreEqual(2, var_index_map[kSampleVar2]);
    // Duplicate. Shouldn't affect var
    var_list.InsertVarName(kSampleVar2);
    var_index_map = var_list.GetVarToIndexMapping();
    Assert::AreEqual(2, var_index_map[kSampleVar2]);
  }
};
}  // namespace PKBTests
