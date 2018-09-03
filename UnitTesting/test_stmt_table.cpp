#include "CppUnitTest.h"
#include "stdafx.h"
#include "stmt_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestStmtTable) {
  const int kSuccess = 0;
  const int kFailure = -1;
  const int kSampleStmtIdx1 = 3;
  const int kSampleStmtIdx2 = 4;
  const string kSampleStmt1 = "a = b";
  const string kSampleStmt2 = "c = d";
  TEST_METHOD(TestInsertStmt) {
    StmtTable stmt_table;
    int result = stmt_table.InsertStmt(kSampleStmtIdx1, kSampleStmt1);
    Assert::AreEqual(kSuccess, result);
  }

  TEST_METHOD(TestInsertStmtExactDuplicate) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kSampleStmtIdx1, kSampleStmt1);
    int result = stmt_table.InsertStmt(kSampleStmtIdx1, kSampleStmt1);
    Assert::AreEqual(kFailure, result);
  }
  TEST_METHOD(TestInsertStmtDuplicate) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kSampleStmtIdx1, kSampleStmt1);
    int result = stmt_table.InsertStmt(kSampleStmtIdx1, kSampleStmt2);
    Assert::AreEqual(kFailure, result);
  }
  TEST_METHOD(TestGetAllStmtNumsSingle) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kSampleStmtIdx1, kSampleStmt1);
    std::vector<int> stmt_nums = stmt_table.GetAllStmtNums();
    Assert::AreEqual(kSampleStmtIdx1, stmt_nums.at(0));
  }
  TEST_METHOD(TestGetAllStmtNumsMultiple) {
    StmtTable stmt_table;
    stmt_table.InsertStmt(kSampleStmtIdx1, kSampleStmt1);
    stmt_table.InsertStmt(kSampleStmtIdx2, kSampleStmt1);
    std::vector<int> stmt_nums = stmt_table.GetAllStmtNums();
    Assert::AreEqual(kSampleStmtIdx1, stmt_nums.at(0));
    Assert::AreEqual(kSampleStmtIdx2, stmt_nums.at(1));
  }
};
}