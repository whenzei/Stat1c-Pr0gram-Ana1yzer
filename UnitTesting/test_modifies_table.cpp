#include "stdafx.h"
#include "CppUnitTest.h"
#include "modifies_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestModifiesTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "3";
  const StmtNum kStmtNum3 = "5";
  const VarName kVarName1 = "a";
  const VarName kVarName2 = "b";
  const VarName kVarName3 = "c";
  const StmtListIndex kStmtListIndex1 = 1;
  const StmtListIndex kStmtListIndex2 = 2;
  const StmtListIndex kStmtListIndex3 = 3;

  TEST_METHOD(TestIsModifiedBy) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    Assert::IsTrue(modifies_table.IsModifiedBy(kStmtNum1, kVarName1));
    Assert::IsFalse(modifies_table.IsModifiedBy(kStmtNum1, kVarName2));
  }

  TEST_METHOD(TestGetModifiedVar) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    VarNameList result1 = modifies_table.GetModifiedVar(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kVarName1, result1.front());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    VarNameList result2 = modifies_table.GetModifiedVar(kStmtNum1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kVarName1, result2.front());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName2);
    VarNameList result3 = modifies_table.GetModifiedVar(kStmtNum1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kVarName1, result3.front());
    Assert::AreEqual(kVarName2, result3.back());
    VarNameList result4 = modifies_table.GetModifiedVar(kStmtNum2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetModifiedVarStmtList) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    VarNameSet result1 = modifies_table.GetModifiedVar(kStmtListIndex1);
    Assert::IsTrue(result1.size() == 1);
    Assert::IsTrue(result1.find(kVarName1) != result1.end());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    VarNameSet result2 = modifies_table.GetModifiedVar(kStmtListIndex1);
    Assert::IsTrue(result2.size() == 1);
    Assert::IsTrue(result2.find(kVarName1) != result2.end());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName2);
    VarNameSet result3 = modifies_table.GetModifiedVar(kStmtListIndex1);
    Assert::IsTrue(result3.size() == 2);
    Assert::IsTrue(result3.find(kVarName1) != result3.end());
    Assert::IsTrue(result3.find(kVarName2) != result3.end());
    VarNameSet result4 = modifies_table.GetModifiedVar(kStmtListIndex2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetAllModifiedVar) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    VarNameList result1 = modifies_table.GetAllModifiedVar();
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kVarName1, result1.front());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    VarNameList result2 = modifies_table.GetAllModifiedVar();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kVarName1, result2.front());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName2);
    VarNameList result3 = modifies_table.GetAllModifiedVar();
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kVarName1, result3.front());
    Assert::AreEqual(kVarName2, result3.back());
  }

  TEST_METHOD(TestGetModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    StmtNumList result1 = modifies_table.GetModifyingStmt(kVarName1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    StmtNumList result2 = modifies_table.GetModifyingStmt(kVarName1);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    modifies_table.InsertModifies(kStmtNum2, kStmtListIndex1, kVarName1);
    StmtNumList result3 = modifies_table.GetModifyingStmt(kVarName1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
    StmtNumList result4 = modifies_table.GetModifyingStmt(kVarName2);
    Assert::IsTrue(result4.size() == 0);
  }

  TEST_METHOD(TestGetAllModifyingStmt) {
    ModifiesTable modifies_table;
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    StmtNumList result1 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    StmtNumList result2 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    modifies_table.InsertModifies(kStmtNum2, kStmtListIndex1, kVarName1);
    StmtNumList result3 = modifies_table.GetAllModifyingStmt();
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
  }

  TEST_METHOD(TestHasModifiesRelationship) {
    ModifiesTable modifies_table;
    Assert::IsFalse(modifies_table.HasModifiesRelationship());
	// to make sure that these functions don't affect the table size
    modifies_table.GetAllModifiedVar();
    modifies_table.GetModifiedVar(kStmtNum1);
    modifies_table.IsModifiedBy(kStmtNum2, kVarName1);
    modifies_table.GetAllModifiesPair();
    Assert::IsFalse(modifies_table.HasModifiesRelationship());
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    Assert::IsTrue(modifies_table.HasModifiesRelationship());
  }

  TEST_METHOD(TestGetAllModifiesPair) {
    ModifiesTable modifies_table;
    StmtVarPairList result1 = modifies_table.GetAllModifiesPair();
    Assert::IsTrue(result1.size() == 0);
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    StmtVarPairList result2 = modifies_table.GetAllModifiesPair();
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front().first);
    Assert::AreEqual(kVarName1, result2.front().second);
    modifies_table.InsertModifies(kStmtNum1, kStmtListIndex1, kVarName1);
    StmtVarPairList result3 = modifies_table.GetAllModifiesPair();
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum1, result3.front().first);
    Assert::AreEqual(kVarName1, result3.front().second);
    modifies_table.InsertModifies(kStmtNum2, kStmtListIndex1, kVarName1);
    StmtVarPairList result4 = modifies_table.GetAllModifiesPair();
    Assert::IsTrue(result4.size() == 2);
    Assert::AreEqual(kStmtNum1, result4.front().first);
    Assert::AreEqual(kVarName1, result4.front().second);
    Assert::AreEqual(kStmtNum2, result4.back().first);
    Assert::AreEqual(kVarName1, result4.back().second);
  }
};
}  // namespace PKBTests