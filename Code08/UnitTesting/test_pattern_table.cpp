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
  const VarName kVarName1 = a;
  const VarName kVarName2 = b;
  const VarName kVarName3 = c;
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
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtNumList result1 = pattern_table.GetAssignWithLfsVar(kVarName1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 = pattern_table.GetAssignWithLfsVar(kVarName1);
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum1, result2.front());
    Assert::AreEqual(kStmtNum2, result2.back());
  }

  TEST_METHOD(TestGetAssignWithSubExpr) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtNumList result1 =
        pattern_table.GetAssignWithSubExpr({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result5 = pattern_table.GetAssignWithSubExpr({kTokenPlus});
    Assert::IsTrue(result5.empty());
    StmtNumList result6 =
        pattern_table.GetAssignWithSubExpr({kTokenA, kTokenPlus});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 = pattern_table.GetAssignWithSubExpr(
        TokenList({kToken32, kTokenA, kTokenPlus}));
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum1, result2.front());
    Assert::AreEqual(kStmtNum2, result2.back());
    StmtNumList result3 = pattern_table.GetAssignWithSubExpr({kToken32});
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
    StmtNumList result4 = pattern_table.GetAssignWithSubExpr(
        TokenList({kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult}));
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
    pattern_table.InsertAssignPattern(kStmtNum3, kVarName1, kTokenList3);
    StmtNumList result7 = pattern_table.GetAssignWithSubExpr({kToken32});
    Assert::IsTrue(result7.size() == 3);
  }

  TEST_METHOD(TestGetAssignWithExactExpr) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtNumList result1 =
        pattern_table.GetAssignWithExactExpr({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result5 = pattern_table.GetAssignWithExactExpr({kToken32});
    Assert::IsTrue(result5.empty());
    StmtNumList result6 = pattern_table.GetAssignWithExactExpr({kTokenA});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 =
        pattern_table.GetAssignWithExactExpr({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    StmtNumList result3 = pattern_table.GetAssignWithExactExpr({kToken32});
    Assert::IsTrue(result3.empty());
    StmtNumList result4 = pattern_table.GetAssignWithSubExpr(
        {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }

  TEST_METHOD(TestGetAssignWithPattern) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtNumList result1 = pattern_table.GetAssignWithPattern(
        kVarName1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result5 =
        pattern_table.GetAssignWithPattern(kVarName1, {kToken32});
    Assert::IsTrue(result5.size() == 1);
    Assert::AreEqual(kStmtNum1, result5.front());
    StmtNumList result6 =
        pattern_table.GetAssignWithPattern(kVarName2, {kTokenA});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 = pattern_table.GetAssignWithPattern(
        kVarName1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum1, result2.front());
    Assert::AreEqual(kStmtNum2, result2.back());
    pattern_table.InsertAssignPattern(kStmtNum3, kVarName2, kTokenList1);
    StmtNumList result3 = pattern_table.GetAssignWithPattern(
        kVarName2, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result3.empty());
    StmtNumList result4 = pattern_table.GetAssignWithPattern(
        kVarName1, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }

  TEST_METHOD(TestGetAssignWithExactPattern) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtNumList result1 = pattern_table.GetAssignWithExactPattern(
        kVarName1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result5 =
        pattern_table.GetAssignWithExactPattern(kVarName1, {kToken32});
    Assert::IsTrue(result5.empty());
    StmtNumList result6 =
        pattern_table.GetAssignWithExactPattern(kVarName2, {kTokenA});
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 = pattern_table.GetAssignWithExactPattern(
        kVarName1, {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    pattern_table.InsertAssignPattern(kStmtNum3, kVarName2, kTokenList1);
    StmtNumList result3 = pattern_table.GetAssignWithExactPattern(
        kVarName2, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result3.empty());
    StmtNumList result4 = pattern_table.GetAssignWithExactPattern(
        kVarName1, {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }

  TEST_METHOD(TestGetAllAssignPatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtVarPairList result1 =
        pattern_table.GetAllAssignPatternPair({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kVarName1, result1.front().second);
    StmtVarPairList result5 = pattern_table.GetAllAssignPatternPair({kToken32});
    Assert::IsTrue(result5.size() == 1);
    Assert::AreEqual(kStmtNum1, result5.front().first);
    Assert::AreEqual(kVarName1, result5.front().second);
    StmtVarPairList result6 =
        pattern_table.GetAllAssignPatternPair(TokenList());
    Assert::IsTrue(result6.size() == 1);
    Assert::AreEqual(kStmtNum1, result6.front().first);
    Assert::AreEqual(kVarName1, result6.front().second);
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtVarPairList result2 =
        pattern_table.GetAllAssignPatternPair({kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum1, result2.front().first);
    Assert::AreEqual(kVarName1, result2.front().second);
    Assert::AreEqual(kStmtNum2, result2.back().first);
    Assert::AreEqual(kVarName1, result2.back().second);
    StmtVarPairList result3 = pattern_table.GetAllAssignPatternPair({kTokenB});
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum2, result3.front().first);
    Assert::AreEqual(kVarName1, result3.front().second);
    pattern_table.InsertAssignPattern(kStmtNum3, kVarName2, kTokenList1);
    StmtVarPairList result4 =
        pattern_table.GetAllAssignPatternPair(TokenList());
    Assert::IsTrue(result4.size() == 3);
    StmtVarPairList::iterator iter = result4.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
  }

  TEST_METHOD(TestGetAllAssignExactPatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtVarPairList result1 = pattern_table.GetAllAssignExactPatternPair(
        {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front().first);
    Assert::AreEqual(kVarName1, result1.front().second);
    StmtVarPairList result5 =
        pattern_table.GetAllAssignExactPatternPair({kToken32});
    Assert::IsTrue(result5.empty());
    StmtVarPairList result6 =
        pattern_table.GetAllAssignExactPatternPair(TokenList());
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtVarPairList result2 =
        pattern_table.GetAllAssignExactPatternPair({kTokenA});
    Assert::IsTrue(result2.empty());
    StmtVarPairList result3 = pattern_table.GetAllAssignExactPatternPair(
        {kToken32, kTokenA, kTokenPlus, kTokenB, kTokenMult});
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum2, result3.front().first);
    Assert::AreEqual(kVarName1, result3.front().second);
    pattern_table.InsertAssignPattern(kStmtNum3, kVarName2, kTokenList1);
    StmtVarPairList result4 = pattern_table.GetAllAssignExactPatternPair(
        {kToken32, kTokenA, kTokenPlus});
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kStmtNum1, result4.front().first);
    Assert::AreEqual(kVarName1, result4.front().second);
    Assert::AreEqual(kStmtNum3, result4.back().first);
    Assert::AreEqual(kVarName2, result4.back().second);
  }

  TEST_METHOD(TestGetWhileWithPattern) { 
	PatternTable pattern_table;
    pattern_table.InsertWhilePattern(kStmtNum1, kVarName1);
    pattern_table.InsertWhilePattern(kStmtNum2, kVarName2);
    pattern_table.InsertWhilePattern(kStmtNum3, kVarName1);
    StmtNumList result1 = pattern_table.GetWhileWithPattern(kVarName1);
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum3, result1.back());
    StmtNumList result2 = pattern_table.GetWhileWithPattern(kVarName2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum2, result2.front());
  }

  TEST_METHOD(TestGetAllWhilePatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertWhilePattern(kStmtNum1, kVarName1);
    pattern_table.InsertWhilePattern(kStmtNum1, kVarName3);
    pattern_table.InsertWhilePattern(kStmtNum2, kVarName2);
    pattern_table.InsertWhilePattern(kStmtNum3, kVarName1);
    StmtVarPairList result = pattern_table.GetAllWhilePatternPair();
    Assert::IsTrue(result.size() == 4);
    StmtVarPairList::iterator iter = result.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName3, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
  }

  TEST_METHOD(TestGetIfWithPattern) {
    PatternTable pattern_table;
    pattern_table.InsertIfPattern(kStmtNum1, kVarName1);
    pattern_table.InsertIfPattern(kStmtNum2, kVarName2);
    pattern_table.InsertIfPattern(kStmtNum3, kVarName1);
    StmtNumList result1 = pattern_table.GetIfWithPattern(kVarName1);
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum3, result1.back());
    StmtNumList result2 = pattern_table.GetIfWithPattern(kVarName2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum2, result2.front());
  }

  TEST_METHOD(TestGetAllIfPatternPair) {
    PatternTable pattern_table;
    pattern_table.InsertIfPattern(kStmtNum1, kVarName1);
    pattern_table.InsertIfPattern(kStmtNum1, kVarName3);
    pattern_table.InsertIfPattern(kStmtNum2, kVarName2);
    pattern_table.InsertIfPattern(kStmtNum3, kVarName1);
    StmtVarPairList result = pattern_table.GetAllIfPatternPair();
    Assert::IsTrue(result.size() == 4);
    StmtVarPairList::iterator iter = result.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarName1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarName3, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarName2, (*iter).second);
  }
};
}  // namespace PKBTests
