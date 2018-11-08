#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using TokenType = Tokenizer::TokenType;

namespace PKBTests {
TEST_CLASS(TestPKB) {
  const ProcName kProcName1 = "one";
  const ProcName kProcName2 = "two";
  const ProcIndex kProcIndex1 = 0;
  const ProcIndex kProcIndex2 = 1;
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 2;
  const StmtNum kStmtNum3 = 3;
  const StmtNum kStmtNum4 = 4;
  const StmtNum kStmtNum5 = 5;
  const StmtNum kStmtNum6 = 6;
  const StmtNum kStmtNum7 = 7;
  const VarName kVarName1 = "a";
  const VarName kVarName2 = "b";
  const VarName kVarName3 = "c";
  const VarIndex kVarIndex1 = 0;
  const VarIndex kVarIndex2 = 1;
  const VarIndex kVarIndex3 = 2;
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
    ProcIndexSet proc_indices = pkb.GetAllProcIndices();
    Assert::IsTrue(proc_indices.size() == 2);
    ProcIndexSet expected_result = ProcIndexSet{kProcIndex1, kProcIndex2};
    Assert::AreEqual(expected_result, proc_indices);
  }

  TEST_METHOD(TestGetAllVarName) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    VarIndexSet result1 = pkb.GetAllVarIndices();
    Assert::IsTrue(result1.size() == 1);
    VarIndexSet expected_result1 = VarIndexSet{ kVarIndex1 };
    Assert::AreEqual(expected_result1, result1);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    VarIndexSet result2 = pkb.GetAllVarIndices();
    Assert::IsTrue(result2.size() == 3);
    VarIndexSet expected_result2 = VarIndexSet{kVarIndex1, kVarIndex2, kVarIndex3};
    Assert::AreEqual(expected_result2, result2);
  }

  TEST_METHOD(TestGetAllConstValue) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    ConstValueSet result = pkb.GetAllConstValue();
    Assert::IsTrue(result.size() == 2);
    ConstValueSet expected_result = ConstValueSet{ 1, 2 };
    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetStmtType) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum3, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum4, kProcIndex1, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum5, kProcIndex1, kVarName1));
    pkb.InsertCallStmt(
        &CallStmtData(kStmtNum6, kProcIndex1, kProcName1, kProcName2));
    Assert::IsTrue(PqlDeclarationEntity::kAssign == pkb.GetStmtType(kStmtNum1));
    Assert::IsTrue(PqlDeclarationEntity::kWhile == pkb.GetStmtType(kStmtNum2));
    Assert::IsTrue(PqlDeclarationEntity::kIf == pkb.GetStmtType(kStmtNum3));
    Assert::IsTrue(PqlDeclarationEntity::kRead == pkb.GetStmtType(kStmtNum4));
    Assert::IsTrue(PqlDeclarationEntity::kPrint == pkb.GetStmtType(kStmtNum5));
    Assert::IsTrue(PqlDeclarationEntity::kCall == pkb.GetStmtType(kStmtNum6));
  }

  TEST_METHOD(TestGetAllAssignStmt) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex2, kVarName2,
                                         kVarNameSet2, kConstValueSet1,
                                         kTokenList));
    StmtNumSet result = pkb.GetAllAssignStmt();
    Assert::IsTrue(result.size() == 2);
    StmtNumSet expected_result = StmtNumSet{ kStmtNum1, kStmtNum2 };
    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetAllWhileStmt) {
    PKB pkb;
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum1, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex2, kVarNameSet1, kConstValueSet1));
    StmtNumSet result = pkb.GetAllWhileStmt();
    Assert::IsTrue(result.size() == 2);
    StmtNumSet expected_result = StmtNumSet{ kStmtNum1, kStmtNum2 };
    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetAllIfStmt) {
    PKB pkb;
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum1, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum2, kProcIndex2, kVarNameSet1, kConstValueSet1));
    StmtNumSet result = pkb.GetAllIfStmt();
    Assert::IsTrue(result.size() == 2);
    StmtNumSet expected_result = StmtNumSet{ kStmtNum1, kStmtNum2 };
    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetAllReadStmt) {
    PKB pkb;
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum1, kProcIndex1, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum2, kProcIndex1, kVarName2));
    StmtNumSet result = pkb.GetAllReadStmt();
    Assert::IsTrue(result.size() == 2);
    StmtNumSet expected_result = StmtNumSet{ kStmtNum1, kStmtNum2 };
    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetAllPrintStmt) {
    PKB pkb;
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum1, kProcIndex1, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum2, kProcIndex2, kVarName2));
    StmtNumSet result = pkb.GetAllPrintStmt();
    Assert::IsTrue(result.size() == 2);
    StmtNumSet expected_result = StmtNumSet{ kStmtNum1, kStmtNum2 };
    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetAllCallStmt) {
    PKB pkb;
    pkb.InsertCallStmt(&CallStmtData(kStmtNum1, kProcIndex1, "one", "two"));
    pkb.InsertCallStmt(&CallStmtData(kStmtNum2, kProcIndex2, "one", "three"));
    StmtNumSet result = pkb.GetAllCallStmt();
    Assert::IsTrue(result.size() == 2);
    StmtNumSet expected_result = StmtNumSet{ kStmtNum1, kStmtNum2 };
    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetAllStmt) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex2, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum3, kProcIndex2, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum4, kProcIndex2, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum5, kProcIndex2, kVarName2));
    StmtNumSet result = pkb.GetAllStmt();
    Assert::IsTrue(result.size() == 5);
    StmtNumSet expected_result
      = StmtNumSet{ kStmtNum1, kStmtNum2, kStmtNum3, kStmtNum4, kStmtNum5 };

    Assert::AreEqual(expected_result, result);
  }

  TEST_METHOD(TestGetAssignWithPattern) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtNumSet result1 = pkb.GetAssignWithPattern(kVarName1, {kToken32});
    Assert::IsTrue(result1.size() == 2);
    StmtNumSet expected_result1 = StmtNumSet{ kStmtNum1, kStmtNum3 };
    Assert::AreEqual(expected_result1, result1);
    
    StmtNumSet result2 = pkb.GetAssignWithPattern(kVarName2, kTokenList);
    Assert::IsTrue(result2.size() == 1);
    StmtNumSet expected_result2 = StmtNumSet{ kStmtNum2 };
    Assert::AreEqual(expected_result2, result2);

    StmtNumSet result3 = pkb.GetAssignWithPattern("", {kTokenA});
    Assert::IsTrue(result3.size() == 3);
    StmtNumSet expected_result3 = StmtNumSet{ kStmtNum1, kStmtNum2, kStmtNum3 };
    Assert::AreEqual(expected_result3, result3);

    StmtNumSet result4 = pkb.GetAssignWithPattern("", kTokenList);
    Assert::IsTrue(result4.size() == 3);
    StmtNumSet expected_result4 = StmtNumSet{ kStmtNum1, kStmtNum2, kStmtNum3 };
    Assert::AreEqual(expected_result4, result4);
  }

  TEST_METHOD(TestGetAssignWithExactPattern) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtNumSet result1 = pkb.GetAssignWithExactPattern(kVarName1, kTokenList1);
    Assert::IsTrue(result1.size() == 1);
    StmtNumSet expected_result1 = StmtNumSet{ kStmtNum1 };
    Assert::AreEqual(expected_result1, result1);

    StmtNumSet result2 = pkb.GetAssignWithExactPattern(kVarName1, {kTokenA});
    Assert::IsTrue(result2.empty());
    
    StmtNumSet result3 = pkb.GetAssignWithExactPattern("", kTokenList1);
    Assert::IsTrue(result3.size() == 2);
    StmtNumSet expected_result3 = StmtNumSet{ kStmtNum1, kStmtNum2 };
    Assert::AreEqual(expected_result3, result3);
    
    StmtNumSet result4 = pkb.GetAssignWithExactPattern("", kTokenList2);
    Assert::IsTrue(result4.size() == 1);
    StmtNumSet expected_result4 = StmtNumSet{ kStmtNum3 };
    Assert::AreEqual(expected_result4, result4);
    
    StmtNumSet result5 = pkb.GetAssignWithExactPattern(kVarName1, kTokenList);
    Assert::IsTrue(result5.empty());
  }

  TEST_METHOD(TestGetAllAssignPatternPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtVarPairSet result1 = pkb.GetAllAssignPatternPair({kTokenB});
    Assert::IsTrue(result1.size() == 1);
    StmtVarPairSet expected_result1 = { { kStmtNum3, kVarIndex1 } };
    Assert::AreEqual(expected_result1, result1);

    StmtVarPairSet result2 = pkb.GetAllAssignPatternPair({kTokenA});
    Assert::IsTrue(result2.size() == 3);
    StmtVarPairSet expected_result2 = { { kStmtNum1, kVarIndex1 },
                                        { kStmtNum2, kVarIndex2 },
                                        { kStmtNum3, kVarIndex1 } };
    Assert::AreEqual(expected_result2, result2);

    StmtVarPairSet result3 = pkb.GetAllAssignPatternPair(kTokenList2);
    Assert::IsTrue(result3.size() == 1);
    StmtVarPairSet expected_result3 = { { kStmtNum3, kVarIndex1 } };
    Assert::AreEqual(expected_result3, result3);

    StmtVarPairSet result4 = pkb.GetAllAssignPatternPair(kTokenList);
    Assert::IsTrue(result4.size() == 3);
    StmtVarPairSet expected_result4 = { { kStmtNum1, kVarIndex1 },
                                        { kStmtNum2, kVarIndex2 },
                                        { kStmtNum3, kVarIndex1 } };
    Assert::AreEqual(expected_result4, result4);

    StmtVarPairSet result5 = pkb.GetAllAssignPatternPair({kTokenPlus});
    Assert::IsTrue(result5.empty());
  }

  TEST_METHOD(TestGetAllAssignExactPatternPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtVarPairSet result1 = pkb.GetAllAssignExactPatternPair(kTokenList2);
    Assert::IsTrue(result1.size() == 1);
    StmtVarPairSet expected_result1 = StmtVarPairSet{ { kStmtNum3, kVarIndex1 } };
    Assert::AreEqual(expected_result1, result1);

    StmtVarPairSet result2 = pkb.GetAllAssignExactPatternPair(kTokenList1);
    Assert::IsTrue(result2.size() == 2);
    StmtVarPairSet expected_result2 = StmtVarPairSet{ { kStmtNum1, kVarIndex1 },
                                                      { kStmtNum2, kVarIndex2 } };
    Assert::AreEqual(expected_result2, result2);
    
    
    StmtVarPairSet result3 = pkb.GetAllAssignExactPatternPair({kToken32});
    Assert::IsTrue(result3.empty());
    
    StmtVarPairSet result4 = pkb.GetAllAssignExactPatternPair(kTokenList);
    Assert::IsTrue(result4.empty());
  }

  TEST_METHOD(TestCallGraph) {
    PKB pkb;
    pkb.InsertEdgeInCallGraph(kProcName1, kProcName2);
    Assert::IsTrue(pkb.GetCallGraph()->GetSize() == 2);

    ProcIndexList actual_sorted_calls = pkb.GetToposortedCalls();
    // expect 2 first, then 1
    ProcIndexList expected_calls = ProcIndexList{kProcIndex2, kProcIndex1};
    Assert::IsTrue(actual_sorted_calls == expected_calls);
  }
};
}  // namespace PKBTests