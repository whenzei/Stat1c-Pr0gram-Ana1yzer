#include "stdafx.h" 
#include "CppUnitTest.h"
#include "stmt_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestStmtTable) {
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 2;
  const StmtNum kStmtNum3 = 3;
  const ProcIndex kProcIndex1 = 0;
  const ProcIndex kProcIndex2 = 1;
  const ProcIndex kProcIndex3 = 2;
  
  TEST_METHOD(TestInsertStmt) {
    StmtTable stmt_table;
    bool result = stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign, kProcIndex1);
    Assert::IsTrue(result);
  }
  
  TEST_METHOD(TestInsertStmtDuplicate) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign, kProcIndex1);
    bool result =
        stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kRead, kProcIndex2);
    Assert::IsFalse(result);
  }
  
  TEST_METHOD(TestInsertStmtMultiple) {
    StmtTable stmt_table;
    bool result1 =
        stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kIf, kProcIndex1);
    bool result2 =
        stmt_table.InsertStmt(kStmtNum2, PqlDeclarationEntity::kAssign, kProcIndex2);
    bool result3 =
        stmt_table.InsertStmt(kStmtNum3, PqlDeclarationEntity::kRead, kProcIndex3);
    Assert::IsTrue(result1);
    Assert::IsTrue(result2);
    Assert::IsTrue(result3);
  }
  
  TEST_METHOD(TestCheckProcIndex) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign, kProcIndex1);
    ProcIndex result = stmt_table.GetProcOfStmt(kStmtNum1);
    Assert::AreEqual(kProcIndex1, result);
  }
  
  TEST_METHOD(TestCheckProcIndexMultiple) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kIf, kProcIndex1);
    stmt_table.InsertStmt(kStmtNum2, PqlDeclarationEntity::kAssign, kProcIndex2);
    stmt_table.InsertStmt(kStmtNum3, PqlDeclarationEntity::kAssign, kProcIndex3);
    ProcIndex result1 = stmt_table.GetProcOfStmt(kStmtNum1);
    Assert::AreEqual(kProcIndex1, result1);
    ProcIndex result2 = stmt_table.GetProcOfStmt(kStmtNum2);
    Assert::AreEqual(kProcIndex2, result2);
    ProcIndex result3 = stmt_table.GetProcOfStmt(kStmtNum3);
    Assert::AreEqual(kProcIndex3, result3);
  }

  TEST_METHOD(TestCheckStmtType) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign, kProcIndex1);
    StmtType result = stmt_table.GetStmtType(kStmtNum1);
    Assert::IsTrue(PqlDeclarationEntity::kAssign == result);
  }
   
  TEST_METHOD(TestCheckStmtTypeMultiple) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kIf, kProcIndex1);
    stmt_table.InsertStmt(kStmtNum2, PqlDeclarationEntity::kAssign, kProcIndex2);
    stmt_table.InsertStmt(kStmtNum3, PqlDeclarationEntity::kAssign, kProcIndex3);
    StmtType result1 = stmt_table.GetStmtType(kStmtNum1);
    Assert::IsTrue(PqlDeclarationEntity::kIf == result1);
    StmtType result2 = stmt_table.GetStmtType(kStmtNum2);
    Assert::IsTrue(PqlDeclarationEntity::kAssign == result2);
    StmtType result3 = stmt_table.GetStmtType(kStmtNum3);
    Assert::IsTrue(PqlDeclarationEntity::kAssign == result3);
  }

  TEST_METHOD(TestIsStmtNum) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, PqlDeclarationEntity::kIf, kProcIndex1);
    bool result = stmt_table.IsStmtNum(kStmtNum1);
    Assert::IsTrue(result);
    result = stmt_table.IsStmtNum(kStmtNum2);
    Assert::IsFalse(result);
  }
};
}  // namespace PKBTests