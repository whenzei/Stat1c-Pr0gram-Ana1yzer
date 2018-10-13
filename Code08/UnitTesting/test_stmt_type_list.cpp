#include "stdafx.h" 
#include "CppUnitTest.h"
#include "stmt_type_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestStmtTypeList) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";

  TEST_METHOD(TestGetAllAssignStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kAssign);
    StmtNumList result1 = stmt_type_list.GetAllAssignStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestGetAllAssignStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kAssign);
    StmtNumPairList result1 = stmt_type_list.GetAllAssignStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kStmtNum1, result1.front().second);
    Assert::AreEqual(kStmtNum2, result1.back().first);
    Assert::AreEqual(kStmtNum2, result1.back().second);
  }

  TEST_METHOD(TestGetAllWhileStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kWhile);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kWhile);
    StmtNumList result1 = stmt_type_list.GetAllWhileStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestGetAllWhileStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kWhile);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kWhile);
    StmtNumPairList result1 = stmt_type_list.GetAllWhileStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kStmtNum1, result1.front().second);
    Assert::AreEqual(kStmtNum2, result1.back().first);
    Assert::AreEqual(kStmtNum2, result1.back().second);
  }

  TEST_METHOD(TestGetAllIfStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kIf);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kIf);
    StmtNumList result1 = stmt_type_list.GetAllIfStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestGetAllIfStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kIf);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kIf);
    StmtNumPairList result1 = stmt_type_list.GetAllIfStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kStmtNum1, result1.front().second);
    Assert::AreEqual(kStmtNum2, result1.back().first);
    Assert::AreEqual(kStmtNum2, result1.back().second);
  }

  TEST_METHOD(TestGetAllReadStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kRead);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kRead);
    StmtNumList result1 = stmt_type_list.GetAllReadStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestGetAllReadStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kRead);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kRead);
    StmtNumPairList result1 = stmt_type_list.GetAllReadStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kStmtNum1, result1.front().second);
    Assert::AreEqual(kStmtNum2, result1.back().first);
    Assert::AreEqual(kStmtNum2, result1.back().second);
  }

 TEST_METHOD(TestGetAllPrintStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kPrint);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kPrint);
    StmtNumList result1 = stmt_type_list.GetAllPrintStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestGetAllPrintStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kPrint);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kPrint);
    StmtNumPairList result1 = stmt_type_list.GetAllPrintStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kStmtNum1, result1.front().second);
    Assert::AreEqual(kStmtNum2, result1.back().first);
    Assert::AreEqual(kStmtNum2, result1.back().second);
  }

  TEST_METHOD(TestGetAllCallStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kCall);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kCall);
    StmtNumList result1 = stmt_type_list.GetAllCallStmt();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestGetAllCallStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kCall);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kCall);
    StmtNumPairList result1 = stmt_type_list.GetAllCallStmtTwin();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kStmtNum1, result1.front().second);
    Assert::AreEqual(kStmtNum2, result1.back().first);
    Assert::AreEqual(kStmtNum2, result1.back().second);
  }

  TEST_METHOD(TestGetAllStmt) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kWhile);
    stmt_type_list.InsertStmt(kStmtNum3, PqlDeclarationEntity::kPrint);
    StmtNumList result1 = stmt_type_list.GetAllStmt();
    Assert::IsTrue(result1.size() == 3);
    StmtNumList::iterator iter = result1.begin();
    Assert::AreEqual(kStmtNum1, *iter);
    iter++;
    Assert::AreEqual(kStmtNum2, *iter);
    iter++;
    Assert::AreEqual(kStmtNum3, *iter);
  }

  TEST_METHOD(TestGetAllStmtTwin) {
    StmtTypeList stmt_type_list;
    stmt_type_list.InsertStmt(kStmtNum1, PqlDeclarationEntity::kAssign);
    stmt_type_list.InsertStmt(kStmtNum2, PqlDeclarationEntity::kWhile);
    stmt_type_list.InsertStmt(kStmtNum3, PqlDeclarationEntity::kPrint);
    StmtNumPairList result1 = stmt_type_list.GetAllStmtTwin();
    Assert::IsTrue(result1.size() == 3);
    StmtNumPairList::iterator iter = result1.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kStmtNum1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kStmtNum2, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kStmtNum3, (*iter).second);
  }
};
}  // namespace PKBTests