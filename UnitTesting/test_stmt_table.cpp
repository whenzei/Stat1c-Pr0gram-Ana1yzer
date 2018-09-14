#include "stdafx.h" 
#include "CppUnitTest.h"
#include "stmt_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestStmtTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";
  const StmtListIndex kStmtListIndex1 = 0;
  const StmtListIndex kStmtListIndex2 = 1;
  const StmtListIndex kStmtListIndex3 = 2;
  
  TEST_METHOD(TestInsertStmt) {
    StmtTable stmt_table;
    bool result = stmt_table.InsertStmt(kStmtNum1, kAssign, kStmtListIndex1);
    Assert::IsTrue(result);
  }
  
  TEST_METHOD(TestInsertStmtDuplicate) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kAssign, kStmtListIndex1);
    bool result =
        stmt_table.InsertStmt(kStmtNum1, kRead, kStmtListIndex2);
    Assert::IsFalse(result);
  }
  
  TEST_METHOD(TestInsertStmtMultiple) {
    StmtTable stmt_table;
    bool result1 =
        stmt_table.InsertStmt(kStmtNum1, kIf, kStmtListIndex1);
    bool result2 =
        stmt_table.InsertStmt(kStmtNum2, kAssign, kStmtListIndex2);
    bool result3 =
        stmt_table.InsertStmt(kStmtNum3, kRead, kStmtListIndex3);
    Assert::IsTrue(result1);
    Assert::IsTrue(result2);
    Assert::IsTrue(result3);
  }
  
  TEST_METHOD(TestCheckStmtListIndex) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kAssign, kStmtListIndex1);
    StmtListIndex result = stmt_table.GetStmtListIndex(kStmtNum1);
    Assert::AreEqual(kStmtListIndex1, result);
  }
  
  TEST_METHOD(TestCheckStmtListIndexMultiple) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kIf, kStmtListIndex1);
    stmt_table.InsertStmt(kStmtNum2, kAssign, kStmtListIndex2);
    stmt_table.InsertStmt(kStmtNum3, kAssign, kStmtListIndex3);
    StmtListIndex result1 = stmt_table.GetStmtListIndex(kStmtNum1);
    Assert::AreEqual(kStmtListIndex1, result1);
    StmtListIndex result2 = stmt_table.GetStmtListIndex(kStmtNum2);
    Assert::AreEqual(kStmtListIndex2, result2);
    StmtListIndex result3 = stmt_table.GetStmtListIndex(kStmtNum3);
    Assert::AreEqual(kStmtListIndex3, result3);
  }

  TEST_METHOD(TestCheckStmtType) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kAssign, kStmtListIndex1);
    StmtType result = stmt_table.GetStmtType(kStmtNum1);
    Assert::IsTrue(kAssign == result);
  }
   
  TEST_METHOD(TestCheckStmtTypeMultiple) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kStmtNum1, kIf, kStmtListIndex1);
    stmt_table.InsertStmt(kStmtNum2, kAssign, kStmtListIndex2);
    stmt_table.InsertStmt(kStmtNum3, kAssign, kStmtListIndex3);
    StmtType result1 = stmt_table.GetStmtType(kStmtNum1);
    Assert::IsTrue(kIf == result1);
    StmtType result2 = stmt_table.GetStmtType(kStmtNum2);
    Assert::IsTrue(kAssign == result2);
    StmtType result3 = stmt_table.GetStmtType(kStmtNum3);
    Assert::IsTrue(kAssign == result3);
  }
};
}  // namespace PKBTests