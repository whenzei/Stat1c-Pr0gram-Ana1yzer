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
  const VarName kSampleVarName1 = "j";
  const VarName kSampleVarName2 = "iter";
  const ProcName kSampleProcName1 = "simple";
  const ProcName kSampleProcName2 = "p";

  TEST_METHOD(TestGetUsedVarS) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarName1);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarName2);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarName2);
    VarNameList used_vars = uses.GetUsedVarS(kSampleStmtIdx2);
    Assert::IsTrue(used_vars.size() == 2);
    Assert::AreEqual(kSampleVarName1, used_vars.front());
    Assert::AreEqual(kSampleVarName2, used_vars.back());
  }

  TEST_METHOD(TestGetUsedVarP) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcName1, kSampleVarName1);
    uses.InsertUsesP(kSampleProcName1, kSampleVarName2);
    uses.InsertUsesP(kSampleProcName1, kSampleVarName2);
    VarNameList used_vars = uses.GetUsedVarP(kSampleProcName1);
    Assert::IsTrue(used_vars.size() == 2);
    Assert::AreEqual(kSampleVarName1, used_vars.front());
    Assert::AreEqual(kSampleVarName2, used_vars.back());
  }

  TEST_METHOD(TestGetAllUsingStmt) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarName1);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarName2);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarName1);
    StmtNumList using_stmts = uses.GetAllUsingStmt();
    Assert::IsTrue(using_stmts.size() == 2);
    Assert::AreEqual(kSampleStmtIdx2, using_stmts.front());
    Assert::AreEqual(kSampleStmtIdx3, using_stmts.back());
  }

  TEST_METHOD(TestGetAllUsingProc) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcName1, kSampleVarName1);
    uses.InsertUsesP(kSampleProcName2, kSampleVarName2);
    uses.InsertUsesP(kSampleProcName2, kSampleVarName1);
    ProcNameList using_stmts = uses.GetAllUsingProc();
    Assert::IsTrue(using_stmts.size() == 2);
    Assert::AreEqual(kSampleProcName1, using_stmts.front());
    Assert::AreEqual(kSampleProcName2, using_stmts.back());
  }

  TEST_METHOD(TestGetUsingStmt) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarName1);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarName1);
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarName1);
    StmtNumList using_stmts = uses.GetUsingStmt(kSampleVarName1);
    Assert::IsTrue(using_stmts.size() == 2);
    Assert::AreEqual(kSampleStmtIdx1, using_stmts.front());
    Assert::AreEqual(kSampleStmtIdx2, using_stmts.back());
  }

  TEST_METHOD(TestGetUsingProc) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcName1, kSampleVarName1);
    uses.InsertUsesP(kSampleProcName2, kSampleVarName1);
    uses.InsertUsesP(kSampleProcName1, kSampleVarName1);
    ProcNameList using_proc = uses.GetUsingProc(kSampleVarName1);
    Assert::IsTrue(using_proc.size() == 2);
    Assert::AreEqual(kSampleProcName1, using_proc.front());
    Assert::AreEqual(kSampleProcName2, using_proc.back());
  }

  TEST_METHOD(TestIsUsedByS) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarName1);
    uses.InsertUsesS(kSampleStmtIdx3, kSampleVarName1);
    Assert::IsTrue(uses.IsUsedByS(kSampleStmtIdx3, kSampleVarName1));
  }

  TEST_METHOD(TestIsUsedByP) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcName1, kSampleVarName1);
    uses.InsertUsesP(kSampleProcName2, kSampleVarName1);
    Assert::IsTrue(uses.IsUsedByP(kSampleProcName2, kSampleVarName1));
  }

  TEST_METHOD(TestGetAllUsesSPair) {
    UsesTable uses;
    uses.InsertUsesS(kSampleStmtIdx1, kSampleVarName1);
    uses.InsertUsesS(kSampleStmtIdx2, kSampleVarName1);
    StmtVarPairList uses_pair_list = uses.GetAllUsesSPair();
    Assert::IsTrue(uses_pair_list.size() == 2);
    Assert::AreEqual(kSampleStmtIdx1, uses_pair_list.front().first);
    Assert::AreEqual(kSampleVarName1, uses_pair_list.front().second);
    Assert::AreEqual(kSampleStmtIdx2, uses_pair_list.back().first);
    Assert::AreEqual(kSampleVarName1, uses_pair_list.back().second);
  }

  TEST_METHOD(TestGetAllUsesPPair) {
    UsesTable uses;
    uses.InsertUsesP(kSampleProcName1, kSampleVarName1);
    uses.InsertUsesP(kSampleProcName2, kSampleVarName1);
    ProcVarPairList uses_pair_list = uses.GetAllUsesPPair();
    Assert::IsTrue(uses_pair_list.size() == 2);
    Assert::AreEqual(kSampleProcName2, uses_pair_list.front().first);
    Assert::AreEqual(kSampleVarName1, uses_pair_list.front().second);
    Assert::AreEqual(kSampleProcName1, uses_pair_list.back().first);
    Assert::AreEqual(kSampleVarName1, uses_pair_list.back().second);
  }
};
}