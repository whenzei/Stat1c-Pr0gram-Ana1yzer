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
    VarIndexSet result1 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kVarIndex1));
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    VarIndexSet result2 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kVarIndex1));
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex2);
    VarIndexSet result3 = modifies_table.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result3.size() == 2);
    Assert::IsTrue(result3.count(kVarIndex1));
    Assert::IsTrue(result3.count(kVarIndex2));
    VarIndexSet result4 = modifies_table.GetModifiedVarS(kStmtNum2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetModifiedVarP) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex2);
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    VarIndexSet result1 = modifies_table.GetModifiedVarP(kProcIndex1);
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kVarIndex1));
    Assert::IsTrue(result1.count(kVarIndex2));
    VarIndexSet result2 = modifies_table.GetModifiedVarP(kProcIndex2);
    Assert::IsTrue(result2.size() == 0);
  }

  TEST_METHOD(TestGetModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumSet result1 = modifies_table.GetModifyingStmt(kVarIndex1);
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kStmtNum1));
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumSet result2 = modifies_table.GetModifyingStmt(kVarIndex1);
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kStmtNum1));
    modifies_table.InsertModifiesS(kStmtNum2, kVarIndex1);
    StmtNumSet result3 = modifies_table.GetModifyingStmt(kVarIndex1);
    Assert::IsTrue(result3.size() == 2);
    Assert::IsTrue(result3.count(kStmtNum1));
    Assert::IsTrue(result3.count(kStmtNum2));
    StmtNumSet result4 = modifies_table.GetModifyingStmt(kVarIndex2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetModifyingProc) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexSet result1 = modifies_table.GetModifyingProc(kVarIndex1);
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kProcIndex1));
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexSet result2 = modifies_table.GetModifyingProc(kVarIndex1);
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kProcIndex1));
    modifies_table.InsertModifiesP(kProcIndex2, kVarIndex1);
    ProcIndexSet result3 = modifies_table.GetModifyingProc(kVarIndex1);
    Assert::IsTrue(result3.size() == 2);
    Assert::IsTrue(result3.count(kProcIndex1));
    Assert::IsTrue(result3.count(kProcIndex2));
    ProcIndexSet result4 = modifies_table.GetModifyingProc(kVarIndex2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetAllModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumSet result1 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kStmtNum1));
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtNumSet result2 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kStmtNum1));
    modifies_table.InsertModifiesS(kStmtNum2, kVarIndex1);
    StmtNumSet result3 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result3.size() == 2);
    Assert::IsTrue(result3.count(kStmtNum1));
    Assert::IsTrue(result3.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllModifyingProc) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexSet result1 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kProcIndex1));
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcIndexSet result2 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kProcIndex1));
    modifies_table.InsertModifiesP(kProcIndex2, kVarIndex1);
    ProcIndexSet result3 = modifies_table.GetAllModifyingProc();
    Assert::IsTrue(result3.size() == 2);
    Assert::IsTrue(result3.count(kProcIndex1));
    Assert::IsTrue(result3.count(kProcIndex2));
  }

  TEST_METHOD(TestGetAllModifiesPairS) {
    ModifiesTable modifies_table;
    StmtVarPairSet result1 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result1.size() == 0);
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtVarPairSet result2 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(std::make_pair(kStmtNum1, kVarIndex1)));
    modifies_table.InsertModifiesS(kStmtNum1, kVarIndex1);
    StmtVarPairSet result3 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result3.size() == 1);
    Assert::IsTrue(result3.count(std::make_pair(kStmtNum1, kVarIndex1)));
    modifies_table.InsertModifiesS(kStmtNum2, kVarIndex1);
    StmtVarPairSet result4 = modifies_table.GetAllModifiesPairS();
    Assert::IsTrue(result4.size() == 2);
    Assert::IsTrue(result4.count(std::make_pair(kStmtNum1, kVarIndex1)));
    Assert::IsTrue(result4.count(std::make_pair(kStmtNum2, kVarIndex1)));
  }

  TEST_METHOD(TestGetAllModifiesPairP) {
    ModifiesTable modifies_table;
    ProcVarPairSet result1 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result1.size() == 0);
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcVarPairSet result2 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(std::make_pair(kProcIndex1, kVarIndex1)));
    modifies_table.InsertModifiesP(kProcIndex1, kVarIndex1);
    ProcVarPairSet result3 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result3.size() == 1);
    Assert::IsTrue(result3.count(std::make_pair(kProcIndex1, kVarIndex1)));
    modifies_table.InsertModifiesP(kProcIndex2, kVarIndex1);
    ProcVarPairSet result4 = modifies_table.GetAllModifiesPairP();
    Assert::IsTrue(result4.size() == 2);
    Assert::IsTrue(result4.count(std::make_pair(kProcIndex1, kVarIndex1)));
    Assert::IsTrue(result4.count(std::make_pair(kProcIndex2, kVarIndex1)));
  }
};
}  // namespace PKBTests