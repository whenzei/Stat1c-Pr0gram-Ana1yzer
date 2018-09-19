#include "CppUnitTest.h"
#include "stdafx.h"
#include "stmt_type_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {
using ste = StmtTypeList::StmtTypeEnum;

TEST_CLASS(TestStmtTypeList) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";

  TEST_METHOD(TestInsertAssignStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, ste::kAssign);
    StmtNumList result1 = stmt_type_list.GetAllAssignStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertWhileStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, ste::kWhile);
    StmtNumList result1 = stmt_type_list.GetAllWhileStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertIfStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, ste::kIf);
    StmtNumList result1 = stmt_type_list.GetAllIfStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertReadStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, ste::kRead);
    StmtNumList result1 = stmt_type_list.GetAllReadStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertPrintStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, ste::kPrint);
    StmtNumList result1 = stmt_type_list.GetAllPrintStmt();
    StmtNumList result2 = stmt_type_list.GetAllStmt();
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum1, result2.front());
  }

  TEST_METHOD(TestInsertMultipleStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, ste::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, ste::kWhile);
    stmt_type_list.InsertStmt(kStmtNum3, ste::kAssign);
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
}  // namespace PKBTests