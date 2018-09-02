#include "CppUnitTest.h"
#include "stdafx.h"
#include "var_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestVarTable) {
  const int kSuccess = 0;
  const int kFailure = -1;
  const string kSampleVar1 = "a";
  const string kSampleVar2 = "b";

  TEST_METHOD(TestInsertSingleVar) {
    VarTable var_table;
    int result = var_table.InsertVar(kSampleVar1);
    Assert::AreEqual(kSuccess, result);
  }

  TEST_METHOD(TestInsertDuplicate) {
    VarTable var_table;
    var_table.InsertVar(kSampleVar1);
    int result = var_table.InsertVar(kSampleVar1);
    Assert::AreEqual(kFailure, result);
  }

  TEST_METHOD(TestGetAllVarSingle) {
    VarTable var_table;
    var_table.InsertVar(kSampleVar1);
    string* result_table = var_table.GetAllVar();
    Assert::AreEqual(kSampleVar1, result_table[0]);
  }

  TEST_METHOD(TestGetAllVarMultiple) {
    VarTable var_table;
    var_table.InsertVar(kSampleVar1);
    var_table.InsertVar(kSampleVar2);
    string* result_table = var_table.GetAllVar();
    Assert::AreEqual(kSampleVar1, result_table[0]);
    Assert::AreEqual(kSampleVar2, result_table[1]);
  }

  TEST_METHOD(TestGetAllVarEmpty) {
    VarTable var_table;
    string result = *var_table.GetAllVar();
    Assert::AreEqual(string(NULL), result);
  }

};
}
