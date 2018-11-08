#include "stdafx.h"
#include "CppUnitTest.h"
#include "var_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestVarList) {
  const string kSampleVar1 = "a";
  const string kSampleVar2 = "b";
  const string kSampleVar3 = "c";
  const int kSampleVarIndex1 = 0;
  const int kSampleVarIndex2 = 1;

  TEST_METHOD(TestInsertVar) {
    VarList var_list;
    int result = var_list.InsertVarName(kSampleVar1);
    Assert::AreEqual(0, result);
    result = var_list.InsertVarName(kSampleVar2);
    Assert::AreEqual(1, result);
    result = var_list.InsertVarName(kSampleVar2);
    Assert::AreEqual(-1, result);
  }

  TEST_METHOD(TestGetAllVarName) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    // duplicate
    var_list.InsertVarName(kSampleVar1);
    VarIndexSet result_list = var_list.GetAllVarIndices();
    Assert::IsTrue(result_list.size() == 2);
    Assert::IsTrue(result_list.count(kSampleVarIndex1));
    Assert::IsTrue(result_list.count(kSampleVarIndex2));
  }

  TEST_METHOD(TestIsVarName) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    bool result = var_list.IsVarName(kSampleVar1);
    Assert::IsTrue(result);
    result = var_list.IsVarName(kSampleVar2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestIsVarIndex) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    bool result = var_list.IsVarIndex(kSampleVarIndex1);
    Assert::IsTrue(result);
    result = var_list.IsVarIndex(kSampleVarIndex2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetAllVarNameTwin) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    // duplicate
    var_list.InsertVarName(kSampleVar1);
    VarIndexPairSet result_list = var_list.GetAllVarIndexTwin();
    Assert::IsTrue(result_list.size() == 2);
    Assert::IsTrue(
        result_list.count(make_pair(kSampleVarIndex1, kSampleVarIndex1)));
    Assert::IsTrue(
        result_list.count(make_pair(kSampleVarIndex2, kSampleVarIndex2)));
  }

  TEST_METHOD(TestGetIndexToVarMapping) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    IndexVarMap index_var_map = var_list.GetIndexToVarMapping();
    Assert::AreEqual(kSampleVar1, index_var_map[0]);
    index_var_map = var_list.GetIndexToVarMapping();
    Assert::AreEqual(kSampleVar2, index_var_map[1]);
    // Duplicate. Shouldn't affect index
    var_list.InsertVarName(kSampleVar2);
    index_var_map = var_list.GetIndexToVarMapping();
    Assert::AreEqual(kSampleVar2, index_var_map[1]);
  }

  TEST_METHOD(TestGetVarToIndexMapping) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1);
    var_list.InsertVarName(kSampleVar2);
    VarIndexMap var_index_map = var_list.GetVarToIndexMapping();
    Assert::IsTrue(var_index_map.size() == 2);
    Assert::AreEqual(0, var_index_map[kSampleVar1]);
    var_index_map = var_list.GetVarToIndexMapping();
    Assert::AreEqual(1, var_index_map[kSampleVar2]);
    // Duplicate. Shouldn't affect var
    var_list.InsertVarName(kSampleVar2);
    var_index_map = var_list.GetVarToIndexMapping();
    Assert::AreEqual(1, var_index_map[kSampleVar2]);
  }

  TEST_METHOD(TestGetReadPrintVar) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 3);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kPrint, 5);
    var_list.InsertVarName(kSampleVar3);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar1), var_list.GetReadVar(3));
    Assert::AreEqual(VarIndex(), var_list.GetReadVar(5));
    Assert::AreEqual(VarIndex(), var_list.GetReadVar(2));
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2),
                     var_list.GetPrintVar(5));
    Assert::AreEqual(VarIndex(), var_list.GetReadVar(3));
    Assert::AreEqual(VarIndex(), var_list.GetReadVar(0));
  }

  TEST_METHOD(TestGetReadPrintStmt) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 3);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kPrint, 5);
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 6);
    var_list.InsertVarName(kSampleVar3);
    StmtNumSet result =
        var_list.GetReadStmt(var_list.GetVarIndex(kSampleVar1));
    Assert::IsTrue(result.size() == 2);
    Assert::IsTrue(result.count(3));
    Assert::IsTrue(result.count(6));
    result = var_list.GetReadStmt(var_list.GetVarIndex(kSampleVar2));
    Assert::IsTrue(result.empty());
    result = var_list.GetReadStmt(var_list.GetVarIndex(kSampleVar3));
    Assert::IsTrue(result.empty());
    result = var_list.GetReadStmt(var_list.GetVarIndex("non existent"));
    Assert::IsTrue(result.empty());
    result = var_list.GetPrintStmt(var_list.GetVarIndex(kSampleVar1));
    Assert::IsTrue(result.empty());
    result = var_list.GetPrintStmt(var_list.GetVarIndex(kSampleVar2));
    Assert::IsTrue(result.size() == 1);
    Assert::IsTrue(result.count(5));
    result = var_list.GetPrintStmt(var_list.GetVarIndex(kSampleVar3));
    Assert::IsTrue(result.empty());
    result = var_list.GetPrintStmt(var_list.GetVarIndex("non existent"));
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestIsReadPrintVar) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 3);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kPrint, 5);
    var_list.InsertVarName(kSampleVar3);
    Assert::IsTrue(var_list.IsReadVar(var_list.GetVarIndex(kSampleVar1)));
    Assert::IsTrue(var_list.IsPrintVar(var_list.GetVarIndex(kSampleVar2)));
    Assert::IsFalse(var_list.IsReadVar(var_list.GetVarIndex(kSampleVar2)));
    Assert::IsFalse(var_list.IsPrintVar(var_list.GetVarIndex(kSampleVar1)));
    Assert::IsFalse(var_list.IsReadVar(var_list.GetVarIndex(kSampleVar3)));
    Assert::IsFalse(var_list.IsPrintVar(var_list.GetVarIndex(kSampleVar3)));
  }

  TEST_METHOD(TestGetAllReadPrintVar) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 3);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kPrint, 5);
    var_list.InsertVarName(kSampleVar3);
    var_list.InsertVarName(kSampleVar3, PqlDeclarationEntity::kPrint, 6);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kRead, 10);
    VarIndexSet read_vars = var_list.GetAllReadVar();
    Assert::IsTrue(read_vars.size() == 2);
    Assert::IsTrue(read_vars.count(var_list.GetVarIndex(kSampleVar1)));
    Assert::IsTrue(read_vars.count(var_list.GetVarIndex(kSampleVar2)));
    VarIndexSet print_vars = var_list.GetAllPrintVar();
    Assert::IsTrue(print_vars.size() == 2);
    Assert::IsTrue(print_vars.count(var_list.GetVarIndex(kSampleVar2)));
    Assert::IsTrue(print_vars.count(var_list.GetVarIndex(kSampleVar3)));
  }

  TEST_METHOD(TestGetAllReadPrintVarTwin) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 3);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kPrint, 5);
    var_list.InsertVarName(kSampleVar3);
    var_list.InsertVarName(kSampleVar3, PqlDeclarationEntity::kPrint, 6);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kRead, 10);
    VarIndexPairSet read_vars = var_list.GetAllReadVarTwin();
    Assert::IsTrue(read_vars.size() == 2);
    Assert::IsTrue(read_vars.count(make_pair(
        var_list.GetVarIndex(kSampleVar1), var_list.GetVarIndex(kSampleVar1))));
    Assert::IsTrue(read_vars.count(make_pair(
        var_list.GetVarIndex(kSampleVar2), var_list.GetVarIndex(kSampleVar2))));
    VarIndexPairSet print_vars = var_list.GetAllPrintVarTwin();
    Assert::IsTrue(print_vars.size() == 2);
    Assert::IsTrue(print_vars.count(make_pair(
        var_list.GetVarIndex(kSampleVar2), var_list.GetVarIndex(kSampleVar2))));
    Assert::IsTrue(print_vars.count(make_pair(
        var_list.GetVarIndex(kSampleVar3), var_list.GetVarIndex(kSampleVar3))));
  }
};
}  // namespace PKBTests
