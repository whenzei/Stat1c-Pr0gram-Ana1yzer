#include "stdafx.h"
#include "CppUnitTest.h"
#include "parent_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestParentTable) {
  const StmtNum kStmtNum1 = "1";
  const StmtNum kStmtNum2 = "2";
  const StmtNum kStmtNum3 = "3";
  const StmtNum kStmtNum4 = "4";
  
  TEST_METHOD(TestIsParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    Assert::IsTrue(parent_table.IsParent(kStmtNum1, kStmtNum2));
    Assert::IsTrue(parent_table.IsParent(kStmtNum2, kStmtNum3));
    Assert::IsFalse(parent_table.IsParent(kStmtNum1, kStmtNum3));
    Assert::IsFalse(parent_table.IsParent(kStmtNum2, kStmtNum1));
  }

  TEST_METHOD(TestIsParentT) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    Assert::IsTrue(parent_table.IsParentT(kStmtNum1, kStmtNum2));
    Assert::IsTrue(parent_table.IsParentT(kStmtNum2, kStmtNum3));
    Assert::IsTrue(parent_table.IsParentT(kStmtNum1, kStmtNum3));
    Assert::IsFalse(parent_table.IsParentT(kStmtNum2, kStmtNum1));
  }

  TEST_METHOD(TestGetParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    Assert::AreEqual(StmtNum(), parent_table.GetParent(kStmtNum1));
    Assert::AreEqual(kStmtNum1, parent_table.GetParent(kStmtNum2));
    Assert::AreEqual(kStmtNum2, parent_table.GetParent(kStmtNum3));
  }

  TEST_METHOD(TestGetParentT) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    StmtNumList result1 = parent_table.GetParentT(kStmtNum3);
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum2, result1.front());
    Assert::AreEqual(kStmtNum1, result1.back());
    StmtNumList result2 = parent_table.GetParentT(kStmtNum2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    StmtNumList result3 = parent_table.GetParentT(kStmtNum1);
    Assert::IsTrue(result3.empty());
  }

  TEST_METHOD(TestGetAllParent) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    StmtNumList result = parent_table.GetAllParent();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetChild) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum4);
    StmtNumList result1 = parent_table.GetChild(kStmtNum1);
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum2, result1.front());
    Assert::AreEqual(kStmtNum4, result1.back());
    StmtNumList result2 = parent_table.GetChild(kStmtNum2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum3, result2.front());
  }

  TEST_METHOD(TestGetChildT) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum4);
    StmtNumList result1 = parent_table.GetChildT(kStmtNum1);
    Assert::IsTrue(result1.size() == 3);
    StmtNumList::iterator iter = result1.begin();
    Assert::AreEqual(kStmtNum2, (*iter));
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter));
    iter++;
    Assert::AreEqual(kStmtNum4, (*iter));
    StmtNumList result2 = parent_table.GetChildT(kStmtNum2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum3, result2.front());
  }

  TEST_METHOD(TestGetAllChild) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    parent_table.InsertDirectParentRelationship(kStmtNum2, kStmtNum3);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum3);
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum4);
    StmtNumList result1 = parent_table.GetAllChild();
    Assert::IsTrue(result1.size() == 3);
    StmtNumList::iterator iter = result1.begin();
    Assert::AreEqual(kStmtNum2, (*iter));
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter));
    iter++;
    Assert::AreEqual(kStmtNum4, (*iter));
  }

  TEST_METHOD(TestHasParentRelationship) {
    ParentTable parent_table;
    Assert::IsFalse(parent_table.HasParentRelationship());
    parent_table.GetParentT(kStmtNum1);
    Assert::IsFalse(parent_table.HasParentRelationship());
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    Assert::IsTrue(parent_table.HasParentRelationship());
  }

  TEST_METHOD(TestGetAllParentPair) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    StmtNumPairList parent_pair = parent_table.GetAllParentPair();
    Assert::IsTrue(parent_pair.size() == 1);
    Assert::AreEqual(kStmtNum1, parent_pair.front().first);
    Assert::AreEqual(kStmtNum2, parent_pair.front().second);
    parent_table.InsertIndirectParentRelationship(kStmtNum2, kStmtNum4);
    parent_pair = parent_table.GetAllParentPair();
    Assert::IsTrue(parent_pair.size() == 1);
  }

  TEST_METHOD(TestGetAllParentTPair) {
    ParentTable parent_table;
    parent_table.InsertDirectParentRelationship(kStmtNum1, kStmtNum2);
    StmtNumPairList parent_pair = parent_table.GetAllParentTPair();
    Assert::IsTrue(parent_pair.size() == 1);
    Assert::AreEqual(kStmtNum1, parent_pair.front().first);
    Assert::AreEqual(kStmtNum2, parent_pair.front().second);
    parent_table.InsertIndirectParentRelationship(kStmtNum1, kStmtNum4);
    parent_pair = parent_table.GetAllParentTPair();
    Assert::IsTrue(parent_pair.size() == 2);
    Assert::AreEqual(kStmtNum1, parent_pair.back().first);
    Assert::AreEqual(kStmtNum4, parent_pair.back().second);
  }
};
}  // namespace UnitTesting