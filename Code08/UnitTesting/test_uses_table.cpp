#include "stdafx.h"
#include "CPPUnitTest.h"
#include "uses_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {
TEST_CLASS(TestUsesTable) {
  const StmtNum kSampleStmtIdx1 = 1;
  const StmtNum kSampleStmtIdx2 = 4;
  const StmtNum kSampleStmtIdx3 = 102;
  const StmtNum kSampleStmtIdx4 = 27;
  const VarIndex kSampleVarIndex1 = 0;
  const VarIndex kSampleVarIndex2 = 1;
  const ProcIndex kSampleProcIndex1 = 0;
  const ProcIndex kSampleProcIndex2 = 1;

  TEST_METHOD(TestGetUsedVarS) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex2);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex2);
    VarIndexSet used_vars = uses.GetUsedVarS(kSampleStmtIdx2);
    Assert::IsTrue(used_vars.size() == 2);
    VarIndexSet expected_results = VarIndexSet{kSampleVarIndex1, kSampleVarIndex2};
    Assert::IsTrue(expected_results == used_vars);
  }

  TEST_METHOD(TestGetUsedVarP) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex2);
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex2);
    VarIndexSet used_vars = uses.GetUsedVarP(kSampleProcIndex1);
    Assert::IsTrue(used_vars.size() == 2);
    VarIndexSet expected_results = VarIndexSet{ kSampleVarIndex1, kSampleVarIndex2 };
    Assert::IsTrue(expected_results == used_vars);
  }

  TEST_METHOD(TestGetAllUsingStmt) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarIndex2);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarIndex1);
    StmtNumSet using_stmts = uses.GetAllUsingStmt();
    Assert::IsTrue(using_stmts.size() == 2);
    VarIndexSet expected_results = VarIndexSet{ kSampleStmtIdx2, kSampleStmtIdx3 };
    Assert::IsTrue(expected_results == using_stmts);
  }

  TEST_METHOD(TestGetAllUsingProc) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex2);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex1);
    ProcIndexSet using_proc = uses.GetAllUsingProc();
    Assert::IsTrue(using_proc.size() == 2);
    ProcIndexSet expected_results = ProcIndexSet{ kSampleProcIndex1, kSampleProcIndex2 };
    Assert::IsTrue(expected_results == using_proc);
  }

  TEST_METHOD(TestGetUsingStmt) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarIndex1);
    StmtNumSet using_stmts = uses.GetUsingStmt(kSampleVarIndex1);
    Assert::IsTrue(using_stmts.size() == 2);
    StmtNumSet expected_results = StmtNumSet{ kSampleStmtIdx1, kSampleStmtIdx2 };
    Assert::IsTrue(expected_results == using_stmts);
  }

  TEST_METHOD(TestGetUsingProc) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    ProcIndexSet using_proc = uses.GetUsingProc(kSampleVarIndex1);
    Assert::IsTrue(using_proc.size() == 2);
    ProcIndexSet expected_results = ProcIndexSet{ kSampleProcIndex1, kSampleProcIndex2 };
    Assert::IsTrue(expected_results == using_proc);
  }

  TEST_METHOD(TestIsUsedByS) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarIndex1);
    Assert::IsTrue(uses.IsUsedByS(kSampleStmtIdx3, kSampleVarIndex1));
  }

  TEST_METHOD(TestIsUsedByP) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex1);
    Assert::IsTrue(uses.IsUsedByP(kSampleProcIndex2, kSampleVarIndex1));
  }

  TEST_METHOD(TestGetAllUsesSPair) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex1);
    StmtVarPairSet uses_pair_set = uses.GetAllUsesSPair();
    Assert::IsTrue(uses_pair_set.size() == 2);
    StmtVarPairSet expected_result = StmtVarPairSet{ 
      {kSampleStmtIdx1,kSampleVarIndex1},
      {kSampleStmtIdx2,kSampleVarIndex1} };
    Assert::IsTrue(expected_result == uses_pair_set);
  }

  TEST_METHOD(TestGetAllUsesPPair) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex1);
    ProcVarPairSet uses_pair_set = uses.GetAllUsesPPair();
    Assert::IsTrue(uses_pair_set.size() == 2);
    ProcVarPairSet expected_result = ProcVarPairSet{
      {kSampleProcIndex1,kSampleVarIndex1},
      {kSampleProcIndex2,kSampleVarIndex1} };
    Assert::IsTrue(expected_result == uses_pair_set);
  }
};
}  // namespace PKBTests