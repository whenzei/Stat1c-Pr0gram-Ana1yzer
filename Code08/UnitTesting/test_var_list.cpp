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
    VarIndexList result_list = var_list.GetAllVarIndices();
    Assert::IsTrue(result_list.size() == 2);
    Assert::AreEqual(kSampleVarIndex1, result_list.front());
    Assert::AreEqual(kSampleVarIndex2, result_list.back());
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
    VarIndexPairList result_list = var_list.GetAllVarIndexTwin();
    Assert::IsTrue(result_list.size() == 2);
    Assert::AreEqual(kSampleVarIndex1, result_list.front().first);
    Assert::AreEqual(kSampleVarIndex1, result_list.front().second);
    Assert::AreEqual(kSampleVarIndex2, result_list.back().first);
    Assert::AreEqual(kSampleVarIndex2, result_list.back().second);
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
    Assert::AreEqual(-1, var_list.GetReadVar(5));
    Assert::AreEqual(-1, var_list.GetReadVar(2));
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2),
                     var_list.GetPrintVar(5));
    Assert::AreEqual(-1, var_list.GetPrintVar(3));
    Assert::AreEqual(-1, var_list.GetPrintVar(0));
  }

  TEST_METHOD(TestGetReadPrintStmt) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 3);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kPrint, 5);
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 6);
    var_list.InsertVarName(kSampleVar3);
    StmtNumList result =
        var_list.GetReadStmt(var_list.GetVarIndex(kSampleVar1));
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(3, result.front());
    Assert::AreEqual(6, result.back());
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
    Assert::AreEqual(5, result.front());
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
    VarIndexList read_vars = var_list.GetAllReadVar();
    Assert::IsTrue(read_vars.size() == 2);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar1), read_vars.front());
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2), read_vars.back());
    VarIndexList print_vars = var_list.GetAllPrintVar();
    Assert::IsTrue(print_vars.size() == 2);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2), print_vars.front());
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar3), print_vars.back());
  }

  TEST_METHOD(TestGetAllReadPrintVarTwin) {
    VarList var_list;
    var_list.InsertVarName(kSampleVar1, PqlDeclarationEntity::kRead, 3);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kPrint, 5);
    var_list.InsertVarName(kSampleVar3);
    var_list.InsertVarName(kSampleVar3, PqlDeclarationEntity::kPrint, 6);
    var_list.InsertVarName(kSampleVar2, PqlDeclarationEntity::kRead, 10);
    VarIndexPairList read_vars = var_list.GetAllReadVarTwin();
    Assert::IsTrue(read_vars.size() == 2);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar1),
                     read_vars.front().first);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar1),
                     read_vars.front().second);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2), read_vars.back().first);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2),
                     read_vars.back().second);
    VarIndexPairList print_vars = var_list.GetAllPrintVarTwin();
    Assert::IsTrue(print_vars.size() == 2);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2),
                     print_vars.front().first);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar2),
                     print_vars.front().second);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar3),
                     print_vars.back().first);
    Assert::AreEqual(var_list.GetVarIndex(kSampleVar3),
                     print_vars.back().second);
  }
};
}  // namespace PKBTests
