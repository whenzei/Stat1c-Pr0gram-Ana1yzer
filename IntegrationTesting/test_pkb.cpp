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
                                       kStmtListIndex3, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex3, kStmtListIndex4,
                     kStmtListIndex5, kVarNameSet1, kConstValueSet1));
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
    bool result = pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1,
                                      kStmtListIndex2, kVarNameSet1, kConstValueSet1));
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllWhileStmt) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                        kVarNameSet1, kConstValueSet1));
    StmtNumList result = pkb.GetAllWhileStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestInsertIfStmt) {
    PKB pkb;
    bool result =
        pkb.InsertIfStmt(&IfStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                         kStmtListIndex3, kVarNameSet1, kConstValueSet1));
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllIfStmt) {
    PKB pkb;
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                     kStmtListIndex3, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex4,
                     kStmtListIndex5, kVarNameSet1, kConstValueSet1));
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
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex3, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt5, kStmtListIndex3, kVarName2));
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

  TEST_METHOD(TestIsParent) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    Assert::IsTrue(pkb.IsParent(kStmtNum1, kStmtNum2));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    Assert::IsTrue(pkb.IsParent(kStmtNum2, kStmtNum3));
    Assert::IsTrue(pkb.IsParent(kStmtNum2, kStmtNum4));
    Assert::IsFalse(pkb.IsParent(kStmtNum1, kStmtNum3));
    Assert::IsFalse(pkb.IsParent(kStmtNum1, kStmtNum4));
  }

  TEST_METHOD(TestIsParentT) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    Assert::IsTrue(pkb.IsParentT(kStmtNum1, kStmtNum2));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    Assert::IsTrue(pkb.IsParentT(kStmtNum2, kStmtNum3));
    Assert::IsTrue(pkb.IsParentT(kStmtNum2, kStmtNum4));
    Assert::IsTrue(pkb.IsParentT(kStmtNum1, kStmtNum3));
    Assert::IsTrue(pkb.IsParentT(kStmtNum1, kStmtNum4));
  }

  TEST_METHOD(TestGetParent) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    StmtNumList result1 = pkb.GetParent(kStmtNum1);
    Assert::IsTrue(result1.size() == 0);
    StmtNumList result2 = pkb.GetParent(kStmtNum2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    StmtNumList result3 = pkb.GetParent(kStmtNum3);
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum2, result3.front());
    StmtNumList result4 = pkb.GetParent(kStmtNum4);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }

  TEST_METHOD(TestGetParentT) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    StmtNumList result1 = pkb.GetParentT(kStmtNum1);
    Assert::IsTrue(result1.size() == 0);
    StmtNumList result2 = pkb.GetParentT(kStmtNum2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    StmtNumList result3 = pkb.GetParentT(kStmtNum3);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum2, result3.front());
    Assert::AreEqual(kStmtNum1, result3.back());
    StmtNumList result4 = pkb.GetParentT(kStmtNum4);
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kStmtNum2, result4.front());
    Assert::AreEqual(kStmtNum1, result4.back());
  }

  TEST_METHOD(TestGetAllParent) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    StmtNumList result = pkb.GetAllParent();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetChild) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    StmtNumList result1 = pkb.GetChild(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum2, result1.front());
    StmtNumList result2 = pkb.GetChild(kStmtNum2);
    Assert::IsTrue(result2.size() == 0);
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    result2 = pkb.GetChild(kStmtNum2);
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum3, result2.front());
    Assert::AreEqual(kStmtNum4, result2.back());
    StmtNumList result3 = pkb.GetChild(kStmtNum3);
    Assert::IsTrue(result3.size() == 0);
    StmtNumList result4 = pkb.GetChild(kStmtNum4);
    Assert::IsTrue(result3.size() == 0);
  }

  TEST_METHOD(TestGetChildT) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    StmtNumList result1 = pkb.GetChildT(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum2, result1.front());
    StmtNumList result2 = pkb.GetChildT(kStmtNum2);
    Assert::IsTrue(result2.size() == 0);
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    result2 = pkb.GetChildT(kStmtNum2);
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum3, result2.front());
    Assert::AreEqual(kStmtNum4, result2.back());
    StmtNumList result3 = pkb.GetChildT(kStmtNum3);
    Assert::IsTrue(result3.size() == 0);
    StmtNumList result4 = pkb.GetChildT(kStmtNum4);
    Assert::IsTrue(result3.size() == 0);
    result1 = pkb.GetChildT(kStmtNum1);
    Assert::IsTrue(result1.size() == 3);
    Assert::AreEqual(kStmtNum2, result1.front());
    StmtNumList::iterator iter = result1.begin();
    iter++;
    Assert::AreEqual(kStmtNum3, *iter);
    Assert::AreEqual(kStmtNum4, result1.back());
  }

  TEST_METHOD(TestGetAllChild) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    StmtNumList result = pkb.GetAllChild();
    Assert::IsTrue(result.size() == 3);
    Assert::AreEqual(kStmtNum2, result.front());
    StmtNumList::iterator iter = result.begin();
    iter++;
    Assert::AreEqual(kStmtNum3, *iter);
    Assert::AreEqual(kStmtNum4, result.back());
  }

  TEST_METHOD(TestHasParentRelationship) {
    PKB pkb;
    Assert::IsFalse(pkb.HasParentRelationship());
    pkb.GetParentT(kStmtNum1);
    Assert::IsFalse(pkb.HasParentRelationship());
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1));
    Assert::IsTrue(pkb.HasParentRelationship());
  }

  TEST_METHOD(TestGetAllParentPair) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    StmtNumPairList result = pkb.GetAllParentPair();
    Assert::IsTrue(result.size() == 3);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kStmtNum2, result.front().second);
    StmtNumPairList::iterator iter = result.begin();
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kStmtNum3, (*iter).second);
    Assert::AreEqual(kStmtNum2, result.back().first);
    Assert::AreEqual(kStmtNum4, result.back().second);
  }

  TEST_METHOD(TestGetAllParentTPair) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt2, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex3, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt4, kStmtListIndex4, kVarName2));
    StmtNumPairList result = pkb.GetAllParentTPair();
    Assert::IsTrue(result.size() == 5);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kStmtNum2, result.front().second);
    StmtNumPairList::iterator iter = result.begin();
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kStmtNum3, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kStmtNum3, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kStmtNum4, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kStmtNum4, (*iter).second);
  }

  TEST_METHOD(TestIsModifiedByS) {
    PKB pkb;
    Assert::IsFalse(pkb.IsModifiedByS(kStmtNum1, kVarName1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    Assert::IsTrue(pkb.IsModifiedByS(kStmtNum1, kVarName1));
  }

  TEST_METHOD(TestIsModifiedByP) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    Assert::IsFalse(pkb.IsModifiedByP(kProcName, kVarName1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    Assert::IsTrue(pkb.IsModifiedByP(kProcName, kVarName1));
    Assert::IsFalse(pkb.IsModifiedByP(kProcName, kVarName2));
  }

  TEST_METHOD(TestGetModifiedVarS) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex3, kVarName2,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt5, kStmtListIndex4, kVarName1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    VarNameList result4 = pkb.GetModifiedVarS(kStmtNum4);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kVarName2, result4.front());
    VarNameList result5 = pkb.GetModifiedVarS(kStmtNum5);
    Assert::IsTrue(result5.size() == 1);
    Assert::AreEqual(kVarName1, result5.front());
    VarNameList result6 = pkb.GetModifiedVarS(kStmtNum3);
    Assert::IsTrue(result6.size() == 2);
    Assert::AreEqual(kVarName2, result6.front());
    Assert::AreEqual(kVarName1, result6.back());
    VarNameList result7 = pkb.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result7.size() == 2);
    Assert::AreEqual(kVarName1, result7.front());
    Assert::AreEqual(kVarName2, result7.back());
  }

  TEST_METHOD(TestGetModifiedVarP) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    VarNameList result1 = pkb.GetModifiedVarP(kProcName);
    Assert::IsTrue(result1.size() == 0);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    VarNameList result2 = pkb.GetModifiedVarP(kProcName);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kVarName1, result2.front());
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex2, kVarName1));
    VarNameList result3 = pkb.GetModifiedVarP(kProcName);
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kVarName1, result3.front());
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName2,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    VarNameList result4 = pkb.GetModifiedVarP(kProcName);
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kVarName1, result4.front());
    Assert::AreEqual(kVarName2, result4.back());
    VarNameList result5 = pkb.GetModifiedVarP("wrong name");
    Assert::IsTrue(result5.size() == 0);
  }

  TEST_METHOD(TestGetAllModifiedVar) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    VarNameList result1 = pkb.GetAllModifiedVar();
    Assert::IsTrue(result1.size() == 0);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    VarNameList result2 = pkb.GetAllModifiedVar();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kVarName1, result2.front());
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt3, kStmtListIndex2, kVarName1));
    VarNameList result3 = pkb.GetAllModifiedVar();
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kVarName1, result3.front());
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName2,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    VarNameList result4 = pkb.GetAllModifiedVar();
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kVarName1, result4.front());
    Assert::AreEqual(kVarName2, result4.back());
  }

  TEST_METHOD(TestGetModifyingS) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex3, kVarName2,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt5, kStmtListIndex4, kVarName1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    StmtNumList result3 = pkb.GetModifyingS(kVarName1);
    Assert::IsTrue(result3.size() == 4);
    StmtNumList::iterator iter1 = result3.begin();
    Assert::AreEqual(kStmtNum2, *iter1);
    iter1++;
    Assert::AreEqual(kStmtNum5, *iter1);
    iter1++;
    Assert::AreEqual(kStmtNum3, *iter1);
    iter1++;
    Assert::AreEqual(kStmtNum1, *iter1);
    StmtNumList result4 = pkb.GetModifyingS(kVarName2);
    Assert::IsTrue(result4.size() == 3);
    StmtNumList::iterator iter2 = result4.begin();
    Assert::AreEqual(kStmtNum4, *iter2);
    iter2++;
    Assert::AreEqual(kStmtNum3, *iter2);
    iter2++;
    Assert::AreEqual(kStmtNum1, *iter2);
  }

  TEST_METHOD(TestGetModifyingP) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    ProcNameList result1 = pkb.GetModifyingP(kVarName1);
    Assert::IsTrue(result1.size() == 0);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    ProcNameList result2 = pkb.GetModifyingP(kVarName1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcName, result2.front());
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex3, kVarName2,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt5, kStmtListIndex4, kVarName1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    ProcNameList result3 = pkb.GetModifyingP(kVarName1);
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kProcName, result3.front());
    ProcNameList result4 = pkb.GetModifyingP(kVarName2);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kProcName, result4.front());
    ProcNameList result5 = pkb.GetModifyingP(kVarName3);
    Assert::IsTrue(result5.size() == 0);
  }

  TEST_METHOD(TestHasModifiesRelationship) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    Assert::IsFalse(pkb.HasModifiesRelationship());
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    Assert::IsTrue(pkb.HasModifiesRelationship());
  }

  TEST_METHOD(TestGetAllModifiesPairS) {
    PKB pkb;
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    StmtVarPairList result1 = pkb.GetAllModifiesPairS();
    Assert::IsTrue(result1.size() == 0);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    StmtVarPairList result2 = pkb.GetAllModifiesPairS();
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum2, result2.front().first);
    Assert::AreEqual(kVarName1, result2.front().second);
    Assert::AreEqual(kStmtNum1, result2.back().first);
    Assert::AreEqual(kVarName1, result2.back().second);
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex3, kVarName2,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt5, kStmtListIndex4, kVarName1));
    StmtVarPairList result3 = pkb.GetAllModifiesPairS();
    Assert::IsTrue(result3.size() == 7);
    StmtVarPairList::iterator iter = result3.begin();
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum4, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum5, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
  }

  TEST_METHOD(TestGetAllModifiesPairP) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt1, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    ProcVarPairList result1 = pkb.GetAllModifiesPairP();
    Assert::IsTrue(result1.size() == 0);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex2, kVarName1,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    ProcVarPairList result2 = pkb.GetAllModifiesPairP();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kProcName, result2.front().first);
    Assert::AreEqual(kVarName1, result2.front().second);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex3, kVarName2,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt5, kStmtListIndex4, kVarName1));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt3, kStmtListIndex2, kStmtListIndex3,
                     kStmtListIndex4, kVarNameSet1, kConstValueSet1));
    ProcVarPairList result3 = pkb.GetAllModifiesPairP();
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kProcName, result3.front().first);
    Assert::AreEqual(kVarName1, result3.front().second);
    Assert::AreEqual(kProcName, result3.back().first);
    Assert::AreEqual(kVarName2, result3.back().second);
  }

  TEST_METHOD(TestIsFollowsT) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    Assert::IsTrue(pkb.IsFollowsT(kStmtNum1, kStmtNum2));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    Assert::IsTrue(pkb.IsFollowsT(kStmtNum1, kStmtNum3));
  }

  TEST_METHOD(TestIsFollows) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    Assert::IsTrue(pkb.IsFollows(kStmtNum1, kStmtNum2));
    Assert::IsFalse(pkb.IsFollows(kStmtNum1, kStmtNum3));
    Assert::IsFalse(pkb.IsFollows(kStmtNum3, kStmtNum4));
  }

  TEST_METHOD(TestGetFollowsT) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    StmtList followers = pkb.GetFollowsT(kStmtNum1);
    Assert::IsTrue(followers.size() == 2);
    StmtList expected_followers;
    expected_followers.push_back(kStmtNum2);
    expected_followers.push_back(kStmtNum3);
    Assert::AreEqual(expected_followers.front(), followers.front());
    Assert::AreEqual(expected_followers.back(), followers.back());
  }

  TEST_METHOD(TestGetFollows) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    StmtList followers = pkb.GetFollows(kStmtNum1);
    Assert::IsTrue(followers.size() == 1);
    StmtList expected_followers;
    expected_followers.push_back(kStmtNum2);
    Assert::AreEqual(expected_followers.front(), followers.front());
  }

  TEST_METHOD(TestGetAllFollows) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    StmtList all_followers = pkb.GetAllFollows();
    StmtList expected_followers;
    expected_followers.push_back(kStmtNum2);
    expected_followers.push_back(kStmtNum3);
    Assert::AreEqual(expected_followers.front(), all_followers.front());
    Assert::AreEqual(expected_followers.back(), all_followers.back());
  }

  TEST_METHOD(TestGetFollowedByT) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    StmtList followees = pkb.GetFollowedByT(kStmtNum3);
    Assert::IsTrue(followees.size() == 2);
    StmtList expected_followees;
    expected_followees.push_back(kStmtNum2);
    expected_followees.push_back(kStmtNum1);
    Assert::AreEqual(expected_followees.front(), followees.front());
    Assert::AreEqual(expected_followees.back(), followees.back());
  }

  TEST_METHOD(TestGetFollowedBy) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    StmtList followees = pkb.GetFollowedBy(kStmtNum3);
    Assert::IsTrue(followees.size() == 1);
    StmtList expected_followee;
    expected_followee.push_back(kStmtNum2);
    Assert::AreEqual(expected_followee.front(), followees.front());
  }

  TEST_METHOD(TestGetAllFollowedBy) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt5, kStmtListIndex2, kVarName1));
    StmtList all_followees = pkb.GetAllFollowedBy();
    StmtList expected_followees;
    expected_followees.push_back(kStmtNum1);
    expected_followees.push_back(kStmtNum2);
    expected_followees.push_back(kStmtNum4);
    StmtList::iterator iter_1 = all_followees.begin();
    StmtList::iterator iter_2 = expected_followees.begin();
    Assert::AreEqual(*iter_2, *iter_1);
    iter_1++;
    iter_2++;
    Assert::AreEqual(*iter_2, *iter_1);
    iter_1++;
    iter_2++;
    Assert::AreEqual(*iter_2, *iter_1);
  }

  TEST_METHOD(TestHasFollowsRelationship) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    bool result = pkb.HasFollowsRelationship();
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllFollowsTPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    StmtNumPairList all_pairs = pkb.GetAllFollowsTPair();
    Assert::IsTrue(all_pairs.size() == 3);
    StmtNumPairList expected_list;
    expected_list.push_back(make_pair(kStmtNum1, kStmtNum2));
    expected_list.push_back(make_pair(kStmtNum1, kStmtNum3));
    expected_list.push_back(make_pair(kStmtNum2, kStmtNum3));
    StmtNumPairList::iterator iter_1 = all_pairs.begin();
    StmtNumPairList::iterator iter_2 = expected_list.begin();
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
    iter_1++;
    iter_2++;
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
    iter_1++;
    iter_2++;
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
  }

  TEST_METHOD(TestGetAllFollowsPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt3, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt4, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    StmtNumPairList all_pairs = pkb.GetAllFollowsPair();
    Assert::IsTrue(all_pairs.size() == 2);
    StmtNumPairList expected_list;
    expected_list.push_back(make_pair(kStmtNum1, kStmtNum2));
    expected_list.push_back(make_pair(kStmtNum2, kStmtNum3));
    StmtNumPairList::iterator iter_1 = all_pairs.begin();
    StmtNumPairList::iterator iter_2 = expected_list.begin();
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
    iter_1++;
    iter_2++;
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
  }

  TEST_METHOD(TestGetAllUsedVar) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    VarNameList used_vars = pkb.GetAllUsedVar();
    Assert::IsTrue(used_vars.size() == 2);
    Assert::AreEqual(kVarName2, used_vars.front());
    Assert::AreEqual(kVarName3, used_vars.back());
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    used_vars = pkb.GetAllUsedVar();
    Assert::IsTrue(used_vars.size() == 3);
    Assert::AreEqual(kVarName1, used_vars.back());
  }

  TEST_METHOD(TestGetUsedVarS) {
    PKB pkb;
    // Double nested uses
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt4, kStmtListIndex2, kStmtListIndex2,
                     kStmtListIndex3, kVarNameSet3, kConstValueSet1));
    VarNameList used_vars = pkb.GetUsedVarS(kStmtNum2);
    Assert::IsTrue(used_vars.size() == 6);
  }

  TEST_METHOD(TestGetUsedVarP) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    VarNameList used_vars = pkb.GetUsedVarP(kProcName);
    Assert::IsTrue(used_vars.size() == 4);
  }

  TEST_METHOD(TestGetAllUsingStmt) {
    PKB pkb;
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    pkb.InsertIfStmt(&IfStmtData(kStmtNumInt4, kStmtListIndex2, kStmtListIndex2,
                     kStmtListIndex3, kVarNameSet2, kConstValueSet1));
    StmtList using_stmts = pkb.GetAllUsingStmt();
    StmtList expected_list;
    expected_list.push_back(kStmtNum2);
    expected_list.push_back(kStmtNum3);
    expected_list.push_back(kStmtNum4);
    StmtList::iterator iter_1 = using_stmts.begin();
    StmtList::iterator iter_2 = expected_list.begin();
    Assert::AreEqual(*iter_2, *iter_1);
    iter_1++;
    iter_2++;
    Assert::AreEqual(*iter_2, *iter_1);
    iter_1++;
    iter_2++;
    Assert::AreEqual(*iter_2, *iter_1);
  }

  TEST_METHOD(TestGetAllUsingProc) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    ProcNameList using_proc = pkb.GetAllUsingProc();
    Assert::IsTrue(using_proc.size() == 1);
    Assert::AreEqual(kProcName, using_proc.front());
  }

  TEST_METHOD(TestGetUsingStmt) {
    PKB pkb;
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName2));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet1, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex2, kVarName2,
                         kVarNameSet2, kConstValueSet1, kTokenList));
    StmtList using_stmt = pkb.GetUsingStmt(kVarName2);
    Assert::AreEqual(kStmtNum2, using_stmt.front());
  }

  TEST_METHOD(TestGetUsingProc) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    ProcNameList using_proc = pkb.GetUsingProc(kVarName1);
    Assert::IsTrue(using_proc.size() == 1);
    Assert::AreEqual(kProcName, using_proc.front());
  }

  TEST_METHOD(TestIsUsedByS) {
    PKB pkb;
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    bool result_1 = pkb.IsUsedByS(kStmtNum1, kVarName1);
    Assert::IsTrue(result_1);
    bool result_2 = pkb.IsUsedByS(kStmtNum2, kVarName1);
    Assert::IsFalse(result_2);
  }

  TEST_METHOD(TestIsUsedByP) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex2, kVarName1,
                         kVarNameSet1, kConstValueSet1, kTokenList));
    bool result = pkb.IsUsedByP(kProcName, kVarName2);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestHasUsesRelationship) {
    PKB pkb;
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    bool result = pkb.HasUsesRelationship();
    Assert::IsFalse(result);
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt2, kStmtListIndex1, kVarName1));
    result = pkb.HasUsesRelationship();
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllUsesPairS) {
    PKB pkb;
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    StmtVarPairList expected_list;
    StmtVarPairList uses_pair_list = pkb.GetAllUsesPairS();
    expected_list.push_back(make_pair(kStmtNum1, kVarName1));
    for (auto& var : kVarNameSet2) {
      expected_list.push_back(make_pair(kStmtNum2, var));
    }
    Assert::IsTrue(uses_pair_list.size() == 3);
    StmtNumPairList::iterator iter_1 = uses_pair_list.begin();
    StmtNumPairList::iterator iter_2 = expected_list.begin();
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
    iter_1++;
    iter_2++;
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
    iter_1++;
    iter_2++;
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
  }

  TEST_METHOD(TestGetAllUsesPairP) {
    PKB pkb;
    pkb.InsertProcName(kProcName);
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertWhileStmt(&WhileStmtData(kStmtNumInt2, kStmtListIndex1, kStmtListIndex2,
                        kVarNameSet2, kConstValueSet1));
    StmtVarPairList expected_list;
    StmtVarPairList uses_pair_list = pkb.GetAllUsesPairP();
    expected_list.push_back(make_pair(kProcName, kVarName1));
    for (auto& var : kVarNameSet2) {
      expected_list.push_back(make_pair(kProcName, var));
    }
    Assert::IsTrue(uses_pair_list.size() == 3);
    StmtNumPairList::iterator iter_1 = uses_pair_list.begin();
    StmtNumPairList::iterator iter_2 = expected_list.begin();
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
    iter_1++;
    iter_2++;
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
    iter_1++;
    iter_2++;
    Assert::AreEqual((*iter_2).first, (*iter_1).first);
    Assert::AreEqual((*iter_2).second, (*iter_1).second);
  }
};
}  // namespace PKBTests