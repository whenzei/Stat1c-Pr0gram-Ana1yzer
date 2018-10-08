#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using TokenType = Tokenizer::TokenType;

namespace PKBTests {
TEST_CLASS(TestPKB) {
  const ProcName kProcName1 = "one";
  const ProcName kProcName2 = "two";
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
  const Token kToken32 = {TokenType::kDigit, "32"};
  const Token kTokenA = {TokenType::kName, "a"};
  const Token kTokenPlus = {TokenType::kOperator, "+"};
  const Token kTokenB = {TokenType::kName, "b"};
  const Token kTokenMult = {TokenType::kOperator, "*"};
  const TokenList kTokenList = TokenList();
  const TokenList kTokenList1 = {kToken32, kTokenA, kTokenPlus};
  const TokenList kTokenList2 = {kToken32, kTokenA, kTokenPlus, kTokenB,
                                 kTokenMult};
  const TokenList kTokenList3 = {kToken32};

 public:
  TEST_METHOD(TestGetAllProcName) {
    PKB pkb;
    pkb.InsertProcName(kProcName1);
    pkb.InsertProcName(kProcName2);
    ProcNameList proc_names = pkb.GetAllProcName();
    Assert::IsTrue(proc_names.size() == 2);
    Assert::AreEqual(proc_names[0], kProcName1);
    Assert::AreEqual(proc_names[1], kProcName2);
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

  TEST_METHOD(TestGetAllConstValue) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, VarNameSet(),
                                         kConstValueSet1, kTokenList));
    ConstValueList result = pkb.GetAllConstValue();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(string("1"), result.front());
    Assert::AreEqual(string("2"), result.back());
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
    pkb.InsertPrintStmt(
        &PrintStmtData(kStmtNumInt5, kStmtListIndex5, kVarName1));
    pkb.InsertCallStmt(
        &CallStmtData(kStmtNumInt6, kStmtListIndex5, kProcName1, kProcName2));
    Assert::IsTrue(PqlDeclarationEntity::kAssign == pkb.GetStmtType(kStmtNum1));
    Assert::IsTrue(PqlDeclarationEntity::kWhile == pkb.GetStmtType(kStmtNum2));
    Assert::IsTrue(PqlDeclarationEntity::kIf == pkb.GetStmtType(kStmtNum3));
    Assert::IsTrue(PqlDeclarationEntity::kRead == pkb.GetStmtType(kStmtNum4));
    Assert::IsTrue(PqlDeclarationEntity::kPrint == pkb.GetStmtType(kStmtNum5));
    Assert::IsTrue(PqlDeclarationEntity::kCall == pkb.GetStmtType(kStmtNum6));
  }

  TEST_METHOD(TestGetAllAssignStmt) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1,
                                         kVarName2, kVarNameSet2,
                                         kConstValueSet1, kTokenList));
    StmtNumList result = pkb.GetAllAssignStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
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

  TEST_METHOD(TestGetAllReadStmt) {
    PKB pkb;
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNumInt2, kStmtListIndex1, kVarName2));
    StmtNumList result = pkb.GetAllReadStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllPrintStmt) {
    PKB pkb;
    pkb.InsertPrintStmt(
        &PrintStmtData(kStmtNumInt1, kStmtListIndex1, kVarName1));
    pkb.InsertPrintStmt(
        &PrintStmtData(kStmtNumInt2, kStmtListIndex1, kVarName2));
    StmtNumList result = pkb.GetAllPrintStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllCallStmt) {
    PKB pkb;
    pkb.InsertCallStmt(
<<<<<<< Code08/IntegrationTesting/test_pkb.cpp
        &CallStmtData(kStmtNumInt1, kStmtListIndex1, "one", "two"));
    pkb.InsertCallStmt(
        &CallStmtData(kStmtNumInt2, kStmtListIndex1, "one", "three"));
=======
      &CallStmtData(kStmtNumInt1, kStmtListIndex1, "one", "two"));
    pkb.InsertCallStmt(
      &CallStmtData(kStmtNumInt2, kStmtListIndex1, "one", "three"));
>>>>>>> Code08/IntegrationTesting/test_pkb.cpp
    StmtNumList result = pkb.GetAllCallStmt();
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

  TEST_METHOD(TestGetAssignWithPattern) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1,
                                         kVarName2, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList2));
    StmtNumList result1 = pkb.GetAssignWithPattern(kVarName1, {kToken32});
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum3, result1.back());
    StmtNumList result2 = pkb.GetAssignWithPattern(kVarName2, kTokenList);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum2, result2.front());
    StmtNumList result3 = pkb.GetAssignWithPattern("", {kTokenA});
    Assert::IsTrue(result3.size() == 3);
    StmtNumList::iterator iter1 = result3.begin();
    Assert::AreEqual(kStmtNum1, *iter1);
    iter1++;
    Assert::AreEqual(kStmtNum2, *iter1);
    iter1++;
    Assert::AreEqual(kStmtNum3, *iter1);
    StmtNumList result4 = pkb.GetAssignWithPattern("", kTokenList);
    Assert::IsTrue(result4.size() == 3);
    StmtNumList::iterator iter2 = result4.begin();
    Assert::AreEqual(kStmtNum1, *iter2);
    iter2++;
    Assert::AreEqual(kStmtNum2, *iter2);
    iter2++;
    Assert::AreEqual(kStmtNum3, *iter2);
  }

  TEST_METHOD(TestGetAssignWithExactPattern) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1,
                                         kVarName2, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList2));
    StmtNumList result1 = pkb.GetAssignWithExactPattern(kVarName1, kTokenList1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result2 = pkb.GetAssignWithExactPattern(kVarName1, {kTokenA});
    Assert::IsTrue(result2.empty());
    StmtNumList result3 = pkb.GetAssignWithExactPattern("", kTokenList1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
    StmtNumList result4 = pkb.GetAssignWithExactPattern("", kTokenList2);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum3, result4.front());
    StmtNumList result5 = pkb.GetAssignWithExactPattern(kVarName1, kTokenList);
    Assert::IsTrue(result5.empty());
  }

  TEST_METHOD(TestGetAllAssignPatternPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1,
                                         kVarName2, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList2));
    StmtVarPairList result1 = pkb.GetAllAssignPatternPair({kTokenB});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum3, result1.front().first);
    Assert::AreEqual(kVarName1, result1.front().second);
    StmtVarPairList result2 = pkb.GetAllAssignPatternPair({kTokenA});
    Assert::IsTrue(result2.size() == 3);
    StmtVarPairList::iterator iter = result2.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    StmtVarPairList result3 = pkb.GetAllAssignPatternPair(kTokenList2);
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum3, result3.front().first);
    Assert::AreEqual(kVarName1, result3.front().second);
    StmtVarPairList result4 = pkb.GetAllAssignPatternPair(kTokenList);
    Assert::IsTrue(result4.size() == 3);
    StmtVarPairList::iterator iter1 = result4.begin();
    Assert::AreEqual(kStmtNum1, (*iter1).first);
    Assert::AreEqual(kVarName1, (*iter1).second);
    iter1++;
    Assert::AreEqual(kStmtNum2, (*iter1).first);
    Assert::AreEqual(kVarName2, (*iter1).second);
    iter1++;
    Assert::AreEqual(kStmtNum3, (*iter1).first);
    Assert::AreEqual(kVarName1, (*iter1).second);
    StmtVarPairList result5 = pkb.GetAllAssignPatternPair({kTokenPlus});
    Assert::IsTrue(result5.empty());
  }

  TEST_METHOD(TestGetAllAssignExactPatternPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt1, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt2, kStmtListIndex1,
                                         kVarName2, kVarNameSet1,
                                         kConstValueSet1, kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNumInt3, kStmtListIndex1,
                                         kVarName1, kVarNameSet1,
                                         kConstValueSet1, kTokenList2));
    StmtVarPairList result1 = pkb.GetAllAssignExactPatternPair(kTokenList2);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum3, result1.front().first);
    Assert::AreEqual(kVarName1, result1.front().second);
    StmtVarPairList result2 = pkb.GetAllAssignExactPatternPair(kTokenList1);
    Assert::IsTrue(result2.size() == 2);
    StmtVarPairList::iterator iter = result2.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
    StmtVarPairList result3 = pkb.GetAllAssignExactPatternPair({kToken32});
    Assert::IsTrue(result3.empty());
    StmtVarPairList result4 = pkb.GetAllAssignExactPatternPair(kTokenList);
    Assert::IsTrue(result4.empty());
  }

  TEST_METHOD(TestCallGraph) {
    PKB pkb;
    pkb.InsertEdgeInCallGraph(kProcName1, kProcName2);
    Assert::IsTrue(pkb.GetCallGraph()->GetSize() == 2);

    ProcNameList actual_sorted_calls = pkb.GetToposortedCalls();
    ProcNameList expected_calls = ProcNameList{kProcName2, kProcName1};
    Assert::IsTrue(actual_sorted_calls == expected_calls);
  }
};
}  // namespace PKBTests