#include "stdafx.h"
#include "CppUnitTest.h"
#include "modifies_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestModifiesTable) {
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 3;
  const VarIndex kVarIndex1 = 1;
  const VarIndex kVarIndex2 = 2;
  const ProcIndex kProcIndex1 = 1;
  const ProcIndex kProcIndex2 = 2;

  TEST_METHOD(TestIsModifiedByS) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    Assert::IsTrue(modifies_table.IsModifiedByS(kStmtNum1, kVarIndex1));
    Assert::IsFalse(modifies_table.IsModifiedByS(kStmtNum1, kVarIndex2));
  }

  TEST_METHOD(TestIsModifiedByP) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    Assert::IsTrue(modifies_table.IsModifiedByP(kProcIndex1, kVarIndex1));
    Assert::IsFalse(modifies_table.IsModifiedByP(kProcIndex1, kVarIndex2));
  }

  TEST_METHOD(TestGetModifiedVarS) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    VarIndexList result1 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kVarIndex1, result1.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    VarIndexList result2 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kVarIndex1, result2.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex2);
    VarIndexList result3 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kVarIndex1, result3.front());
    Assert::AreEqual(kVarIndex2, result3.back());
    VarIndexList result4 = modifies_table.GetModifiedVarS(kStmtNum2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetModifiedVarP) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex2);
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    VarIndexList result1 = modifies_table.GetModifiedVarP(kProcIndex1);
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kVarIndex1, result1.front());
    Assert::AreEqual(kVarIndex2, result1.back());
    VarIndexList result2 = modifies_table.GetModifiedVarP(kProcIndex2);
    Assert::IsTrue(result2.size() == 0);
  }

  TEST_METHOD(TestGetModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumList result1 = modifies_table.GetModifyingStmt(kVarIndex1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumList result2 = modifies_table.GetModifyingStmt(kVarIndex1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    modifies_table.InsertModifiesS(kStmtNum2, kVarIndex1);
    StmtNumList result3 = modifies_table.GetModifyingStmt(kVarIndex1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
    StmtNumList result4 = modifies_table.GetModifyingStmt(kVarIndex2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetModifyingProc) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexList result1 = modifies_table.GetModifyingProc(kVarIndex1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kProcIndex1, result1.front());
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexList result2 = modifies_table.GetModifyingProc(kVarIndex1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcIndex1, result2.front());
    modifies_table.InsertModifiesP(kProcIndex2, kVarIndex1);
    ProcIndexList result3 = modifies_table.GetModifyingProc(kVarIndex1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kProcIndex1, result3.front());
    Assert::AreEqual(kProcIndex2, result3.back());
    ProcIndexList result4 = modifies_table.GetModifyingProc(kVarIndex2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetAllModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumList result1 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumList result2 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    modifies_table.InsertModifiesS(kStmtNum2, kVarIndex1);
    StmtNumList result3 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
  }

  TEST_METHOD(TestGetAllModifyingProc) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexList result1 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kProcIndex1, result1.front());
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexList result2 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcIndex1, result2.front());
    modifies_table.InsertModifiesP(kProcIndex2, kVarIndex1);
    ProcIndexList result3 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kProcIndex1, result3.front());
    Assert::AreEqual(kProcIndex2, result3.back());
  }

  TEST_METHOD(TestGetAllModifiesPairS) {
    ModifiesTable modifies_table;
    StmtVarIndexPairList result1 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result1.size() == 0);
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtVarIndexPairList result2 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front().first);
    Assert::AreEqual(kVarIndex1, result2.front().second);
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtVarIndexPairList result3 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum1, result3.front().first);
    Assert::AreEqual(kVarIndex1, result3.front().second);
    modifies_table.InsertModifiesS(kStmtNum2, kVarIndex1);
    StmtVarIndexPairList result4 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kStmtNum1, result4.front().first);
    Assert::AreEqual(kVarIndex1, result4.front().second);
    Assert::AreEqual(kStmtNum2, result4.back().first);
    Assert::AreEqual(kVarIndex1, result4.back().second);
  }

  TEST_METHOD(TestGetAllModifiesPairP) {
    ModifiesTable modifies_table;
    ProcVarPairList result1 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result1.size() == 0);
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcVarPairList result2 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcIndex1, result2.front().first);
    Assert::AreEqual(kVarIndex1, result2.front().second);
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcVarPairList result3 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kProcIndex1, result3.front().first);
    Assert::AreEqual(kVarIndex1, result3.front().second);
    modifies_table.InsertModifiesP(kProcIndex2, kVarIndex1);
    ProcVarPairList result4 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kProcIndex2, result4.front().first);
    Assert::AreEqual(kVarIndex1, result4.front().second);
    Assert::AreEqual(kProcIndex1, result4.back().first);
    Assert::AreEqual(kVarIndex1, result4.back().second);
  }
};
}  // namespace PKBTests