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
};
}
