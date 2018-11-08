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
    bool result =
        stmt_table.InsertStmt(kStmtNum1, StmtType::kAssign, kProcIndex1);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestInsertStmtDuplicate) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, StmtType::kAssign, kProcIndex1);
    bool result =
        stmt_table.InsertStmt(kStmtNum1, StmtType::kRead, kProcIndex2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestInsertStmtMultiple) {
    StmtTable stmt_table;
    bool result1 = stmt_table.InsertStmt(kStmtNum1, StmtType::kIf, kProcIndex1);
    bool result2 = stmt_table.InsertStmt(
        kStmtNum2, PqlDeclarationEntity::kAssign, kProcIndex2);
    bool result3 =
        stmt_table.InsertStmt(kStmtNum3, StmtType::kRead, kProcIndex3);
    Assert::IsTrue(result1);
    Assert::IsTrue(result2);
    Assert::IsTrue(result3);
  }

  TEST_METHOD(TestGetProcIndex) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, StmtType::kIf, kProcIndex1);
    stmt_table.InsertStmt(kStmtNum2, StmtType::kAssign, kProcIndex2);
    stmt_table.InsertStmt(kStmtNum3, StmtType::kAssign, kProcIndex3);
    ProcIndex result1 = stmt_table.GetProcOfStmt(kStmtNum1);
    Assert::AreEqual(kProcIndex1, result1);
    ProcIndex result2 = stmt_table.GetProcOfStmt(kStmtNum2);
    Assert::AreEqual(kProcIndex2, result2);
    ProcIndex result3 = stmt_table.GetProcOfStmt(kStmtNum3);
    Assert::AreEqual(kProcIndex3, result3);
  }

  TEST_METHOD(TestGetStmtTypeMultiple) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, StmtType::kIf, kProcIndex1);
    stmt_table.InsertStmt(kStmtNum2, StmtType::kAssign, kProcIndex2);
    stmt_table.InsertStmt(kStmtNum3, StmtType::kAssign, kProcIndex3);
    StmtType result1 = stmt_table.GetStmtType(kStmtNum1);
    Assert::IsTrue(StmtType::kIf == result1);
    StmtType result2 = stmt_table.GetStmtType(kStmtNum2);
    Assert::IsTrue(StmtType::kAssign == result2);
    StmtType result3 = stmt_table.GetStmtType(kStmtNum3);
    Assert::IsTrue(StmtType::kAssign == result3);
  }

  TEST_METHOD(TestIsStmtNum) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, StmtType::kIf, kProcIndex1);
    bool result = stmt_table.IsStmtNum(kStmtNum1);
    Assert::IsTrue(result);
    result = stmt_table.IsStmtNum(kStmtNum2);
    Assert::IsFalse(result);
  }
};
}  // namespace PKBTests