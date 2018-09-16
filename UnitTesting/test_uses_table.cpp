#include "stdafx.h"
#include "CPPUnitTest.h"
#include "uses_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {
TEST_CLASS(TestUsesTable) {
  const StmtNum kSampleStmtIdx1 = "1";
  const StmtNum kSampleStmtIdx2 = "4";
  const StmtNum kSampleStmtIdx3 = "102";
  const StmtNum kSampleStmtIdx4 = "27";
  const VarName kSampleVarName1 = "j";
  const VarName kSampleVarName2 = "iter";

  TEST_METHOD(TestInsertUsesSingleRel) {
    UsesTable uses;
    bool result = uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestInsertUsesDuplicateRel) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    bool result = uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestInsertUsesMultipleRel) {
    UsesTable uses;
    bool result = uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    Assert::IsTrue(result);
    result = uses.InsertUses(kSampleVarName2, kSampleStmtIdx3);
    Assert::IsTrue(result);
    result = uses.InsertUses(kSampleVarName2, kSampleStmtIdx4);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetAllUsedVar) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    uses.InsertUses(kSampleVarName2, kSampleStmtIdx3);
    VarNameList used_vars = uses.GetAllUsedVar();
    Assert::IsTrue(used_vars.size() == 2);
  }

  TEST_METHOD(TestGetAllUsedVarDuplicate) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx3);
    VarNameList used_vars = uses.GetAllUsedVar();
    Assert::IsTrue(used_vars.size() == 1);
  }

  TEST_METHOD(TestGetAllUsedVarSpecified) {
    VarNameList expected_used_vars;
    UsesTable uses;
    expected_used_vars.push_back(kSampleVarName1);
    expected_used_vars.push_back(kSampleVarName2);

    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    uses.InsertUses(kSampleVarName2, kSampleStmtIdx2);
    VarNameList used_vars = uses.GetAllUsedVar(kSampleStmtIdx2);
    Assert::AreEqual(expected_used_vars.front(), used_vars.front());
    Assert::AreEqual(expected_used_vars.back(), used_vars.back());
  }

  TEST_METHOD(TestGetAllUsingStmt) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    uses.InsertUses(kSampleVarName2, kSampleStmtIdx3);
    StmtList using_stmts = uses.GetAllUsingStmt();
    Assert::IsTrue(using_stmts.size() == 2);
  }

  TEST_METHOD(TestGetAllUsingStmtDuplicate) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    uses.InsertUses(kSampleVarName2, kSampleStmtIdx2);
    StmtList using_stmts = uses.GetAllUsingStmt();
    Assert::IsTrue(using_stmts.size() == 1);
  }

  TEST_METHOD(TestGetAllUsingStmtSpecified) {
    StmtList expected_using_stmts;
    UsesTable uses;
    expected_using_stmts.push_back(kSampleStmtIdx2);
    //expected_using_stmts.push_back(kSampleStmtIdx3);

    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    //uses.InsertUses(kSampleVarName1, kSampleStmtIdx3);
    StmtList using_stmts = uses.GetAllUsingStmt(kSampleVarName1);
    Assert::AreEqual(expected_using_stmts.front(), using_stmts.front());
    //Assert::AreEqual(expected_using_stmts.back(), using_stmts.back());
  }

  TEST_METHOD(TestIsUsedBy) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx3);
    bool result = uses.IsUsedBy(kSampleVarName1, kSampleStmtIdx3);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestHasUsesRelationships) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    bool result = uses.HasUsesRelationship();
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestHasUsesRelationshipsEmpty) {
    UsesTable uses;
    bool result = uses.HasUsesRelationship();
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetAllUsesPair) {
    UsesTable uses;
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx1);
    uses.InsertUses(kSampleVarName1, kSampleStmtIdx2);
    StmtVarPairList uses_pair_list = uses.GetAllUsesPair();
    Assert::AreEqual(kSampleStmtIdx1, uses_pair_list.front().first);
    Assert::AreEqual(kSampleVarName1, uses_pair_list.front().second);
    Assert::AreEqual(kSampleStmtIdx2, uses_pair_list.back().first);
    Assert::AreEqual(kSampleVarName1, uses_pair_list.back().second);
  }
};
}