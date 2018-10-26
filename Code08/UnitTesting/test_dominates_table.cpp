#include "stdafx.h"
#include "CppUnitTest.h"
#include "dominates_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestDominatesTable) {
  TEST_METHOD(TestIsDominates) {
	DominatesTable dominates_table;
    dominates_table.InsertDominates(1, {1, 2, 3, 4});
	dominates_table.InsertDominates(2, {2});
	dominates_table.InsertDominates(3, {3, 4});
	dominates_table.InsertDominates(4, {4});
    Assert::IsTrue(dominates_table.IsDominates(1, 1));
    Assert::IsTrue(dominates_table.IsDominates(1, 2));
    Assert::IsTrue(dominates_table.IsDominates(1, 3));
    Assert::IsTrue(dominates_table.IsDominates(1, 4));
    Assert::IsFalse(dominates_table.IsDominates(2, 1));
    Assert::IsTrue(dominates_table.IsDominates(2, 2));
    Assert::IsFalse(dominates_table.IsDominates(2, 3));
    Assert::IsFalse(dominates_table.IsDominates(2, 4));
    Assert::IsFalse(dominates_table.IsDominates(3, 1));
    Assert::IsFalse(dominates_table.IsDominates(3, 2));
    Assert::IsTrue(dominates_table.IsDominates(3, 3));
    Assert::IsTrue(dominates_table.IsDominates(3, 4));
    Assert::IsFalse(dominates_table.IsDominates(4, 1));
    Assert::IsFalse(dominates_table.IsDominates(4, 2));
    Assert::IsFalse(dominates_table.IsDominates(4, 3));
    Assert::IsTrue(dominates_table.IsDominates(4, 4));
  }

  TEST_METHOD(TestGetDominating) {
    DominatesTable dominates_table;
    dominates_table.InsertDominates(
        1, {1, 2, 3, 4});
    dominates_table.InsertDominates(2, {2});
    dominates_table.InsertDominates(3, {3, 4});
    dominates_table.InsertDominates(4, {4});
    StmtNumList result1 = dominates_table.GetDominating(1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(1, result1.front());
    StmtNumList result2 = dominates_table.GetDominating(2);
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(1, result2.front());
    Assert::AreEqual(2, result2.back());
    StmtNumList result3 = dominates_table.GetDominating(3);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(1, result3.front());
    Assert::AreEqual(3, result3.back());
    StmtNumList result4 = dominates_table.GetDominating(4);
    Assert::IsTrue(result4.size() == 3);
    StmtNumList::iterator iter = result4.begin();
    Assert::AreEqual(1, (*iter++));
    Assert::AreEqual(3, (*iter++));
    Assert::AreEqual(4, (*iter));
  }

  TEST_METHOD(TestGetDominated) {
    DominatesTable dominates_table;
    dominates_table.InsertDominates(1, {1, 2, 3, 4});
    dominates_table.InsertDominates(2, {2});
    dominates_table.InsertDominates(3, {3, 4});
    dominates_table.InsertDominates(4, {4});
    StmtNumList result1 = dominates_table.GetDominated(1);
    Assert::IsTrue(result1.size() == 4);
    StmtNumList::iterator iter = result1.begin();
    Assert::AreEqual(1, (*iter++));
    Assert::AreEqual(2, (*iter++));
    Assert::AreEqual(3, (*iter++));
    Assert::AreEqual(4, (*iter));
    StmtNumList result2 = dominates_table.GetDominated(2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(2, result2.front());
    StmtNumList result3 = dominates_table.GetDominated(3);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(3, result3.front());
    Assert::AreEqual(4, result3.back());
    StmtNumList result4 = dominates_table.GetDominated(4);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(4, result4.front());
  }

  TEST_METHOD(TestGetAllDominatesPairs) {
    DominatesTable dominates_table;
    dominates_table.InsertDominates(1, {1, 2, 3, 4});
    dominates_table.InsertDominates(2, {2});
    dominates_table.InsertDominates(3, {3, 4});
    dominates_table.InsertDominates(4, {4});
    StmtNumPairList result = dominates_table.GetAllDominatesPairs();
    StmtNumPairList expected = StmtNumPairList(
        {make_pair(1, 1), make_pair(1, 2), make_pair(1, 3), make_pair(1, 4),
         make_pair(2, 2), make_pair(3, 3), make_pair(3, 4), make_pair(4, 4)});
    Assert::IsTrue(expected == result);
  }

  TEST_METHOD(TestHasDominatesRelationship) {
    DominatesTable dominates_table;
    Assert::IsFalse(dominates_table.HasDominatesRelationship());
    dominates_table.InsertDominates(1, {1});
    Assert::IsTrue(dominates_table.HasDominatesRelationship());
  }
};
}  // namespace PKBTests