#include "stdafx.h"
#include "CppUnitTest.h"
#include "stmt_type_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestStmtTypeList) {
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 2;
  const StmtNum kStmtNum3 = 3;

  TEST_METHOD(TestGetAllAssignStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kAssign);
    StmtNumSet result1 = stmt_type_list.GetAllAssignStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllAssignStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kAssign);
    StmtNumPairSet result1 = stmt_type_list.GetAllAssignStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kStmtNum1)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum2, kStmtNum2)));
  }

  TEST_METHOD(TestGetAllWhileStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kWhile);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kWhile);
    StmtNumSet result1 = stmt_type_list.GetAllWhileStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllWhileStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kWhile);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kWhile);
    StmtNumPairSet result1 = stmt_type_list.GetAllWhileStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kStmtNum1)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum2, kStmtNum2)));
  }

  TEST_METHOD(TestGetAllIfStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kIf);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kIf);
    StmtNumSet result1 = stmt_type_list.GetAllIfStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllIfStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kIf);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kIf);
    StmtNumPairSet result1 = stmt_type_list.GetAllIfStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kStmtNum1)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum2, kStmtNum2)));
  }

  TEST_METHOD(TestGetAllReadStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kRead);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kRead);
    StmtNumSet result1 = stmt_type_list.GetAllReadStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllReadStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kRead);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kRead);
    StmtNumPairSet result1 = stmt_type_list.GetAllReadStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kStmtNum1)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum2, kStmtNum2)));
  }

  TEST_METHOD(TestGetAllPrintStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kPrint);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kPrint);
    StmtNumSet result1 = stmt_type_list.GetAllPrintStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllPrintStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kPrint);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kPrint);
    StmtNumPairSet result1 = stmt_type_list.GetAllPrintStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kStmtNum1)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum2, kStmtNum2)));
  }

  TEST_METHOD(TestGetAllCallStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kCall);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kCall);
    StmtNumSet result1 = stmt_type_list.GetAllCallStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllCallStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kCall);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kCall);
    StmtNumPairSet result1 = stmt_type_list.GetAllCallStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kStmtNum1)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum2, kStmtNum2)));
  }

  TEST_METHOD(TestGetAllStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kWhile);
    stmt_type_list.InsertStmt(kStmtNum3, StmtType::kPrint);
    StmtNumSet result1 = stmt_type_list.GetAllStmt();
    Assert::IsTrue(result1.size() == 3);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum2));
    Assert::IsTrue(result1.count(kStmtNum3));
  }

  TEST_METHOD(TestGetAllStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, StmtType::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, StmtType::kWhile);
    stmt_type_list.InsertStmt(kStmtNum3, StmtType::kPrint);
    StmtNumPairSet result1 = stmt_type_list.GetAllStmtTwin();
    Assert::IsTrue(result1.size() == 3);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kStmtNum1)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum2, kStmtNum2)));
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum3, kStmtNum3)));
  }
};
}  // namespace PKBTests