#include "stdafx.h" 
#include "CppUnitTest.h"
#include "stmtlist_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestStmtListTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";
  const StmtListIndex kStmtListIndex1 = 0;
  const StmtListIndex kStmtListIndex2 = 1;
  const StmtListIndex kStmtListIndex3 = 1;

  TEST_METHOD(TestInsertSingle) {
    StmtListTable stmtlist_table;
    stmtlist_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    StmtNumList result1 = stmtlist_table.GetStmtNumList(kStmtListIndex1);
    Assert::AreEqual(kStmtNum1, result1.front());
  }
  
  TEST_METHOD(TestInsertToSameStmtList) {
    StmtListTable stmtlist_table;
    stmtlist_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    stmtlist_table.InsertStmt(kStmtNum2, kStmtListIndex1);
    StmtNumList result = stmtlist_table.GetStmtNumList(kStmtListIndex1);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }
  
  TEST_METHOD(TestInsertToDifferentStmtList) {
    StmtListTable stmtlist_table;
    stmtlist_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    stmtlist_table.InsertStmt(kStmtNum2, kStmtListIndex2);
    StmtNumList result1 = stmtlist_table.GetStmtNumList(kStmtListIndex1);
    StmtNumList result2 = stmtlist_table.GetStmtNumList(kStmtListIndex2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result2.front());
  }
  
  TEST_METHOD(TestInsertMultiple) {
    StmtListTable stmtlist_table;
    stmtlist_table.InsertStmt(kStmtNum1, kStmtListIndex1);
    stmtlist_table.InsertStmt(kStmtNum2, kStmtListIndex2);
    stmtlist_table.InsertStmt(kStmtNum3, kStmtListIndex3);
    StmtNumList result1 = stmtlist_table.GetStmtNumList(kStmtListIndex1);
    StmtNumList result2 = stmtlist_table.GetStmtNumList(kStmtListIndex2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result2.front());
    Assert::AreEqual(kStmtNum3, result2.back());
  }

};
}  // namespace UnitTesting