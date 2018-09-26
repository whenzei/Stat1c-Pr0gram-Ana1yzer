#include "stdafx.h"
#include "CPPUnitTest.h"
#include "follows_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::next;

namespace PKBTests {
  TEST_CLASS(TestFollowsTable) {
    const StmtNum kSampleStmtIdx1 = "1";
    const StmtNum kSampleStmtIdx2 = "2";
    const StmtNum kSampleStmtIdx3 = "3";
    const StmtNum kSampleStmtIdx4 = "4";
    const StmtNum kSampleStmtIdx5 = "5";

    TEST_METHOD(TestIsFollowsT) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows_table.IsFollowsT(kSampleStmtIdx1, kSampleStmtIdx3);
      Assert::IsTrue(result);
      result = follows_table.IsFollowsT(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::IsTrue(result);
    }

    TEST_METHOD(TestIsFollows) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      bool result = follows_table.IsFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::IsTrue(result);
      result = follows_table.IsFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      Assert::IsFalse(result);
    }

    TEST_METHOD(TestGetFollowsT) {
      StmtNumList expected_followers;
      FollowsTable follows_table;
      expected_followers.push_back(kSampleStmtIdx2);
      expected_followers.push_back(kSampleStmtIdx3);

      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      StmtNumList followers = follows_table.GetFollowsT(kSampleStmtIdx1);
      Assert::AreEqual(expected_followers.front(), followers.front());
      Assert::AreEqual(expected_followers.back(), followers.back());
    }

    TEST_METHOD(TestGetFollows) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);

      StmtNumList follower = follows_table.GetFollows(kSampleStmtIdx1);
      Assert::AreEqual(kSampleStmtIdx2, follower.front());
    }

    TEST_METHOD(TestGetAllFollows) {
      StmtNumList expected_followers;
      FollowsTable follows_table;
      expected_followers.push_back(kSampleStmtIdx2);
      expected_followers.push_back(kSampleStmtIdx3);
      expected_followers.push_back(kSampleStmtIdx4);

      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);

      StmtNumList followers = follows_table.GetAllFollows();
      Assert::IsTrue(followers.size()==3);
      Assert::AreEqual(expected_followers.front(), followers.front());
      Assert::AreEqual(expected_followers.back(), followers.back());
    }

    TEST_METHOD(TestGetFollowedByT) {
      StmtNumList expected_followees;
      FollowsTable follows_table;
      expected_followees.push_back(kSampleStmtIdx2);
      expected_followees.push_back(kSampleStmtIdx1);

      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      StmtNumList followees = follows_table.GetFollowedByT(kSampleStmtIdx3);
      Assert::AreEqual(expected_followees.front(), followees.front());
      Assert::AreEqual(expected_followees.back(), followees.back());
    }

    TEST_METHOD(TestGetFollowedBy) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);

      StmtNumList followee = follows_table.GetFollowedBy(kSampleStmtIdx3);
      Assert::AreEqual(kSampleStmtIdx2, followee.front());
    }

    TEST_METHOD(TestGetAllFollowedBy) {
      StmtNumList expected_followees;
      FollowsTable follows_table;
      expected_followees.push_back(kSampleStmtIdx1);
      expected_followees.push_back(kSampleStmtIdx2);
      expected_followees.push_back(kSampleStmtIdx3);
      expected_followees.push_back(kSampleStmtIdx5);

      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);
      follows_table.InsertFollows(kSampleStmtIdx5, kSampleStmtIdx4);

      StmtNumList followees = follows_table.GetAllFollowedBy();
      Assert::IsTrue(followees.size() == 4);
      Assert::AreEqual(expected_followees.front(), followees.front());
      Assert::AreEqual(expected_followees.back(), followees.back());
    }

    TEST_METHOD(TestHasFollowsRelationship) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows_table.HasFollowsRelationship();
      Assert::AreEqual(true, result);
    }

    TEST_METHOD(TestHasFollowsRelationshipEmpty) {
      FollowsTable follows_table;
      bool result = follows_table.HasFollowsRelationship();
      Assert::AreEqual(false, result);
    }

    TEST_METHOD(TestGetFollowsPairTEmpty) {
      FollowsTable follows_t_table;
      StmtNumPairList follows_pair = follows_t_table.GetAllFollowsTPair();
      Assert::IsTrue(follows_pair.empty());
    }

    TEST_METHOD(TestGetFollowsPairT) {
      FollowsTable follows_t_table;
      follows_t_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      StmtNumPairList follows_pair = follows_t_table.GetAllFollowsTPair();
      Assert::AreEqual(kSampleStmtIdx1, follows_pair.front().first);
      Assert::AreEqual(kSampleStmtIdx2, follows_pair.front().second);
    }

    TEST_METHOD(TestGetFollowsPairTMultipleFollower) {
      FollowsTable follows_t_table;
      follows_t_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_t_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      StmtNumPairList follows_pair = follows_t_table.GetAllFollowsTPair();
      Assert::AreEqual(kSampleStmtIdx1, follows_pair.front().first);
      Assert::AreEqual(kSampleStmtIdx2, follows_pair.front().second);
      Assert::AreEqual(kSampleStmtIdx1, follows_pair.back().first);
      Assert::AreEqual(kSampleStmtIdx3, follows_pair.back().second);
    }

    TEST_METHOD(TestGetFollowsPairEmpty) {
      FollowsTable follows_table;
      StmtNumPairList follows_pair = follows_table.GetAllFollowsPair();
      Assert::IsTrue(follows_pair.empty());
    }
    
    TEST_METHOD(TestGetFollowsPair) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      StmtNumPairList follows_pair = follows_table.GetAllFollowsPair();
      Assert::AreEqual(kSampleStmtIdx1, follows_pair.front().first);
      Assert::AreEqual(kSampleStmtIdx2, follows_pair.front().second);
    }

    TEST_METHOD(TestGetFollowsPairMultipleFollower) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);
      follows_table.InsertFollows(kSampleStmtIdx4, kSampleStmtIdx5);
      StmtNumPairList follows_pair = follows_table.GetAllFollowsPair();
      Assert::AreEqual(kSampleStmtIdx1, follows_pair.front().first);
      Assert::AreEqual(kSampleStmtIdx2, follows_pair.front().second);
      Assert::AreEqual(kSampleStmtIdx4, follows_pair.back().first);
      Assert::AreEqual(kSampleStmtIdx5, follows_pair.back().second);
    }
  };
}