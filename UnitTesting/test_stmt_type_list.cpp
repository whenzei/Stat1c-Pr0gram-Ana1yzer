#include "CppUnitTest.h"
#include "stdafx.h"
#include "stmt_type_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestStmtTypeList) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";

  TEST_METHOD(TestInsertAssignStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertAssignStmt(kStmtNum1);
    StmtNumList result1 = stmt_type_list.GetAllAssignStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertWhileStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertWhileStmt(kStmtNum1);
    StmtNumList result1 = stmt_type_list.GetAllWhileStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertIfStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertIfStmt(kStmtNum1);
    StmtNumList result1 = stmt_type_list.GetAllIfStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertReadStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertReadStmt(kStmtNum1);
    StmtNumList result1 = stmt_type_list.GetAllReadStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertPrintStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertPrintStmt(kStmtNum1);
    StmtNumList result1 = stmt_type_list.GetAllPrintStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertMultipleStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertAssignStmt(kStmtNum1);
    stmt_type_list.InsertWhileStmt(kStmtNum2);
    stmt_type_list.InsertAssignStmt(kStmtNum3);
    StmtNumList result1 = stmt_type_list.GetAllAssignStmt();
    StmtNumList result2 = stmt_type_list.GetAllWhileStmt();
    StmtNumList result3 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum3, result1.back());
    Assert::AreEqual(kStmtNum2, result2.front());
    Assert::AreEqual(kStmtNum1, result3.front());
    StmtNumList::iterator iter = result3.begin();
    iter++;
    Assert::AreEqual(kStmtNum2, *iter);
    Assert::AreEqual(kStmtNum3, result3.back());
  }
};
}  // namespace UnitTesting