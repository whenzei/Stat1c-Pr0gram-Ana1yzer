#include "stdafx.h"
#include "CppUnitTest.h"
#include "stmt_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestStmtTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";
  const StmtNum kStmtNum4 = "";
  const StmtListIndex kStmtListIndex1 = 0;
  const StmtListIndex kStmtListIndex2 = 1;
  const StmtListIndex kStmtListIndex3 = 2;
  
  TEST_METHOD(TestInsertStmt) {
    StmtTable stmt_table;
    bool result = stmt_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestInsertStmtEmpty) {
    StmtTable stmt_table;
    bool result = stmt_table.InsertStmt(kStmtNum4, kStmtListIndex1);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestInsertStmtDuplicate) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    bool result = stmt_table.InsertStmt(kStmtNum1, kStmtListIndex2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestInsertStmtMultiple) {
    StmtTable stmt_table;
    bool result1 = stmt_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    bool result2 = stmt_table.InsertStmt(kStmtNum2, kStmtListIndex2);
    bool result3 = stmt_table.InsertStmt(kStmtNum3, kStmtListIndex3);
    Assert::IsTrue(result1);
    Assert::IsTrue(result2);
    Assert::IsTrue(result3);
  }

  TEST_METHOD(TestCheckStmtListIndex) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    StmtListIndex result = stmt_table.GetStmtListIndex(kStmtNum1);
    Assert::AreEqual(kStmtListIndex1, result);
  }
  TEST_METHOD(TestCheckStmtListIndexMultiple) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    stmt_table.InsertStmt(kStmtNum2, kStmtListIndex2);
    stmt_table.InsertStmt(kStmtNum3, kStmtListIndex3);
    StmtListIndex result1 = stmt_table.GetStmtListIndex(kStmtNum1);
    Assert::AreEqual(kStmtListIndex1, result1);
    StmtListIndex result2 = stmt_table.GetStmtListIndex(kStmtNum2);
    Assert::AreEqual(kStmtListIndex2, result2);
    StmtListIndex result3 = stmt_table.GetStmtListIndex(kStmtNum3);
    Assert::AreEqual(kStmtListIndex3, result3);
  }
};
}