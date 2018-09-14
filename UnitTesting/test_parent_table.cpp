#include "stdafx.h"
#include "CppUnitTest.h"
#include "parent_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestParentTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "3";
  const StmtNum kStmtNum3 = "5";
  const StmtListIndex kStmtListIndex1 = 1;
  const StmtListIndex kStmtListIndex2 = 2;
  const StmtListIndex kStmtListIndex3 = 3;
  
  TEST_METHOD(TestIsParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    Assert::IsTrue(parent_table.IsParent(kStmtNum1, kStmtListIndex1));
    Assert::IsFalse(parent_table.IsParent(kStmtNum1, kStmtListIndex2));
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex3);
    parent_table.InsertIndirectParentRelationship(kStmtNum3, kStmtListIndex3);
    Assert::IsTrue(parent_table.IsParent(kStmtNum2, kStmtListIndex3));
    Assert::IsFalse(parent_table.IsParent(kStmtNum3, kStmtListIndex3));
  }

  TEST_METHOD(TestIsParentT) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    Assert::IsTrue(parent_table.IsParentT(kStmtNum1, kStmtListIndex2));
    Assert::IsFalse(parent_table.IsParentT(kStmtNum1, kStmtListIndex3));
  }

  TEST_METHOD(TestGetParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    Assert::AreEqual(kStmtNum1, parent_table.GetParent(kStmtListIndex1));
    Assert::AreNotEqual(kStmtNum1, parent_table.GetParent(kStmtListIndex2));
  }

  TEST_METHOD(TestGetParentT) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex1);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex1);
    StmtNumList result = parent_table.GetParentT(kStmtListIndex1);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum1, result.back());
  }

  TEST_METHOD(TestGetAllParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex2);
    ParentsSet result = parent_table.GetAllParent();
    Assert::IsTrue(result.size() == 2);
    Assert::IsTrue(result.find(kStmtNum1) != result.end());
    Assert::IsTrue(result.find(kStmtNum2) != result.end());
  }

  TEST_METHOD(TestGetChild) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex2);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex3);
    StmtListIndexList result = parent_table.GetChild(kStmtNum1);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtListIndex1, result.front());
    Assert::AreEqual(kStmtListIndex2, result.back());
  }

  TEST_METHOD(TestGetChildT) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex2);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex3);
    StmtListIndexList result = parent_table.GetChildT(kStmtNum1);
    Assert::IsTrue(result.size() == 3);
    Assert::AreEqual(kStmtListIndex1, result.front());
    StmtListIndexList::iterator iter = result.begin();
    iter++;
    Assert::AreEqual(kStmtListIndex2, *iter);
    Assert::AreEqual(kStmtListIndex3, result.back());
  }

  TEST_METHOD(TestGetAllChild) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex2);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    parent_table.InsertDirectParentRelationship(kStmtNum3, kStmtListIndex3);
    ChildrenSet result = parent_table.GetAllChild();
    Assert::IsTrue(result.size() == 3);
    Assert::IsTrue(result.find(kStmtListIndex1) != result.end());
    Assert::IsTrue(result.find(kStmtListIndex2) != result.end());
    Assert::IsTrue(result.find(kStmtListIndex3) != result.end());
  }

  TEST_METHOD(TestHasParentRelationship) {
    ParentTable parent_table;
    Assert::IsFalse(parent_table.HasParentRelationship());
    parent_table.GetParentT(kStmtListIndex1);
    Assert::IsFalse(parent_table.HasParentRelationship());
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    Assert::IsTrue(parent_table.HasParentRelationship());
  }

  TEST_METHOD(TestGetAllParentPair) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex2);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    parent_table.InsertDirectParentRelationship(kStmtNum3, kStmtListIndex3);
    DirectParentMap result = parent_table.GetAllParentPair();
    Assert::IsTrue(result.size() == 3);
    Assert::IsTrue(result[kStmtListIndex1] == kStmtNum1);
    Assert::IsTrue(result[kStmtListIndex2] == kStmtNum2);
    Assert::IsTrue(result[kStmtListIndex3] == kStmtNum3);
  }

  TEST_METHOD(TestGetAllParentTPair) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtListIndex1);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtListIndex2);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtListIndex2);
    parent_table.InsertDirectParentRelationship(kStmtNum3, kStmtListIndex3);
    ParentsMap result = parent_table.GetAllParentTPair();
    Assert::IsTrue(result.size() == 3);
    Assert::IsTrue(result[kStmtListIndex1].size() == 1);
    Assert::IsTrue(result[kStmtListIndex1].front() == kStmtNum1);
    Assert::IsTrue(result[kStmtListIndex2].size() == 2);
    Assert::IsTrue(result[kStmtListIndex2].front() == kStmtNum2);
    Assert::IsTrue(result[kStmtListIndex2].back() == kStmtNum1);
    Assert::IsTrue(result[kStmtListIndex3].size() == 1);
    Assert::IsTrue(result[kStmtListIndex3].front() == kStmtNum3);
  }
};
}  // namespace UnitTesting