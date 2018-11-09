#include "stdafx.h"
#include "CppUnitTest.h"
#include "pattern_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using TokenType = Tokenizer::TokenType;

namespace PKBTests {

TEST_CLASS(TestPatternTable) {
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 3;
  const StmtNum kStmtNum3 = 5;
  const VarName kVarName1 = "a";
  const VarName kVarName2 = "b";
  const VarName kVarName3 = "c";
  const VarIndex kVarIndex1 = 0;
  const VarIndex kVarIndex2 = 1;
  const VarIndex kVarIndex3 = 2;
  const Token kToken32 = {TokenType::kDigit, "32"};
  const Token kTokenA = {TokenType::kName, "a"};
  const Token kTokenPlus = {TokenType::kOperator, "+"};
  const Token kTokenB = {TokenType::kName, "b"};
  const Token kTokenMult = {TokenType::kOperator, "*"};
  const TokenList kTokenList1 = {kToken32, kTokenA, kTokenPlus};
  const TokenList kTokenList2 = {kToken32, kTokenA, kTokenPlus, kTokenB,
                                 kTokenMult};
  const TokenList kTokenList3 = {kToken32};

  TEST_METHOD(TestGetAssignWithLfsVar) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarIndex1, kTokenList1);
    StmtNumSet result1 = pattern_table.GetAssignWithLfsVar(kVarIndex1);
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kStmtNum1));
    pattern_table.InsertAssignPattern(kStmtNum2, kVarIndex1, kTokenList2);
    StmtNumSet result2 = pattern_table.GetAssignWithLfsVar(kVarIndex1);
    Assert::IsTrue(result2.size() == 2);
    Assert::IsTrue(result2.count(kStmtNum1));
    Assert::IsTrue(result2.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAssignWithSubExpr) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarIndex1, kTokenList1);
    StmtNumSet result1 =
        pattern_table.GetAssignWithSubExpr({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kStmtNum1));
    StmtNumSet result5 = pattern_table.GetAssignWithSubExpr({kTokenPlus});
    Assert::IsTrue(result5.empty());
    StmtNumSet result6 =
        pattern_table.GetAssignWithSubExpr({kTokenA, kTokenPlus});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarIndex1, kTokenList2);
    StmtNumSet result2 = pattern_table.GetAssignWithSubExpr(
        TokenList({kToken32, kTokenA, kTokenPlus}));
    Assert::IsTrue(result2.size() == 2);
    Assert::IsTrue(result2.count(kStmtNum1));
    Assert::IsTrue(result2.count(kStmtNum2));
    StmtNumSet result3 = pattern_table.GetAssignWithSubExpr({kToken32});
    Assert::IsTrue(result3.size() == 2);
    Assert::IsTrue(result3.count(kStmtNum1));
    Assert::IsTrue(result3.count(kStmtNum2));
    StmtNumSet result4 = pattern_table.GetAssignWithSubExpr(
        TokenList({kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult}));
    Assert::IsTrue(result4.size() == 1);
    Assert::IsTrue(result4.count(kStmtNum2));
    pattern_table.InsertAssignPattern(kStmtNum3, kVarIndex1, kTokenList3);
    StmtNumSet result7 = pattern_table.GetAssignWithSubExpr({kToken32});
    Assert::IsTrue(result7.size() == 3);
  }

  TEST_METHOD(TestGetAssignWithExactExpr) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarIndex1, kTokenList1);
    StmtNumSet result1 =
        pattern_table.GetAssignWithExactExpr({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kStmtNum1));
    StmtNumSet result5 = pattern_table.GetAssignWithExactExpr({kToken32});
    Assert::IsTrue(result5.empty());
    StmtNumSet result6 = pattern_table.GetAssignWithExactExpr({kTokenA});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarIndex1, kTokenList2);
    StmtNumSet result2 =
        pattern_table.GetAssignWithExactExpr({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kStmtNum1));
    StmtNumSet result3 = pattern_table.GetAssignWithExactExpr({kToken32});
    Assert::IsTrue(result3.empty());
    StmtNumSet result4 = pattern_table.GetAssignWithSubExpr(
        {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result4.size() == 1);
    Assert::IsTrue(result4.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAssignWithPattern) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarIndex1, kTokenList1);
    StmtNumSet result1 = pattern_table.GetAssignWithPattern(
        kVarIndex1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kStmtNum1));
    StmtNumSet result5 =
        pattern_table.GetAssignWithPattern(kVarIndex1, {kToken32});
    Assert::IsTrue(result5.size() == 1);
    Assert::IsTrue(result5.count(kStmtNum1));
    StmtNumSet result6 =
        pattern_table.GetAssignWithPattern(kVarIndex2, {kTokenA});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarIndex1, kTokenList2);
    StmtNumSet result2 = pattern_table.GetAssignWithPattern(
        kVarIndex1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 2);
    Assert::IsTrue(result2.count(kStmtNum1));
    Assert::IsTrue(result2.count(kStmtNum2));
    pattern_table.InsertAssignPattern(kStmtNum3, kVarIndex2, kTokenList1);
    StmtNumSet result3 = pattern_table.GetAssignWithPattern(
        kVarIndex2, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result3.empty());
    StmtNumSet result4 = pattern_table.GetAssignWithPattern(
        kVarIndex1, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result4.size() == 1);
    Assert::IsTrue(result4.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAssignWithExactPattern) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarIndex1, kTokenList1);
    StmtNumSet result1 = pattern_table.GetAssignWithExactPattern(
        kVarIndex1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(kStmtNum1));
    StmtNumSet result5 =
        pattern_table.GetAssignWithExactPattern(kVarIndex1, {kToken32});
    Assert::IsTrue(result5.empty());
    StmtNumSet result6 =
        pattern_table.GetAssignWithExactPattern(kVarIndex2, {kTokenA});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarIndex1, kTokenList2);
    StmtNumSet result2 = pattern_table.GetAssignWithExactPattern(
        kVarIndex1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kStmtNum1));
    pattern_table.InsertAssignPattern(kStmtNum3, kVarIndex2, kTokenList1);
    StmtNumSet result3 = pattern_table.GetAssignWithExactPattern(
        kVarIndex2, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result3.empty());
    StmtNumSet result4 = pattern_table.GetAssignWithExactPattern(
        kVarIndex1, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result4.size() == 1);
    Assert::IsTrue(result4.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllAssignPatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarIndex1, kTokenList1);
    StmtVarPairSet result1 =
        pattern_table.GetAllAssignPatternPair({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kVarIndex1)));
    StmtVarPairSet result5 = pattern_table.GetAllAssignPatternPair({kToken32});
    Assert::IsTrue(result5.size() == 1);
    Assert::IsTrue(result5.count(std::make_pair(kStmtNum1, kVarIndex1)));
    StmtVarPairSet result6 =
        pattern_table.GetAllAssignPatternPair(TokenList());
    Assert::IsTrue(result6.size() == 1);
    Assert::IsTrue(result6.count(std::make_pair(kStmtNum1, kVarIndex1)));
    pattern_table.InsertAssignPattern(kStmtNum2, kVarIndex1, kTokenList2);
    StmtVarPairSet result2 =
        pattern_table.GetAllAssignPatternPair({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 2);
    Assert::IsTrue(result2.count(std::make_pair(kStmtNum1, kVarIndex1)));
    Assert::IsTrue(result2.count(std::make_pair(kStmtNum2, kVarIndex1)));
    StmtVarPairSet result3 = pattern_table.GetAllAssignPatternPair({kTokenB});
    Assert::IsTrue(result3.size() == 1);
    Assert::IsTrue(result3.count(std::make_pair(kStmtNum2, kVarIndex1)));
    pattern_table.InsertAssignPattern(kStmtNum3, kVarIndex2, kTokenList1);
    StmtVarPairSet result4 =
        pattern_table.GetAllAssignPatternPair(TokenList());
    Assert::IsTrue(result4.size() == 3);
    Assert::IsTrue(result4.count(std::make_pair(kStmtNum1, kVarIndex1)));
    Assert::IsTrue(result4.count(std::make_pair(kStmtNum2, kVarIndex1)));
    Assert::IsTrue(result4.count(std::make_pair(kStmtNum3, kVarIndex2)));
  }

  TEST_METHOD(TestGetAllAssignExactPatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarIndex1, kTokenList1);
    StmtVarPairSet result1 = pattern_table.GetAllAssignExactPatternPair(
        {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.count(std::make_pair(kStmtNum1, kVarIndex1)));
    StmtVarPairSet result5 =
        pattern_table.GetAllAssignExactPatternPair({kToken32});
    Assert::IsTrue(result5.empty());
    StmtVarPairSet result6 =
        pattern_table.GetAllAssignExactPatternPair(TokenList());
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarIndex1, kTokenList2);
    StmtVarPairSet result2 =
        pattern_table.GetAllAssignExactPatternPair({kTokenA});
    Assert::IsTrue(result2.empty());
    StmtVarPairSet result3 = pattern_table.GetAllAssignExactPatternPair(
        {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result3.size() == 1);
    Assert::IsTrue(result3.count(std::make_pair(kStmtNum2, kVarIndex1)));
    pattern_table.InsertAssignPattern(kStmtNum3, kVarIndex2, kTokenList1);
    StmtVarPairSet result4 = pattern_table.GetAllAssignExactPatternPair(
        {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result4.size() == 2);
    Assert::IsTrue(result4.count(std::make_pair(kStmtNum1, kVarIndex1)));
    Assert::IsTrue(result4.count(std::make_pair(kStmtNum3, kVarIndex2)));
  }

  TEST_METHOD(TestGetWhileWithPattern) {
    PatternTable pattern_table;
    pattern_table.InsertWhilePattern(kStmtNum1, kVarIndex1);
    pattern_table.InsertWhilePattern(kStmtNum2, kVarIndex2);
    pattern_table.InsertWhilePattern(kStmtNum3, kVarIndex1);
    StmtNumSet result1 = pattern_table.GetWhileWithPattern(kVarIndex1);
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum3));
    StmtNumSet result2 = pattern_table.GetWhileWithPattern(kVarIndex2);
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllWhilePatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertWhilePattern(kStmtNum1, kVarIndex1);
    pattern_table.InsertWhilePattern(kStmtNum1, kVarIndex3);
    pattern_table.InsertWhilePattern(kStmtNum2, kVarIndex2);
    pattern_table.InsertWhilePattern(kStmtNum3, kVarIndex1);
    StmtVarPairSet result = pattern_table.GetAllWhilePatternPair();
    Assert::IsTrue(result.size() == 4);
    Assert::IsTrue(result.count(std::make_pair(kStmtNum1, kVarIndex1)));
    Assert::IsTrue(result.count(std::make_pair(kStmtNum3, kVarIndex1)));
    Assert::IsTrue(result.count(std::make_pair(kStmtNum1, kVarIndex3)));
    Assert::IsTrue(result.count(std::make_pair(kStmtNum2, kVarIndex2)));
  }

  TEST_METHOD(TestGetIfWithPattern) {
    PatternTable pattern_table;
    pattern_table.InsertIfPattern(kStmtNum1, kVarIndex1);
    pattern_table.InsertIfPattern(kStmtNum2, kVarIndex2);
    pattern_table.InsertIfPattern(kStmtNum3, kVarIndex1);
    StmtNumSet result1 = pattern_table.GetIfWithPattern(kVarIndex1);
    Assert::IsTrue(result1.size() == 2);
    Assert::IsTrue(result1.count(kStmtNum1));
    Assert::IsTrue(result1.count(kStmtNum3));
    StmtNumSet result2 = pattern_table.GetIfWithPattern(kVarIndex2);
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.count(kStmtNum2));
  }

  TEST_METHOD(TestGetAllIfPatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertIfPattern(kStmtNum1, kVarIndex1);
    pattern_table.InsertIfPattern(kStmtNum1, kVarIndex3);
    pattern_table.InsertIfPattern(kStmtNum2, kVarIndex2);
    pattern_table.InsertIfPattern(kStmtNum3, kVarIndex1);
    StmtVarPairSet result = pattern_table.GetAllIfPatternPair();
    Assert::IsTrue(result.size() == 4);
    StmtVarPairSet::iterator iter = result.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarIndex1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarIndex1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarIndex3, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarIndex2, (*iter).second);
  }
};
}  // namespace PKBTests
