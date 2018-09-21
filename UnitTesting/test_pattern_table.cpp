#include "stdafx.h"
#include "CppUnitTest.h"
#include "pattern_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using TokenType = Tokenizer::TokenType;

namespace PKBTests {

TEST_CLASS(TestPatternTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "3";
  const StmtNum kStmtNum3 = "5";
  const VarName kVarName1 = "a";
  const VarName kVarName2 = "b";
  const VarName kVarName3 = "c";
  const TokenList kTokenList1 = TokenList({{TokenType::kDigit, "32"},
                                           {TokenType::kName, "a"},
                                           {TokenType::kOperator, "+"}});
  const TokenList kTokenList2 = TokenList({{TokenType::kDigit, "32"},
                                           {TokenType::kName, "a"},
                                           {TokenType::kOperator, "+"},
                                           {TokenType::kName, "b"},
                                           {TokenType::kOperator, "*"}});
  const TokenList kTokenList3 = TokenList({{TokenType::kDigit, "32"}});

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
    StmtNumList result1 = pattern_table.GetAssignWithSubExpr("32 a +");
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result5 = pattern_table.GetAssignWithSubExpr("+");
    Assert::IsTrue(result5.empty());
    StmtNumList result6 = pattern_table.GetAssignWithSubExpr("a +");
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 = pattern_table.GetAssignWithSubExpr("32 a +");
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum1, result2.front());
    Assert::AreEqual(kStmtNum2, result2.back());
    StmtNumList result3 = pattern_table.GetAssignWithSubExpr("32");
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
    StmtNumList result4 = pattern_table.GetAssignWithSubExpr("32 a + b *");
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
    pattern_table.InsertAssignPattern(kStmtNum3, kVarName1, kTokenList3);
    StmtNumList result7 = pattern_table.GetAssignWithSubExpr("32");
    Assert::IsTrue(result7.size() == 3);
  }

  TEST_METHOD(TestGetAssignWithExactExpr) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtNumList result1 = pattern_table.GetAssignWithExactExpr("32 a +");
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result5 = pattern_table.GetAssignWithExactExpr("32");
    Assert::IsTrue(result5.empty());
    StmtNumList result6 = pattern_table.GetAssignWithExactExpr("a");
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 = pattern_table.GetAssignWithExactExpr("32 a +");
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    StmtNumList result3 = pattern_table.GetAssignWithExactExpr("32");
    Assert::IsTrue(result3.empty());
    StmtNumList result4 = pattern_table.GetAssignWithSubExpr("32 a + b *");
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }

  TEST_METHOD(TestGetAssignWithPattern) {
    PatternTable pattern_table;
    pattern_table.InsertAssignPattern(kStmtNum1, kVarName1, kTokenList1);
    StmtNumList result1 = pattern_table.GetAssignWithPattern(kVarName1, "32 a +");
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result5 = pattern_table.GetAssignWithPattern(kVarName1, "32");
    Assert::IsTrue(result5.size() == 1);
    Assert::AreEqual(kStmtNum1, result5.front());
    StmtNumList result6 = pattern_table.GetAssignWithPattern(kVarName2, "a");
    Assert::IsTrue(result6.empty());
    pattern_table.InsertAssignPattern(kStmtNum2, kVarName1, kTokenList2);
    StmtNumList result2 = pattern_table.GetAssignWithPattern(kVarName1, "32 a +");
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum1, result2.front());
    Assert::AreEqual(kStmtNum2, result2.back());
    pattern_table.InsertAssignPattern(kStmtNum3, kVarName2, kTokenList1);
    StmtNumList result3 =
        pattern_table.GetAssignWithPattern(kVarName2, "32 a + b *");
    Assert::IsTrue(result3.empty());
    StmtNumList result4 = pattern_table.GetAssignWithPattern(kVarName1, "32 a + b *");
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }
};
}  // namespace PKBTests