#include "stdafx.h"
#include "CppUnitTest.h"
#include "modifies_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestModifiesTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "3";
  const VarName kVarName1 = "a";
  const VarName kVarName2 = "b";
  const ProcName kProcName1 = "simple";
  const ProcName kProcName2 = "one";

  TEST_METHOD(TestIsModifiedByS) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    Assert::IsTrue(modifies_table.IsModifiedByS(kStmtNum1, kVarName1));
    Assert::IsFalse(modifies_table.IsModifiedByS(kStmtNum1, kVarName2));
  }

  TEST_METHOD(TestIsModifiedByP) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    Assert::IsTrue(modifies_table.IsModifiedByP(kProcName1, kVarName1));
    Assert::IsFalse(modifies_table.IsModifiedByP(kProcName1, kVarName2));
  }

  TEST_METHOD(TestGetModifiedVarS) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    VarNameList result1 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kVarName1, result1.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    VarNameList result2 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kVarName1, result2.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarName2);
    VarNameList result3 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kVarName1, result3.front());
    Assert::AreEqual(kVarName2, result3.back());
    VarNameList result4 = modifies_table.GetModifiedVarS(kStmtNum2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetModifiedVarP) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    modifies_table.InsertModifiesP(kProcName1, kVarName2);
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    VarNameList result1 = modifies_table.GetModifiedVarP(kProcName1);
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kVarName1, result1.front());
    Assert::AreEqual(kVarName2, result1.back());
    VarNameList result2 = modifies_table.GetModifiedVarP(kProcName2);
    Assert::IsTrue(result2.size() == 0);
  }

  TEST_METHOD(TestGetModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    StmtNumList result1 = modifies_table.GetModifyingStmt(kVarName1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    StmtNumList result2 = modifies_table.GetModifyingStmt(kVarName1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    modifies_table.InsertModifiesS(kStmtNum2, kVarName1);
    StmtNumList result3 = modifies_table.GetModifyingStmt(kVarName1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
    StmtNumList result4 = modifies_table.GetModifyingStmt(kVarName2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetModifyingProc) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    ProcNameList result1 = modifies_table.GetModifyingProc(kVarName1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kProcName1, result1.front());
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    ProcNameList result2 = modifies_table.GetModifyingProc(kVarName1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcName1, result2.front());
    modifies_table.InsertModifiesP(kProcName2, kVarName1);
    ProcNameList result3 = modifies_table.GetModifyingProc(kVarName1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kProcName1, result3.front());
    Assert::AreEqual(kProcName2, result3.back());
    ProcNameList result4 = modifies_table.GetModifyingProc(kVarName2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetAllModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    StmtNumList result1 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    StmtNumList result2 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    modifies_table.InsertModifiesS(kStmtNum2, kVarName1);
    StmtNumList result3 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
  }

  TEST_METHOD(TestGetAllModifyingProc) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    ProcNameList result1 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kProcName1, result1.front());
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    ProcNameList result2 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcName1, result2.front());
    modifies_table.InsertModifiesP(kProcName2, kVarName1);
    ProcNameList result3 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kProcName1, result3.front());
    Assert::AreEqual(kProcName2, result3.back());
  }

  TEST_METHOD(TestGetAllModifiesPairS) {
    ModifiesTable modifies_table;
    StmtVarPairList result1 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result1.size() == 0);
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    StmtVarPairList result2 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front().first);
    Assert::AreEqual(kVarName1, result2.front().second);
    modifies_table.InsertModifiesS(kStmtNum1, kVarName1);
    StmtVarPairList result3 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum1, result3.front().first);
    Assert::AreEqual(kVarName1, result3.front().second);
    modifies_table.InsertModifiesS(kStmtNum2, kVarName1);
    StmtVarPairList result4 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kStmtNum1, result4.front().first);
    Assert::AreEqual(kVarName1, result4.front().second);
    Assert::AreEqual(kStmtNum2, result4.back().first);
    Assert::AreEqual(kVarName1, result4.back().second);
  }

  TEST_METHOD(TestGetAllModifiesPairP) {
    ModifiesTable modifies_table;
    ProcVarPairList result1 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result1.size() == 0);
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    ProcVarPairList result2 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcName1, result2.front().first);
    Assert::AreEqual(kVarName1, result2.front().second);
    modifies_table.InsertModifiesP(kProcName1, kVarName1);
    ProcVarPairList result3 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kProcName1, result3.front().first);
    Assert::AreEqual(kVarName1, result3.front().second);
    modifies_table.InsertModifiesP(kProcName2, kVarName1);
    ProcVarPairList result4 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kProcName2, result4.front().first);
    Assert::AreEqual(kVarName1, result4.front().second);
    Assert::AreEqual(kProcName1, result4.back().first);
    Assert::AreEqual(kVarName1, result4.back().second);
  }
};
}  // namespace PKBTests