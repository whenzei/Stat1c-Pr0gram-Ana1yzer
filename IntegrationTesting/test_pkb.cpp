#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {
TEST_CLASS(TestPKB) {
  const ProcName kProcName = "abc";
  const StmtNumInt kStmtNumInt1 = 1;
  const StmtNumInt kStmtNumInt2 = 2;
  const StmtNumInt kStmtNumInt3 = 3;
  const StmtNumInt kStmtNumInt4 = 4;
  const StmtNumInt kStmtNumInt5 = 5;
  const StmtNumInt kStmtNumInt6 = 6;
  const StmtNumInt kStmtNumInt7 = 7;
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";
  const StmtNum kStmtNum4 = "4";
  const StmtNum kStmtNum5 = "5";
  const StmtNum kStmtNum6 = "6";
  const StmtNum kStmtNum7 = "7";
  const StmtListIndex kStmtListIndex1 = 0;
  const StmtListIndex kStmtListIndex2 = 1;
  const StmtListIndex kStmtListIndex3 = 2;
  const StmtListIndex kStmtListIndex4 = 3;
  const StmtListIndex kStmtListIndex5 = 4;
  const VarName kVarName1 = "a";
  const VarName kVarName2 = "b";
  const VarName kVarName3 = "c";
  const ConstValue kConstValue1 = 12;
  const ConstValue kConstValue2 = 345;
  const VarNameSet kVarNameSet1 = {"b", "c"};
  const VarNameSet kVarNameSet2 = {"d", "e"};
  const VarNameSet kVarNameSet3 = {"f", "g"};
  const ConstValueSet kConstValueSet1 = {1, 2};
  const TokenList kTokenList = TokenList();

 public:
  TEST_METHOD(TestInsertProcName) {
    PKB pkb;
    bool result1 = pkb.InsertProcName(kProcName);
    Assert::IsTrue(result1);
    bool result2 = pkb.InsertProcName(kProcName);
    Assert::IsFalse(result2);
  }

  TEST_METHOD(TestGetAllProcName) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    Assert::IsTrue(pkb.GetAllProcName().size() == 1);
    Assert::AreEqual(kProcName, pkb.GetAllProcName().front());
  }

  TEST_METHOD(TestGetAllVarName) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, VarNameSet(),
                                         kConstValueSet1, kTokenList));
    VarNameList result1 = pkb.GetAllVarName();
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kVarName1, result1.front());
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1,
                                         kVarName2, kVarNameSet1,
                                         kConstValueSet1, kTokenList));
    VarNameList result2 = pkb.GetAllVarName();
    Assert::IsTrue(result2.size() == 3);
    Assert::AreEqual(kVarName1, result2.front());
    VarNameList::iterator iter = result2.begin();
    iter++;
    Assert::AreEqual(kVarName2, *iter);
    Assert::AreEqual(kVarName3, result2.back());
  }

  TEST_METHOD(TestInsertAndGetConstValue) {
    PKB pkb;
    pkb.InsertConstValue(kConstValue1);
    pkb.InsertConstValue(kConstValue2);
    ConstValueList result = pkb.GetAllConstValue();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(string("12"), result.front());
    Assert::AreEqual(string("345"), result.back());
  }

  TEST_METHOD(TestGetStmtType) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex2,
                                       kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex3, kVarNameSet1,
                                 kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex5, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt5, kStmtListIndex5, kVarName1));
    Assert::IsTrue(PqlDeclarationEntity::kAssign == pkb.GetStmtType(kStmtNum1));
    Assert::IsTrue(PqlDeclarationEntity::kWhile == pkb.GetStmtType(kStmtNum2));
    Assert::IsTrue(PqlDeclarationEntity::kIf == pkb.GetStmtType(kStmtNum3));
    Assert::IsTrue(PqlDeclarationEntity::kRead == pkb.GetStmtType(kStmtNum4));
    Assert::IsTrue(PqlDeclarationEntity::kPrint == pkb.GetStmtType(kStmtNum5));
  }

  TEST_METHOD(TestInsertAssignStmt) {
    PKB pkb;
    bool result1 = pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                        kVarName1, kVarNameSet1, kConstValueSet1, kTokenList));
    Assert::IsTrue(result1);
  }

  TEST_METHOD(TestGetAllAssignStmt) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1, kVarName2,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    StmtNumList result = pkb.GetAllAssignStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestInsertWhileStmt) {
    PKB pkb;
    bool result = pkb.InsertWhileStmt(&WhileStmtData(
        kStmtNumInt1, kStmtListIndex1, kVarNameSet1, kConstValueSet1));
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllWhileStmt) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1,
                                       kVarNameSet1, kConstValueSet1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex2,
                                       kVarNameSet1, kConstValueSet1));
    StmtNumList result = pkb.GetAllWhileStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestInsertIfStmt) {
    PKB pkb;
    bool result = pkb.InsertIfStmt(&IfStmtData(kStmtNumInt1, kStmtListIndex1,
                                               kVarNameSet1, kConstValueSet1));
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllIfStmt) {
    PKB pkb;
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt1, kStmtListIndex1, kVarNameSet1,
                                 kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kVarNameSet1,
                                 kConstValueSet1));
    StmtNumList result = pkb.GetAllIfStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestInsertReadStmt) {
    PKB pkb;
    bool result = pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllReadStmt) {
    PKB pkb;
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName2)
	);
    StmtNumList result = pkb.GetAllReadStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestInsertPrintStmt) {
    PKB pkb;
    bool result = pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllPrintStmt) {
    PKB pkb;
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt2, kStmtListIndex1, kVarName2));
    StmtNumList result = pkb.GetAllPrintStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllStmt) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1,
                                       kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex2, kVarNameSet1,
                                 kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex3, kVarName1));
    pkb.InsertPrintStmt(
        &PrintStmtData(kStmtNumInt5, kStmtListIndex3, kVarName2));
    StmtNumList result = pkb.GetAllStmt();
    Assert::IsTrue(result.size() == 5);
    Assert::AreEqual(kStmtNum1, result.front());
    StmtNumList::iterator iter = result.begin();
    iter++;
    Assert::AreEqual(kStmtNum2, *iter);
    iter++;
    Assert::AreEqual(kStmtNum3, *iter);
    iter++;
    Assert::AreEqual(kStmtNum4, *iter);
    iter++;
    Assert::AreEqual(kStmtNum5, *iter);
  }
};
}  // namespace PKBTests