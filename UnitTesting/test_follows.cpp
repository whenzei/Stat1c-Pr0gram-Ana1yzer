#include "stdafx.h"
#include "CPPUnitTest.h"
#include "follows_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowsTable) {
    const StmtNum kSampleStmtIdx1 = "1";
    const StmtNum kSampleStmtIdx2 = "5";
    const StmtNum kSampleStmtIdx3 = "0";
    const StmtNum kSampleStmtIdx4 = "17";

    TEST_METHOD(TestInsertFollowsSingleRel) {
      FollowsTable follows_table;
      bool result = follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::IsTrue(result);
    }

    TEST_METHOD(TestInsertFollowsDuplicateRel) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      bool result = follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::IsFalse(result);
    }

    TEST_METHOD(TestInsertFollowsMultipleRel) {
      FollowsTable follows_table;
      bool result = follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::IsTrue(result);
      result = follows_table.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);
      Assert::IsTrue(result);
    }

    TEST_METHOD(TestIsFollows) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows_table.IsFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      Assert::IsTrue(result);
    }

    TEST_METHOD(TestIsFollowsIncludesDirect) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows_table.IsFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::IsTrue(result);
    }

    TEST_METHOD(TestIsDirectFollows) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows_table.IsDirectFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::IsTrue(result);
      result = follows_table.IsDirectFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      Assert::IsTrue(result);
    }

    TEST_METHOD(TestGetFollows) {
      StmtList expected_followers;
      FollowsTable follows_table;
      expected_followers.push_back(kSampleStmtIdx2);
      expected_followers.push_back(kSampleStmtIdx3);

      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      StmtList followers = follows_table.GetFollows(kSampleStmtIdx1);
      Assert::AreEqual(expected_followers.front(), followers.front());
      Assert::AreEqual(expected_followers.back(), followers.back());
    }

    TEST_METHOD(TestGetDirectFollows) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);

      StmtNum follower = follows_table.GetDirectFollows(kSampleStmtIdx1);
      Assert::AreEqual(kSampleStmtIdx2, follower);
    }

    TEST_METHOD(TestGetFollowedBy) {
      StmtList expected_followees;
      FollowsTable follows_table;
      expected_followees.push_back(kSampleStmtIdx1);
      expected_followees.push_back(kSampleStmtIdx2);

      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      StmtList followees = follows_table.GetFollowedBy(kSampleStmtIdx3);
      Assert::AreEqual(expected_followees.front(), followees.front());
      Assert::AreEqual(expected_followees.back(), followees.back());
    }

    TEST_METHOD(TestGetDirectFollowedBy) {
      FollowsTable follows_table;
      follows_table.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      follows_table.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);

      StmtNum followee = follows_table.GetDirectFollowedBy(kSampleStmtIdx3);
      Assert::AreEqual(kSampleStmtIdx1, followee);
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
  };
}