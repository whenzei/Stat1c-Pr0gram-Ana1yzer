#include "stdafx.h"
#include "CPPUnitTest.h"
#include "follows_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::next;

namespace PKBTests {
TEST_CLASS(TestFollowsTable) {
  const StmtNum kSampleStmtIdx1 = 1;
  const StmtNum kSampleStmtIdx2 = 2;
  const StmtNum kSampleStmtIdx3 = 3;
  const StmtNum kSampleStmtIdx4 = 4;
  const StmtNum kSampleStmtIdx5 = 5;

  TEST_METHOD(TestIsFollowsT) {
    FollowsTable follows_table;
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);  // direct
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);  // indirect
    bool result = follows_table.IsFollowsT(kSampleStmtIdx1, kSampleStmtIdx3);
    Assert::IsTrue(result);
    result = follows_table.IsFollowsT(kSampleStmtIdx1, kSampleStmtIdx2);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestIsFollows) {
    FollowsTable follows_table;
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2); // direct
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3); // indirect
    bool result = follows_table.IsFollows(kSampleStmtIdx1, kSampleStmtIdx2);
    Assert::IsTrue(result);
    result = follows_table.IsFollows(kSampleStmtIdx1, kSampleStmtIdx3);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetFollowsT) {
    StmtNumSet expected_results = { kSampleStmtIdx2, kSampleStmtIdx3 };
    FollowsTable follows_table;

    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);  // direct
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);  // indirect
    follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx4);  // direct
    StmtNumSet followers = follows_table.GetFollowsT(kSampleStmtIdx1);
    Assert::AreEqual(expected_results, followers);
  }

  TEST_METHOD(TestGetFollows) {
    FollowsTable follows_table;
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);  // direct
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx4);  // indirect

    StmtNumSet follower = follows_table.GetFollows(kSampleStmtIdx1);
    StmtNumSet expected_results = StmtNumSet{ kSampleStmtIdx2 };
    Assert::AreEqual(expected_results, follower);
  }

  TEST_METHOD(TestGetAllFollows) {
    StmtNumSet expected_results = { kSampleStmtIdx2, kSampleStmtIdx3, kSampleStmtIdx4 };
    FollowsTable follows_table;

    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);  // direct
    follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);  // direct
    follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx4);  // indirect

    StmtNumSet followers = follows_table.GetAllFollows();
    Assert::IsTrue(followers.size() == 3);
    Assert::AreEqual(expected_results, followers);
  }

  TEST_METHOD(TestGetFollowedByT) {
    StmtNumSet expected_results = { kSampleStmtIdx1, kSampleStmtIdx2 };
    FollowsTable follows_table;

    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);  // direct
    follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);  // direct
    StmtNumSet followees = follows_table.GetFollowedByT(kSampleStmtIdx3);
    Assert::AreEqual(expected_results, followees);
  }

  TEST_METHOD(TestGetFollowedBy) {
    FollowsTable follows_table;
    StmtNumSet expected_results = { kSampleStmtIdx2 };
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);  // direct
    follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);  // direct
    follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);  // direct 

    StmtNumSet followee = follows_table.GetFollowedBy(kSampleStmtIdx3);
    Assert::AreEqual(expected_results, followee);
  }

  TEST_METHOD(TestGetAllFollowedBy) {
    StmtNumSet expected_results = { kSampleStmtIdx1, kSampleStmtIdx2, kSampleStmtIdx3, 
      kSampleStmtIdx5 };
    FollowsTable follows_table;

    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
    follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
    follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);
    follows_table.InsertFollows(kSampleStmtIdx5, kSampleStmtIdx4);

    StmtNumSet followees = follows_table.GetAllFollowedBy();
    Assert::IsTrue(followees.size() == 4);
    Assert::AreEqual(expected_results, followees);
  }

  TEST_METHOD(TestHasFollowsRelationship) {
    FollowsTable follows_table_1;
    bool result = follows_table_1.HasFollowsRelationship();
    Assert::AreEqual(false, result);

    follows_table_1.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
    result = follows_table_1.HasFollowsRelationship();
    Assert::AreEqual(true, result);

    FollowsTable follows_table_2;
    follows_table_2.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx4);
    result = follows_table_2.HasFollowsRelationship();
    Assert::AreEqual(true, result);
  }

  TEST_METHOD(TestGetFollowsPairT) {
    FollowsTable follows_table;
    StmtNumPairSet follows_pair = follows_table.GetAllFollowsTPair();
    Assert::IsTrue(follows_pair.empty());

    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
    follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);
    follows_pair = follows_table.GetAllFollowsTPair();
    StmtNumPairSet expected_results = StmtNumPairSet{ {kSampleStmtIdx1, kSampleStmtIdx2},
                                                      {kSampleStmtIdx3, kSampleStmtIdx4} };
    Assert::AreEqual(expected_results, follows_pair);
  }

  TEST_METHOD(TestGetFollowsPairTMultipleFollower) {
    FollowsTable follows_table;
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
    follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
    StmtNumPairSet follows_pair = follows_table.GetAllFollowsTPair();
    StmtNumPairSet expected_results = StmtNumPairSet{ {kSampleStmtIdx1, kSampleStmtIdx2},
                                                      {kSampleStmtIdx1, kSampleStmtIdx3} };
    Assert::AreEqual(expected_results, follows_pair);
  }

  TEST_METHOD(TestGetFollowsPair) {
    FollowsTable follows_table_1;
    StmtNumPairSet follows_pair = follows_table_1.GetAllFollowsPair();
    Assert::IsTrue(follows_pair.empty());

    follows_table_1.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
    follows_pair = follows_table_1.GetAllFollowsPair();
    StmtNumPairSet expected_results_1 = StmtNumPairSet{ (kSampleStmtIdx1, kSampleStmtIdx2) };
    Assert::AreEqual(expected_results_1, follows_pair);
    // Indirect relationship inserted. Results should not change.
    follows_table_1.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
    follows_pair = follows_table_1.GetAllFollowsPair();
    Assert::AreEqual(expected_results_1, follows_pair);
    // More direct relationships inserted. Results should change.
    follows_table_1.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
    follows_table_1.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);
    StmtNumPairSet expected_results_2 = StmtNumPairSet{ {kSampleStmtIdx1, kSampleStmtIdx2},
                                                        {kSampleStmtIdx2, kSampleStmtIdx3},
                                                        {kSampleStmtIdx3, kSampleStmtIdx4} };
    follows_pair = follows_table_1.GetAllFollowsPair();
    Assert::AreEqual(expected_results_2, follows_pair);
  }
};
}  // namespace PKBTests