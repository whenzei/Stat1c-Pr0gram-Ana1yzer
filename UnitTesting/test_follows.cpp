#include "stdafx.h"
#include "CPPUnitTest.h"
#include "follows.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollows) {
    const StmtNum kSampleStmtIdx1 = "1";
    const StmtNum kSampleStmtIdx2 = "5";
    const StmtNum kSampleStmtIdx3 = "0";
    const StmtNum kSampleStmtIdx4 = "17";

    TEST_METHOD(TestInsertFollowsSingleRel) {
      Follows follows;
      bool result = follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::AreEqual(true, result);
    }

    TEST_METHOD(TestInsertFollowsDuplicateRel) {
      Follows follows;
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      bool result = follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::AreEqual(false, result);
    }

    TEST_METHOD(TestInsertFollowsMultipleRel) {
      Follows follows;
      bool result = follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::AreEqual(true, result);
      result = follows.InsertFollows(kSampleStmtIdx3, kSampleStmtIdx4);
      Assert::AreEqual(true, result);
    }

    TEST_METHOD(TestIsFollows) {
      Follows follows;
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows.IsFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      Assert::AreEqual(true, result);
    }

    TEST_METHOD(TestIsFollowsIncludesDirect) {
      Follows follows;
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows.IsFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::AreEqual(true, result);
    }

    TEST_METHOD(TestIsDirectFollows) {
      Follows follows;
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows.IsDirectFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      Assert::AreEqual(true, result);
      result = follows.IsDirectFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      Assert::AreEqual(false, result);
    }

    TEST_METHOD(TestGetFollows) {
      StmtList expectedFollowers;
      Follows follows;
      expectedFollowers.push_back(kSampleStmtIdx2);
      expectedFollowers.push_back(kSampleStmtIdx3);

      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      StmtList followers = follows.GetFollows(kSampleStmtIdx1);
      Assert::AreEqual(expectedFollowers.front(), followers.front());
      Assert::AreEqual(expectedFollowers.back(), followers.back());
    }

    TEST_METHOD(TestGetDirectFollows) {
      Follows follows;
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx2);
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);

      StmtNum follower = follows.GetDirectFollows(kSampleStmtIdx1);
      Assert::AreEqual(kSampleStmtIdx2, follower);
    }

    TEST_METHOD(TestGetFollowedBy) {
      StmtList expectedFollowees;
      Follows follows;
      expectedFollowees.push_back(kSampleStmtIdx1);
      expectedFollowees.push_back(kSampleStmtIdx2);

      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      follows.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);
      StmtList followees = follows.GetFollowedBy(kSampleStmtIdx3);
      Assert::AreEqual(expectedFollowees.front(), followees.front());
      Assert::AreEqual(expectedFollowees.back(), followees.back());
    }

    TEST_METHOD(TestGetDirectFollowedBy) {
      Follows follows;
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      follows.InsertFollows(kSampleStmtIdx2, kSampleStmtIdx3);

      StmtNum followee = follows.GetDirectFollowedBy(kSampleStmtIdx3);
      Assert::AreEqual(kSampleStmtIdx1, followee);
    }

    TEST_METHOD(TestHasFollowsRelationship) {
      Follows follows;
      follows.InsertFollows(kSampleStmtIdx1, kSampleStmtIdx3);
      bool result = follows.HasFollowsRelationship();
      Assert::AreEqual(true, result);
    }

    TEST_METHOD(TestHasFollowsRelationshipEmpty) {
      Follows follows;
      bool result = follows.HasFollowsRelationship();
      Assert::AreEqual(false, result);
    }
  };
}