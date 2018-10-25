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
    VarIndexList used_vars = uses.GetUsedVarS(kSampleStmtIdx2);
    Assert::IsTrue(used_vars.size() == 2);
    Assert::AreEqual(kSampleVarIndex1, used_vars.front());
    Assert::AreEqual(kSampleVarIndex2, used_vars.back());
  }

  TEST_METHOD(TestGetUsedVarP) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex2);
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex2);
    VarIndexList used_vars = uses.GetUsedVarP(kSampleProcIndex1);
    Assert::IsTrue(used_vars.size() == 2);
    Assert::AreEqual(kSampleVarIndex1, used_vars.front());
    Assert::AreEqual(kSampleVarIndex2, used_vars.back());
  }

  TEST_METHOD(TestGetAllUsingStmt) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarIndex2);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarIndex1);
    StmtNumList using_stmts = uses.GetAllUsingStmt();
    Assert::IsTrue(using_stmts.size() == 2);
    Assert::AreEqual(kSampleStmtIdx2, using_stmts.front());
    Assert::AreEqual(kSampleStmtIdx3, using_stmts.back());
  }

  TEST_METHOD(TestGetAllUsingProc) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex2);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex1);
    ProcIndexList using_stmts = uses.GetAllUsingProc();
    Assert::IsTrue(using_stmts.size() == 2);
    Assert::AreEqual(kSampleProcIndex1, using_stmts.front());
    Assert::AreEqual(kSampleProcIndex2, using_stmts.back());
  }

  TEST_METHOD(TestGetUsingStmt) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarIndex1);
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarIndex1);
    StmtNumList using_stmts = uses.GetUsingStmt(kSampleVarIndex1);
    Assert::IsTrue(using_stmts.size() == 2);
    Assert::AreEqual(kSampleStmtIdx1, using_stmts.front());
    Assert::AreEqual(kSampleStmtIdx2, using_stmts.back());
  }

  TEST_METHOD(TestGetUsingProc) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    ProcIndexList using_proc = uses.GetUsingProc(kSampleVarIndex1);
    Assert::IsTrue(using_proc.size() == 2);
    Assert::AreEqual(kSampleProcIndex1, using_proc.front());
    Assert::AreEqual(kSampleProcIndex2, using_proc.back());
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
    StmtVarIndexPairList uses_pair_list = uses.GetAllUsesSPair();
    Assert::IsTrue(uses_pair_list.size() == 2);
    Assert::AreEqual(kSampleStmtIdx1, uses_pair_list.front().first);
    Assert::AreEqual(kSampleVarIndex1, uses_pair_list.front().second);
    Assert::AreEqual(kSampleStmtIdx2, uses_pair_list.back().first);
    Assert::AreEqual(kSampleVarIndex1, uses_pair_list.back().second);
  }

  TEST_METHOD(TestGetAllUsesPPair) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcIndex1, kSampleVarIndex1);
    uses.InsertUsesP(kSampleProcIndex2, kSampleVarIndex1);
    ProcVarPairList uses_pair_list = uses.GetAllUsesPPair();
    Assert::IsTrue(uses_pair_list.size() == 2);
    Assert::AreEqual(kSampleProcIndex1, uses_pair_list.front().first);
    Assert::AreEqual(kSampleVarIndex1, uses_pair_list.front().second);
    Assert::AreEqual(kSampleProcIndex2, uses_pair_list.back().first);
    Assert::AreEqual(kSampleVarIndex1, uses_pair_list.back().second);
  }
};
}