#include "stdafx.h" 
#include "CppUnitTest.h"
#include "parent_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestParentTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "3";
  const StmtNum kStmtNum3 = "5";
  const StmtListIndex kStmtListIndex1 = 1;
  const StmtListIndex kStmtListIndex2 = 2;
  const StmtListIndex kStmtListIndex3 = 3;

  TEST_METHOD(TestIsDirectParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    Assert::IsTrue(parent_table.IsDirectParent(kStmtNum1, kStmtListIndex1));
    Assert::IsFalse(parent_table.IsDirectParent(kStmtNum1, kStmtListIndex2));
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex3);
    parent_table.InsertIndirectParentRelationship(kStmtNum3, kStmtListIndex3);
    Assert::IsTrue(parent_table.IsDirectParent(kStmtNum2, kStmtListIndex3));
    Assert::IsFalse(parent_table.IsDirectParent(kStmtNum3, kStmtListIndex3));
  }

  TEST_METHOD(TestIsParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    Assert::IsTrue(parent_table.IsParent(kStmtNum1, kStmtListIndex2));
    Assert::IsFalse(parent_table.IsParent(kStmtNum1, kStmtListIndex3));
  }

  TEST_METHOD(TestGetDirectParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    Assert::AreEqual(kStmtNum1, parent_table.GetDirectParent(kStmtListIndex1));
    Assert::AreNotEqual(kStmtNum1, parent_table.GetDirectParent(kStmtListIndex2));
  }

  TEST_METHOD(TestGetParents) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex1);
    StmtNumList result = parent_table.GetParents(kStmtListIndex1);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum1, result.back());
  }

  TEST_METHOD(TestGetDirectChildren) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex2);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex3);
    StmtListIndexList result = parent_table.GetDirectChildren(kStmtNum1);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtListIndex1, result.front());
    Assert::AreEqual(kStmtListIndex2, result.back());
  }

  TEST_METHOD(TestGetChildren) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex2);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex3);
    StmtListIndexList result = parent_table.GetChildren(kStmtNum1);
    Assert::IsTrue(result.size() == 3);
    Assert::AreEqual(kStmtListIndex1, result.front());
    StmtListIndexList::iterator iter = result.begin();
    iter++;
    Assert::AreEqual(kStmtListIndex2, *iter);
    Assert::AreEqual(kStmtListIndex3, result.back());
  }

  TEST_METHOD(TestHasParentRelationship) {
    ParentTable parent_table;
    Assert::IsFalse(parent_table.HasParentRelationship());
    parent_table.GetParents(kStmtListIndex1);
    Assert::IsFalse(parent_table.HasParentRelationship());
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    Assert::IsTrue(parent_table.HasParentRelationship());
  }
};
}  // namespace UnitTesting