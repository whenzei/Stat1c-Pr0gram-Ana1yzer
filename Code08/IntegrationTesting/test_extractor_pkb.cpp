#include "stdafx.h"
#include "CppUnitTest.h"
#include "parser.h"
#include "pkb.h"
#include "pql_extractor.h"

#include <list>
#include <set>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::pair;

namespace PkbPqlExtractorTests {
TEST_CLASS(TestPkbPqlExtractor) {
  const string kProcName1 = "one";
  const string kProcName2 = "two";
  PKB pkb1 = GetTestPKBOne();
  PKB pkb2 = GetTestPKBTwo();
  PKB pkb3 = GetTestPKBThree();
  PKB pkb4 = GetTestPKBFour();
  PKB pkb5 = GetTestPKBFive();
  PKB pkb6 = GetTestPKBSix();
  PKB pkb7 = GetTestPKBSeven();

 public:
  /* NextExtractor Tests*/
  TEST_METHOD(TestIsNextTTwoParams) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positives
    Assert::IsTrue(extractor.IsNextT(1, 7));
    Assert::IsTrue(extractor.IsNextT(1, 4));
    Assert::IsTrue(extractor.IsNextT(1, 6));
    Assert::IsTrue(extractor.IsNextT(1, 1));

    // Negatives
    Assert::IsFalse(extractor.IsNextT(12, 13));
    Assert::IsFalse(extractor.IsNextT(9, 8));
    Assert::IsFalse(extractor.IsNextT(11, 11));
    Assert::IsFalse(extractor.IsNextT(11, 222));
    Assert::IsFalse(extractor.IsNextT(222, 11));
    Assert::IsFalse(extractor.IsNextT(222, 222));
  }

  TEST_METHOD(TestIsNextTOneParam) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positives
    Assert::IsTrue(extractor.IsNextT(12));
    Assert::IsTrue(extractor.IsNextT(6));
    Assert::IsTrue(extractor.IsNextT(4));

    // Negatives
    Assert::IsFalse(extractor.IsNextT(13));
    Assert::IsFalse(extractor.IsNextT(2222));
  }

  TEST_METHOD(TestIsPreviousT) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positive
    Assert::IsTrue(extractor.IsPreviousT(1));
    Assert::IsTrue(extractor.IsPreviousT(3));
    Assert::IsTrue(extractor.IsPreviousT(4));
    Assert::IsTrue(extractor.IsPreviousT(6));

    // Negatives
    Assert::IsFalse(extractor.IsPreviousT(12));
    Assert::IsFalse(extractor.IsPreviousT(1000));
    Assert::IsFalse(extractor.IsPreviousT(13));
  }

  TEST_METHOD(TestGetPreviousT) {
    PqlExtractor extractor = PqlExtractor(&pkb2);

    // Positive
    StmtNumList test_result_1 = extractor.GetPreviousT(8);
    StmtNumList expected_result_1 =
        StmtNumList{12, 7, 11, 4, 10, 3, 9, 2, 8, 1};
    Assert::IsTrue(test_result_1 == expected_result_1);

    StmtNumList test_result_2 = extractor.GetPreviousT(6);
    StmtNumList expected_result_2 = StmtNumList{5, 4, 3, 2, 1};
    Assert::IsTrue(test_result_2 == expected_result_2);

    // Negative
    StmtNumList test_result_3 = extractor.GetPreviousT(1);
    StmtNumList expected_result_3 = StmtNumList{};
    Assert::IsTrue(test_result_3 == expected_result_3);

    StmtNumList test_result_4 = extractor.GetPreviousT(222);
    StmtNumList expected_result_4 = StmtNumList{};
    Assert::IsTrue(test_result_4 == expected_result_4);
  }

  TEST_METHOD(TestGetNextT) {
    PqlExtractor extractor = PqlExtractor(&pkb2);

    // Positive
    StmtNumList test_result_1 = extractor.GetNextT(9);
    StmtNumList expected_result_1 = StmtNumList{10, 11, 12, 8, 9, 13};
    Assert::IsTrue(test_result_1 == expected_result_1);

    StmtNumList test_result_2 = extractor.GetNextT(5);
    StmtNumList expected_result_2 = StmtNumList{6, 13};
    Assert::IsTrue(test_result_2 == expected_result_2);

    // Negative
    StmtNumList test_result_3 = extractor.GetNextT(13);
    StmtNumList expected_result_3 = StmtNumList{};
    Assert::IsTrue(test_result_3 == expected_result_3);

    StmtNumList test_result_4 = extractor.GetNextT(222);
    StmtNumList expected_result_4 = StmtNumList{};
    Assert::IsTrue(test_result_4 == expected_result_4);
  }

  TEST_METHOD(TestGetNextTMap) {
    PqlExtractor extractor = PqlExtractor(&pkb7);

    // Positive
    NextTMap actual_result_1 = extractor.GetNextTMap();
    NextTMap expected_result_1;

    expected_result_1.emplace(1, VertexSet{1, 2, 3, 4, 5, 6});
    expected_result_1.emplace(2, VertexSet{1, 2, 3, 4, 5, 6});
    expected_result_1.emplace(3, VertexSet{1, 2, 3, 4, 5, 6});
    expected_result_1.emplace(4, VertexSet{1, 2, 3, 4, 5, 6});
    expected_result_1.emplace(5, VertexSet{1, 2, 3, 4, 5, 6});

    // add empty set for vertices without nextT
    expected_result_1.emplace(6, VertexSet());

    Assert::IsTrue(expected_result_1 == actual_result_1);
  }

  TEST_METHOD(TestGetTypedNextTMap) {
    PqlExtractor extractor = PqlExtractor(&pkb7);

    // Assign stmt
    NextTMap actual_result_1 = extractor.GetAssignNextTMap();
    NextTMap expected_result_1;
    // both 5 and 6 are assign stmts, but only 5 have nextTs
    expected_result_1.emplace(5, VertexSet{1, 2, 3, 4, 5, 6});
    expected_result_1.emplace(6, VertexSet());
    Assert::IsTrue(expected_result_1 == actual_result_1);

    // While stmt + test caching, should be fast
    NextTMap actual_result_2 = extractor.GetWhileNextTMap();
    NextTMap expected_result_2;
    // only 1 is while stmt
    expected_result_2.emplace(1, VertexSet{1, 2, 3, 4, 5, 6});
    Assert::IsTrue(expected_result_2 == actual_result_2);

    // If stmt + test caching, should be fast
    NextTMap actual_result_3 = extractor.GetIfNextTMap();
    NextTMap expected_result_3;
    // only 2 is if stmt
    expected_result_3.emplace(2, VertexSet{1, 2, 3, 4, 5, 6});
    Assert::IsTrue(expected_result_3 == actual_result_3);

    // Read stmt + test caching, should be fast
    NextTMap actual_result_4 = extractor.GetReadNextTMap();
    NextTMap expected_result_4;
    // both 3 and 4 are read stmts
    expected_result_4.emplace(3, VertexSet{1, 2, 3, 4, 5, 6});
    expected_result_4.emplace(4, VertexSet{1, 2, 3, 4, 5, 6});
    Assert::IsTrue(expected_result_4 == actual_result_4);

    // Negative tests, should have no calls
    NextTMap actual_result_5 = extractor.GetCallNextTMap();
    Assert::IsTrue(actual_result_5 == NextTMap());
  }

  /* AffectsExtractor - Affects Tests */
  TEST_METHOD(TestIsAffectsTwoParams) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffects(4, 4);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffects(4, 6);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffects(4, 9);
    Assert::IsTrue(test_result_3);

    // Negatives********************************************

    // Variable of 2 is modified before reaching statement 8
    bool test_result_4 = extractor.IsAffects(2, 8);
    Assert::IsFalse(test_result_4);

    // Encounters read statement that modifies variable of stmt 9
    bool test_result_5 = extractor.IsAffects(9, 11);
    Assert::IsFalse(test_result_5);

    // Stmts in different procedures
    bool test_result_6 = extractor.IsAffects(11, 12);
    Assert::IsFalse(test_result_6);

    // Both stmts are not found in the program
    bool test_result_7 = extractor.IsAffects(22, 100);
    Assert::IsFalse(test_result_7);

    // Both statements are not assignment statements
    bool test_result_8 = extractor.IsAffects(7, 10);
    Assert::IsFalse(test_result_8);

    // Left statement is not assignment statement
    bool test_result_9 = extractor.IsAffects(5, 8);
    Assert::IsFalse(test_result_9);

    // Right statement is not assignment statement
    bool test_result_10 = extractor.IsAffects(8, 10);
    Assert::IsFalse(test_result_10);

    // Modified variable in left statement is not used in right statement
    bool test_result_11 = extractor.IsAffects(2, 4);
    Assert::IsFalse(test_result_11);
  }
  TEST_METHOD(TestIsAffectsOneParam) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffects(8);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffects(4);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffects(6);
    Assert::IsTrue(test_result_3);

    // Negatives********************************************

    // While statement
    bool test_result_4 = extractor.IsAffects(1);
    Assert::IsFalse(test_result_4);

    // If statement
    bool test_result_5 = extractor.IsAffects(3);
    Assert::IsFalse(test_result_5);

    // Print statement
    bool test_result_6 = extractor.IsAffects(5);
    Assert::IsFalse(test_result_6);

    // Stmt is not found in the program
    bool test_result_7 = extractor.IsAffects(22);
    Assert::IsFalse(test_result_7);

    // Variable is modified by call stmt before reaching any assignment
    // statements that use it
    bool test_result_8 = extractor.IsAffects(2);
    Assert::IsFalse(test_result_8);

    // Variable is modified by read stmt before reaching any assignment
    // statements that use it
    bool test_result_9 = extractor.IsAffects(9);
    Assert::IsFalse(test_result_9);
  }
  TEST_METHOD(TestIsAffected) {
    PqlExtractor extractor = PqlExtractor(&pkb3);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffected(15);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffected(12);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffected(9);
    Assert::IsTrue(test_result_3);

    bool test_result_4 = extractor.IsAffected(14);
    Assert::IsTrue(test_result_4);

    // Negatives *******************************
    bool test_result_5 = extractor.IsAffected(10);
    Assert::IsFalse(test_result_5);

    bool test_result_6 = extractor.IsAffected(5);
    Assert::IsFalse(test_result_6);

    bool test_result_7 = extractor.IsAffected(3);
    Assert::IsFalse(test_result_7);

    // Load new program
    extractor = PqlExtractor(&pkb6);

    bool test_result_8 = extractor.IsAffected(6);
    Assert::IsFalse(test_result_8);
  }

  TEST_METHOD(TestGetAffects) {
    PqlExtractor extractor = PqlExtractor(&pkb2);

    // Positives**************************************
    VertexSet test_result_1 = extractor.GetAffects(1);
    VertexSet expected_result_1 = VertexSet{5, 6, 10};
    Assert::IsTrue(test_result_1 == expected_result_1);

    VertexSet test_result_2 = extractor.GetAffects(12);
    VertexSet expected_result_2 = VertexSet{12, 13};
    Assert::IsTrue(test_result_2 == expected_result_2);

    VertexSet test_result_3 = extractor.GetAffects(10);
    VertexSet expected_result_3 = VertexSet{9, 10};
    Assert::IsTrue(test_result_3 == expected_result_3);

    // Negatives***************************************

    // Stmt 2 is a read stmt
    VertexSet test_result_4 = extractor.GetAffects(2);
    VertexSet expected_result_4 = VertexSet{};
    Assert::IsTrue(test_result_4 == expected_result_4);

    // Stmt 9 rhs var is modified
    VertexSet test_result_5 = extractor.GetAffects(9);
    VertexSet expected_result_5 = VertexSet{};
    Assert::IsTrue(test_result_5 == expected_result_5);

    // Stmt 8 is a while stmt
    VertexSet test_result_6 = extractor.GetAffects(8);
    VertexSet expected_result_6 = VertexSet{};
    Assert::IsTrue(test_result_6 == expected_result_6);

    // Stmt is not found in program
    VertexSet test_result_7 = extractor.GetAffects(55);
    VertexSet expected_result_7 = VertexSet{};
    Assert::IsTrue(test_result_7 == expected_result_7);
  }
  TEST_METHOD(TestGetAffectedByWhileInIf) {
    PqlExtractor extractor = PqlExtractor(&pkb3);

    // Positives**************************************
    VertexSet test_result_1 = extractor.GetAffectedBy(15);
    VertexSet expected_result_1 = VertexSet{6, 5, 3, 1, 14, 11, 9, 7};
    Assert::IsTrue(expected_result_1 == test_result_1);

    VertexSet test_result_2 = extractor.GetAffectedBy(12);
    VertexSet expected_result_2 = VertexSet{12, 9};
    Assert::IsTrue(expected_result_2 == test_result_2);

    VertexSet test_result_3 = extractor.GetAffectedBy(6);
    VertexSet expected_result_3 = VertexSet{5};
    Assert::IsTrue(expected_result_3 == test_result_3);

    VertexSet test_result_4 = extractor.GetAffectedBy(11);
    VertexSet expected_result_4 = VertexSet{12};
    Assert::IsTrue(expected_result_4 == test_result_4);

    // Negatives**************************************

    // First statement is not affected by any other statement
    VertexSet test_result_5 = extractor.GetAffectedBy(1);
    VertexSet expected_result_5 = VertexSet{};
    Assert::IsTrue(expected_result_5 == test_result_5);

    // If statement
    VertexSet test_result_6 = extractor.GetAffectedBy(4);
    VertexSet expected_result_6 = VertexSet{};
    Assert::IsTrue(expected_result_6 == test_result_6);

    // Stmt 7 not affected by any other statement
    VertexSet test_result_7 = extractor.GetAffectedBy(7);
    VertexSet expected_result_7 = VertexSet{};
    Assert::IsTrue(expected_result_7 == test_result_7);

    // Additional test: all vars modified by non assignment statement before
    // reaching an affecting assignment statement
    extractor = PqlExtractor(&pkb6);
    VertexSet test_result_8 = extractor.GetAffectedBy(6);
    VertexSet expected_result_8 = VertexSet{};
    Assert::IsTrue(expected_result_8 == test_result_8);
  }

  TEST_METHOD(TestGetAllAffects) {
    PqlExtractor extractor = PqlExtractor(&pkb1);
    VertexSet actual_results_1 = extractor.GetAllAffects();
    VertexSet expected_results_1 = VertexSet{4, 6, 8};
    Assert::IsTrue(actual_results_1 == expected_results_1);

    extractor = PqlExtractor(&pkb2);
    VertexSet actual_results_2 = extractor.GetAllAffects();
    VertexSet expected_results_2 = VertexSet{1, 6, 7, 10, 12};
    Assert::IsTrue(actual_results_2 == expected_results_2);

    extractor = PqlExtractor(&pkb3);
    VertexSet actual_results_3 = extractor.GetAllAffects();
    VertexSet expected_results_3 = VertexSet{1, 3, 5, 6, 7, 9, 11, 12, 14};
    Assert::IsTrue(actual_results_3 == expected_results_3);

    extractor = PqlExtractor(&pkb4);
    VertexSet actual_results_4 = extractor.GetAllAffects();
    VertexSet expected_results_4 = VertexSet{1, 5, 7, 8, 9};
    Assert::IsTrue(actual_results_4 == expected_results_4);
  }

  TEST_METHOD(TestGetAllAffectedBy) {}

  TEST_METHOD(TestGetAffectsTable) {
    // test while-if loop
    PqlExtractor extractor = PqlExtractor(&pkb1);
    AffectsMap actual_results_1 = extractor.GetAffectsMap();
    AffectsMap expected_results_1;
    // variable t is modified at 4 / 6, used at 4, 6, 9, 12 without being
    // modified again
    expected_results_1.emplace(4, VertexSet{4, 6, 9, 12});
    expected_results_1.emplace(6, VertexSet{4, 6, 9, 12});
    // variable g is modified at 8 and used at 12 without being modified again
    expected_results_1.emplace(8, VertexSet{12});

    // add empty set for nodes in graph but doesnt affect anything
    expected_results_1.emplace(9, VertexSet());
    expected_results_1.emplace(12, VertexSet());

    // variable a and f are both modified again by non-assign statements and
    // thus have no results
    Assert::IsTrue(actual_results_1 == expected_results_1);

    // test if-while loop
    extractor = PqlExtractor(&pkb2);
    AffectsMap actual_results_2 = extractor.GetAffectsMap();
    AffectsMap expected_results_2;

    expected_results_2.emplace(1, VertexSet{5, 6, 10});
    expected_results_2.emplace(6, VertexSet{13});
    expected_results_2.emplace(7, VertexSet{10});
    expected_results_2.emplace(10, VertexSet{9, 10});
    expected_results_2.emplace(12, VertexSet{12, 13});

    // add empty set for nodes in graph but doesnt affect anything
    expected_results_2.emplace(5, VertexSet());
    expected_results_2.emplace(13, VertexSet());
    expected_results_2.emplace(9, VertexSet());

    Assert::IsTrue(actual_results_2 == expected_results_2);

    // test while-while loop
    extractor = PqlExtractor(&pkb4);
    AffectsMap actual_results_3 = extractor.GetAffectsMap();
    AffectsMap expected_results_3;

    expected_results_3.emplace(1, VertexSet{3, 12});
    expected_results_3.emplace(5, VertexSet{3, 12});
    expected_results_3.emplace(7, VertexSet{5, 9});
    expected_results_3.emplace(8, VertexSet{7});
    expected_results_3.emplace(9, VertexSet{8});

    // add empty set for nodes in graph but doesnt affect anything
    expected_results_3.emplace(3, VertexSet());
    expected_results_3.emplace(12, VertexSet());

    Assert::IsTrue(actual_results_3 == expected_results_3);
  }

  TEST_METHOD(TestGetAffectedByMap) {
    // test while-if loop
    PqlExtractor extractor = PqlExtractor(&pkb1);
    AffectsMap actual_results_1 = extractor.GetAffectedByMap();
    AffectsMap expected_results_1;
    // Should be reversed of GetAffectsMap
    expected_results_1.emplace(4, VertexSet{4, 6});
    expected_results_1.emplace(6, VertexSet{4, 6});
    expected_results_1.emplace(9, VertexSet{4, 6});
    expected_results_1.emplace(12, VertexSet{4, 6, 8});

    // add empty set for nodes in graph but doesnt affect anything
    expected_results_1.emplace(8, VertexSet());

    Assert::IsTrue(actual_results_1 == expected_results_1);

    // test if-while loop
    extractor = PqlExtractor(&pkb2);
    AffectsMap actual_results_2 = extractor.GetAffectedByMap();
    AffectsMap expected_results_2;

    expected_results_2.emplace(5, VertexSet{1});
    expected_results_2.emplace(6, VertexSet{1});
    expected_results_2.emplace(10, VertexSet{1, 7, 10});
    expected_results_2.emplace(13, VertexSet{6, 12});
    expected_results_2.emplace(9, VertexSet{10});
    expected_results_2.emplace(12, VertexSet{12});

    // add empty set for nodes in graph but doesnt affect anything
    expected_results_2.emplace(1, VertexSet());
    expected_results_2.emplace(7, VertexSet());

    Assert::IsTrue(actual_results_2 == expected_results_2);

    // test while-while loop
    extractor = PqlExtractor(&pkb4);
    AffectsMap actual_results_3 = extractor.GetAffectedByMap();
    AffectsMap expected_results_3;

    expected_results_3.emplace(3, VertexSet{1, 5});
    expected_results_3.emplace(12, VertexSet{1, 5});
    expected_results_3.emplace(5, VertexSet{7});
    expected_results_3.emplace(9, VertexSet{7});
    expected_results_3.emplace(7, VertexSet{8});
    expected_results_3.emplace(8, VertexSet{9});

    // add empty set for nodes in graph but doesnt affect anything
    expected_results_3.emplace(1, VertexSet());

    Assert::IsTrue(actual_results_3 == expected_results_3);
  }

  TEST_METHOD(TestGetAffectsBipMap) {
    PqlExtractor extractor = PqlExtractor(&pkb5);
    AffectsMap actual_results = extractor.GetAffectsBipMap();
    AffectsMap expected_results;

    expected_results.emplace(1, VertexSet{6, 10, 11, 8, 3});
    expected_results.emplace(3, VertexSet{5});
    expected_results.emplace(4, VertexSet{5});
    expected_results.emplace(6, VertexSet{11, 8, 10});
    expected_results.emplace(10, VertexSet{8, 3});

    // add empty set for nodes in graph but doesnt affect anything
    expected_results.emplace(11, VertexSet());
    expected_results.emplace(8, VertexSet());
    expected_results.emplace(5, VertexSet());

    Assert::IsTrue(actual_results == expected_results);
  }

  TEST_METHOD(TestIsAffectsBipTwoParam) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    bool test_result_1 = extractor.IsAffectsBip(1, 6);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectsBip(10, 8);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffectsBip(6, 10);
    Assert::IsTrue(test_result_3);

    bool test_result_4 = extractor.IsAffectsBip(1, 3);
    Assert::IsTrue(test_result_4);

    bool test_result_5 = extractor.IsAffectsBip(1, 2);
    Assert::IsFalse(test_result_5);

    bool test_result_6 = extractor.IsAffectsBip(7, 8);
    Assert::IsFalse(test_result_6);

    bool test_result_7 = extractor.IsAffectsBip(10, 11);
    Assert::IsFalse(test_result_7);
  }

  TEST_METHOD(TestIsAffectsBipOneParam) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    bool test_result_1 = extractor.IsAffectsBip(1);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectsBip(10);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffectsBip(6);
    Assert::IsTrue(test_result_3);

    bool test_result_4 = extractor.IsAffectsBip(1);
    Assert::IsTrue(test_result_4);

    bool test_result_5 = extractor.IsAffectsBip(8);
    Assert::IsFalse(test_result_5);

    bool test_result_6 = extractor.IsAffectsBip(11);
    Assert::IsFalse(test_result_6);

    bool test_result_7 = extractor.IsAffectsBip(22);
    Assert::IsFalse(test_result_7);

    bool test_result_8 = extractor.IsAffectsBip(5);
    Assert::IsFalse(test_result_8);
  }

  TEST_METHOD(TestIsAffectedBip) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    // Positives***********************************
    bool test_result_1 = extractor.IsAffectedBip(11);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectedBip(10);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffectedBip(6);
    Assert::IsTrue(test_result_3);

    bool test_result_4 = extractor.IsAffectedBip(5);
    Assert::IsTrue(test_result_4);

    // Negatives***********************************
    bool test_result_5 = extractor.IsAffectedBip(2);
    Assert::IsFalse(test_result_5);

    bool test_result_6 = extractor.IsAffectedBip(1);
    Assert::IsFalse(test_result_6);

    bool test_result_7 = extractor.IsAffectedBip(4);
    Assert::IsFalse(test_result_7);
  }

  TEST_METHOD(TestGetAffectedByIfInWhile) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positives**************************************
    VertexSet test_result_1 = extractor.GetAffectedBy(9);
    VertexSet expected_result_1 = VertexSet{4, 6};
    Assert::IsTrue(expected_result_1 == test_result_1);

    VertexSet test_result_2 = extractor.GetAffectedBy(12);
    VertexSet expected_result_2 = VertexSet{8, 4, 6};
    Assert::IsTrue(expected_result_2 == test_result_2);

    VertexSet test_result_3 = extractor.GetAffectedBy(4);
    VertexSet expected_result_3 = VertexSet{4, 6};
    Assert::IsTrue(expected_result_3 == test_result_3);

    // Negatives**************************************

    // Print statement
    VertexSet test_result_4 = extractor.GetAffectedBy(5);
    VertexSet expected_result_4 = VertexSet{};
    Assert::IsTrue(expected_result_4 == test_result_4);

    // RHS var of stmt 8 is modified by statement before
    VertexSet test_result_5 = extractor.GetAffectedBy(8);
    VertexSet expected_result_5 = VertexSet{};
    Assert::IsTrue(expected_result_5 == test_result_5);

    // While statement
    VertexSet test_result_6 = extractor.GetAffectedBy(1);
    VertexSet expected_result_6 = VertexSet{};
    Assert::IsTrue(expected_result_6 == test_result_6);
  }

  /* AffectsExtractor - AffectsT Tests */

  TEST_METHOD(TestIsAffectsT) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positives****************************************

    bool test_result_1 = extractor.IsAffectsT(4, 6);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectsT(4, 9);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffectsT(6, 4);
    Assert::IsTrue(test_result_3);

    bool test_result_4 = extractor.IsAffectsT(6, 12);
    Assert::IsTrue(test_result_4);

    // Negatives********************************************

    // Both stmts are not found in the program
    bool test_result_5 = extractor.IsAffectsT(33, 120);
    Assert::IsFalse(test_result_5);

    // Both statements are not assignment statements
    bool test_result_6 = extractor.IsAffectsT(3, 1);
    Assert::IsFalse(test_result_6);
  }

  TEST_METHOD(TestIsAffectsTOneParam) {
    PqlExtractor extractor = PqlExtractor(&pkb1);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffectsT(6);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectsT(4);
    Assert::IsTrue(test_result_2);

    // Negatives****************************************

    // Value of f is overwritten at stmt 10
    bool test_result_3 = extractor.IsAffectsT(9);
    Assert::IsFalse(test_result_3);

    // Not an assign statement
    bool test_result_4 = extractor.IsAffectsT(1);
    Assert::IsFalse(test_result_4);

    // 11 doesn't affect anyone
    bool test_result_5 = extractor.IsAffectsT(11);
    Assert::IsFalse(test_result_5);
  }

  TEST_METHOD(TestIsAffectedT) {
    PqlExtractor extractor = PqlExtractor(&pkb3);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffectedT(6);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectedT(11);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.IsAffectedT(9);
    Assert::IsTrue(test_result_3);

    // Negatives****************************************

    // Not an assign statement
    bool test_result_4 = extractor.IsAffectedT(13);
    Assert::IsFalse(test_result_4);

    // 3 is not affected by anyone
    bool test_result_5 = extractor.IsAffectedT(3);
    Assert::IsFalse(test_result_5);
  }

  TEST_METHOD(TestIsAffectsBipT) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffectsBipT(6, 11);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectsBipT(1, 3);
    Assert::IsTrue(test_result_2);

    // Negatives****************************************
    // Not in an affecting relationship
    bool test_result_3 = extractor.IsAffectsBipT(3, 6);
    Assert::IsFalse(test_result_3);

    // Both are invalid
    bool test_result_4 = extractor.IsAffectsBipT(9, 12);
    Assert::IsFalse(test_result_4);
  }

  TEST_METHOD(TestIsAffectsBipTOneParam) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffectsBipT(6);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectsBipT(1);
    Assert::IsTrue(test_result_1);

    // Negatives****************************************
    // Not in an affecting relationship
    bool test_result_3 = extractor.IsAffectsBipT(5);
    Assert::IsFalse(test_result_3);

    // 7 is a call stmt
    bool test_result_4 = extractor.IsAffectsBipT(7);
    Assert::IsFalse(test_result_4);
  }

  TEST_METHOD(TestIsAffectedBipT) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    // Positives****************************************
    bool test_result_1 = extractor.IsAffectedBipT(8);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.IsAffectedBipT(6);
    Assert::IsTrue(test_result_1);

    bool test_result_3 = extractor.IsAffectedBipT(5);
    Assert::IsTrue(test_result_3);

    // Negatives****************************************
    // Not in an affecting relationship
    bool test_result_4 = extractor.IsAffectedBipT(4);
    Assert::IsFalse(test_result_4);

    // 2 is a call stmt
    bool test_result_5 = extractor.IsAffectedBipT(2);
    Assert::IsFalse(test_result_5);
  }

  TEST_METHOD(TestGetAffectsT) {
    PqlExtractor extractor = PqlExtractor(&pkb2);

    // Positives**************************************
    VertexSet test_result_1 = extractor.GetAffectsT(1);
    VertexSet expected_result_1 = VertexSet{5, 6, 9, 10, 13};
    Assert::IsTrue(test_result_1 == expected_result_1);

    VertexSet test_result_2 = extractor.GetAffectsT(7);
    VertexSet expected_result_2 = VertexSet{9, 10};
    Assert::IsTrue(test_result_2 == expected_result_2);

    VertexSet test_result_3 = extractor.GetAffectsT(12);
    VertexSet expected_result_3 = VertexSet{12, 13};
    Assert::IsTrue(test_result_3 == expected_result_3);

    // Negatives***************************************

    // Stmt 3 is a print stmt
    VertexSet test_result_4 = extractor.GetAffectsT(3);
    VertexSet expected_result_4 = VertexSet{};
    Assert::IsTrue(test_result_4 == expected_result_4);

    // Stmt 13 is the last statement in the procedure
    VertexSet test_result_5 = extractor.GetAffectsT(13);
    VertexSet expected_result_5 = VertexSet{};
    Assert::IsTrue(test_result_5 == expected_result_5);

    // Stmt 4 is a if stmt
    VertexSet test_result_6 = extractor.GetAffectsT(4);
    VertexSet expected_result_6 = VertexSet{};
    Assert::IsTrue(test_result_6 == expected_result_6);

    // Stmt 80 is not found in program
    VertexSet test_result_7 = extractor.GetAffectsT(80);
    VertexSet expected_result_7 = VertexSet{};
    Assert::IsTrue(test_result_7 == expected_result_7);
  }

  TEST_METHOD(TestGetAffectedByT) {
    PqlExtractor extractor = PqlExtractor(&pkb2);

    // Positives**************************************
    VertexSet test_result_1 = extractor.GetAffectedByT(9);
    VertexSet expected_result_1 = VertexSet{1, 7, 10};
    Assert::IsTrue(test_result_1 == expected_result_1);

    VertexSet test_result_2 = extractor.GetAffectedByT(12);
    VertexSet expected_result_2 = VertexSet{12};
    Assert::IsTrue(test_result_2 == expected_result_2);

    VertexSet test_result_3 = extractor.GetAffectedByT(13);
    VertexSet expected_result_3 = VertexSet{1, 6, 12};
    Assert::IsTrue(test_result_3 == expected_result_3);

    // Negatives***************************************

    // Stmt 2 is a read stmt
    VertexSet test_result_4 = extractor.GetAffectedByT(2);
    VertexSet expected_result_4 = VertexSet{};
    Assert::IsTrue(test_result_4 == expected_result_4);

    // Stmt 8 is a while stmt
    VertexSet test_result_5 = extractor.GetAffectedByT(8);
    VertexSet expected_result_5 = VertexSet{};
    Assert::IsTrue(test_result_5 == expected_result_5);

    // Stmt 92 is not found in the program
    VertexSet test_result_6 = extractor.GetAffectedByT(92);
    VertexSet expected_result_6 = VertexSet{};
    Assert::IsTrue(test_result_6 == expected_result_6);
  }

  TEST_METHOD(TestGetAffectsBipT) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    // Positives**************************************
    VertexSet test_result_1 = extractor.GetAffectsBipT(1);
    VertexSet expected_result_1 = VertexSet{3, 5, 6, 8, 10, 11};
    Assert::IsTrue(test_result_1 == expected_result_1);

    VertexSet test_result_2 = extractor.GetAffectsBipT(6);
    VertexSet expected_result_2 = VertexSet{3, 5, 8, 10, 11};
    Assert::IsTrue(test_result_2 == expected_result_2);

    // Negatives***************************************

    // Stmt 7 is a call stmt
    VertexSet test_result_3 = extractor.GetAffectsBipT(7);
    VertexSet expected_result_3 = VertexSet{};
    Assert::IsTrue(test_result_3 == expected_result_3);

    // Stmt 11 does not affect anyone
    VertexSet test_result_4 = extractor.GetAffectsBipT(11);
    VertexSet expected_result_4 = VertexSet{};
    Assert::IsTrue(test_result_4 == expected_result_4);
  }

  TEST_METHOD(TestGetAffectedByBipT) {
    PqlExtractor extractor = PqlExtractor(&pkb5);

    // Positives**************************************
    VertexSet test_result_1 = extractor.GetAffectedByBipT(8);
    VertexSet expected_result_1 = VertexSet{1, 6, 10};
    Assert::IsTrue(test_result_1 == expected_result_1);

    VertexSet test_result_2 = extractor.GetAffectedByBipT(11);
    VertexSet expected_result_2 = VertexSet{1, 6};
    Assert::IsTrue(test_result_2 == expected_result_2);

    // Negatives***************************************

    // Stmt 9 is a if stmt
    VertexSet test_result_3 = extractor.GetAffectedByBipT(9);
    VertexSet expected_result_3 = VertexSet{};
    Assert::IsTrue(test_result_3 == expected_result_3);

    // Stmt 4 is not affected by anyone
    VertexSet test_result_4 = extractor.GetAffectedByBipT(4);
    VertexSet expected_result_4 = VertexSet{};
    Assert::IsTrue(test_result_4 == expected_result_4);
  }

  TEST_METHOD(TestGetAllAffectsT) {
    PqlExtractor extractor_1 = PqlExtractor(&pkb1);
    VertexSet test_result_1 = extractor_1.GetAllAffectsT();
    VertexSet expected_result_1 = VertexSet{4, 6, 8};
    Assert::IsTrue(test_result_1 == expected_result_1);

    PqlExtractor extractor_2 = PqlExtractor(&pkb2);
    VertexSet test_result_2 = extractor_2.GetAllAffectsT();
    VertexSet expected_result_2 = VertexSet{1, 6, 7, 10, 12};
    Assert::IsTrue(test_result_2 == expected_result_2);

    PqlExtractor extractor_3 = PqlExtractor(&pkb3);
    VertexSet test_result_3 = extractor_3.GetAllAffectsT();
    VertexSet expected_result_3 = VertexSet{1, 3, 5, 6, 7, 9, 11, 12, 14};
    Assert::IsTrue(test_result_3 == expected_result_3);

    PqlExtractor extractor_4 = PqlExtractor(&pkb4);
    VertexSet test_result_4 = extractor_4.GetAllAffectsT();
    VertexSet expected_result_4 = VertexSet{1, 5, 7, 8, 9};
    Assert::IsTrue(test_result_4 == expected_result_4);
  }

  TEST_METHOD(TestGetAllAffectedByT) {
    PqlExtractor extractor_1 = PqlExtractor(&pkb1);
    VertexSet test_result_1 = extractor_1.GetAllAffectedByT();
    VertexSet expected_result_1 = VertexSet{4, 6, 9, 12};
    Assert::IsTrue(test_result_1 == expected_result_1);

    PqlExtractor extractor_2 = PqlExtractor(&pkb2);
    VertexSet test_result_2 = extractor_2.GetAllAffectedByT();
    VertexSet expected_result_2 = VertexSet{5, 6, 9, 10, 12, 13};
    Assert::IsTrue(test_result_2 == expected_result_2);

    PqlExtractor extractor_3 = PqlExtractor(&pkb3);
    VertexSet test_result_3 = extractor_3.GetAllAffectedByT();
    VertexSet expected_result_3 = VertexSet{6, 9, 11, 12, 14, 15};
    Assert::IsTrue(test_result_3 == expected_result_3);

    PqlExtractor extractor_4 = PqlExtractor(&pkb4);
    VertexSet test_result_4 = extractor_4.GetAllAffectedByT();
    VertexSet expected_result_4 = VertexSet{3, 5, 7, 8, 9, 12};
    Assert::IsTrue(test_result_4 == expected_result_4);
  }

  TEST_METHOD(TestGetAllAffectsBipT) {
    PqlExtractor extractor_1 = PqlExtractor(&pkb1);
    VertexSet test_result_1 = extractor_1.GetAllAffectsBipT();
    VertexSet expected_result_1 = VertexSet{4, 6, 8};
    Assert::IsTrue(test_result_1 == expected_result_1);

    PqlExtractor extractor_2 = PqlExtractor(&pkb5);
    VertexSet test_result_2 = extractor_2.GetAllAffectsBipT();
    VertexSet expected_result_2 = VertexSet{1, 3, 4, 6, 10};
    Assert::IsTrue(test_result_2 == expected_result_2);

    PqlExtractor extractor_3 = PqlExtractor(&pkb6);
    VertexSet test_result_3 = extractor_3.GetAllAffectsBipT();
    VertexSet expected_result_3 = VertexSet{};
    Assert::IsTrue(test_result_3 == expected_result_3);
  }

  TEST_METHOD(TestGetAllAffectedByBipT) {
    PqlExtractor extractor_1 = PqlExtractor(&pkb1);
    VertexSet test_result_1 = extractor_1.GetAllAffectedByBipT();
    VertexSet expected_result_1 = VertexSet{4, 6, 9, 12};
    Assert::IsTrue(test_result_1 == expected_result_1);

    PqlExtractor extractor_2 = PqlExtractor(&pkb5);
    VertexSet test_result_2 = extractor_2.GetAllAffectedByBipT();
    VertexSet expected_result_2 = VertexSet{3, 5, 6, 8, 10, 11};
    Assert::IsTrue(test_result_2 == expected_result_2);

    PqlExtractor extractor_3 = PqlExtractor(&pkb6);
    VertexSet test_result_3 = extractor_3.GetAllAffectedByBipT();
    VertexSet expected_result_3 = VertexSet{};
    Assert::IsTrue(test_result_3 == expected_result_3);
  }

  TEST_METHOD(TestGetAffectsTMap) {
    PqlExtractor extractor_1 = PqlExtractor(&pkb1);
    AffectsMap test_result_1 = extractor_1.GetAffectsTMap();
    AffectsMap expected_result_1;

    expected_result_1.emplace(4, VertexSet{4, 12, 6, 9});
    expected_result_1.emplace(6, VertexSet{4, 12, 6, 9});
    expected_result_1.emplace(8, VertexSet{12});

    // add empty set for nodes in graph but doesnt affect anything
    expected_result_1.emplace(12, VertexSet());
    expected_result_1.emplace(9, VertexSet());

    Assert::IsTrue(test_result_1 == expected_result_1);

    PqlExtractor extractor_2 = PqlExtractor(&pkb4);
    AffectsMap test_result_2 = extractor_2.GetAffectsTMap();
    AffectsMap expected_result_2;

    expected_result_2.emplace(1, VertexSet{3, 12});
    expected_result_2.emplace(5, VertexSet{3, 12});
    expected_result_2.emplace(7, VertexSet{7, 8, 9, 5, 3, 12});
    expected_result_2.emplace(8, VertexSet{7, 8, 9, 5, 3, 12});
    expected_result_2.emplace(9, VertexSet{7, 8, 9, 5, 3, 12});

    // add empty set for nodes in graph but doesnt affect anything
    expected_result_2.emplace(3, VertexSet());
    expected_result_2.emplace(12, VertexSet());

    Assert::IsTrue(test_result_2 == expected_result_2);
  }

  TEST_METHOD(TestGetAffectedByTMap) {
    PqlExtractor extractor_1 = PqlExtractor(&pkb1);
    AffectsMap test_result_1 = extractor_1.GetAffectedByTMap();
    AffectsMap expected_result_1;

    expected_result_1.emplace(4, VertexSet{4, 6});
    expected_result_1.emplace(9, VertexSet{4, 6});
    expected_result_1.emplace(6, VertexSet{4, 6});
    expected_result_1.emplace(12, VertexSet{4, 6, 8});

    // add empty set for nodes in graph but doesnt affect anything
    expected_result_1.emplace(8, VertexSet());

    Assert::IsTrue(test_result_1 == expected_result_1);

    PqlExtractor extractor_2 = PqlExtractor(&pkb4);
    AffectsMap test_result_2 = extractor_2.GetAffectedByTMap();
    AffectsMap expected_result_2;

    expected_result_2.emplace(3, VertexSet{1, 5, 7, 8, 9});
    expected_result_2.emplace(12, VertexSet{1, 5, 7, 8, 9});
    expected_result_2.emplace(7, VertexSet{7, 8, 9});
    expected_result_2.emplace(8, VertexSet{7, 8, 9});
    expected_result_2.emplace(9, VertexSet{7, 8, 9});
    expected_result_2.emplace(5, VertexSet{7, 8, 9});

    // add empty set for nodes in graph but doesnt affect anything
    expected_result_2.emplace(1, VertexSet());

    Assert::IsTrue(test_result_2 == expected_result_2);
  }

  TEST_METHOD(TestGetAffectsBipTMap) {
    PqlExtractor extractor = PqlExtractor(&pkb5);
    AffectsMap test_result = extractor.GetAffectsBipTMap();
    AffectsMap expected_result;

    expected_result.emplace(1, VertexSet{10, 11, 3, 6, 8, 5});
    expected_result.emplace(3, VertexSet{5});
    expected_result.emplace(4, VertexSet{5});
    expected_result.emplace(10, VertexSet{3, 8, 5});
    expected_result.emplace(6, VertexSet{10, 8, 11, 5, 3});

    expected_result.emplace(11, VertexSet());
    expected_result.emplace(8, VertexSet());
    expected_result.emplace(5, VertexSet());

    Assert::IsTrue(test_result == expected_result);
  }

  TEST_METHOD(TestGetAffectedByBipTMap) {
    PqlExtractor extractor = PqlExtractor(&pkb5);
    AffectsMap test_result = extractor.GetAffectedByBipTMap();
    AffectsMap expected_result;

    expected_result.emplace(6, VertexSet{1});
    expected_result.emplace(10, VertexSet{1, 6});
    expected_result.emplace(11, VertexSet{1, 6});
    expected_result.emplace(3, VertexSet{1, 10, 6});
    expected_result.emplace(5, VertexSet{3, 4, 6, 1, 10});
    expected_result.emplace(8, VertexSet{1, 10, 6});

    // create empty maps for statements that affect nothing but is in graph
    expected_result.emplace(1, VertexSet());
    expected_result.emplace(4, VertexSet());

    Assert::IsTrue(test_result == expected_result);
  }

  PKB GetTestPKBOne() {
    string program =
        " procedure one {"
        "while(a == 1) {"      // 1
        " a = 1;"              // 2
        " if (x == b) then {"  // 3
        "t = t + 1;"           // 4
        "print x;"             // 5
        "} else {"
        "t = t * 2;"  // 6
        "}"
        "}"
        "call two;"                 // 7
        "g = a;"                    // 8
        "f = t + a + c;"            // 9
        "read f;"                   // 10
        "w = f;"                    // 11
        "affects = g + f + a + t;"  // 12
        "}"
        "procedure two {"
        "read a;"  // 13
        "}";

    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    return test_pkb;
  }
  PKB GetTestPKBTwo() {
    string program =
        " procedure one {"
        "a = b;"               // 1
        "read x;"              // 2
        "print y;"             // 3
        " if (x == b) then {"  // 4
        "x = a + 2;"           // 5
        "y = 5*a;"             // 6
        "} else {"
        "t = z * 2;"     // 7
        "while(k==1) {"  // 8
        "g=z;"           // 9
        "z=a+t+z;"       // 10
        "read a;"        // 11
        "y =a + y;"      // 12
        "}"
        "}"
        "k = y;"  // 13
        "}";

    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    return test_pkb;
  }
  PKB GetTestPKBThree() {
    string program =
        " procedure one {"
        "c = 1;"               // 1
        "read x;"              // 2
        "b = 24;"              // 3
        " if (x == b) then {"  // 4
        "a = a + 2;"           // 5
        "y = 5*a;"             // 6
        "} else {"
        "c = 22 + a;"       // 7
        "a = z * 2;"        // 8
        "a = b + 100 + c;"  // 9
        "while(k==1) {"     // 10
        "b=z;"              // 11
        "z=a+t+z;"          // 12
        "read a;"           // 13
        "y =a + y;"         // 14
        "}"
        "}"
        "g = a+ b+ c + y;"  // 15
        "}";

    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    return test_pkb;
  }
  PKB GetTestPKBFour() {
    string program =
        "procedure one {"
        "  a = b;"             // 1
        "  while(b > a) {"     // 2
        "    c = a;"           // 3
        "    read a;"          // 4
        "    a = b;"           // 5
        "    while(c > a) { "  // 6
        "      b = g;"         // 7
        "      g = e;"         // 8
        "      e = b; }"       // 9
        "    print b; }"       // 10
        "  print x;"           // 11
        "  end = a; }";        // 12

    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    return test_pkb;
  }
  PKB GetTestPKBFive() {
    string program =
        "procedure Bill {"
        "  x=5;"        // 1
        "  call Mary;"  // 2
        "  y=x+6;"      // 3
        "  x=5;"        // 4
        "  z=x*y+2;"    // 5
        "}"
        "procedure Mary {"
        "  y=x*3;"      // 6
        "  call John;"  // 7
        "  z=x+y;"      // 8
        "}"
        "procedure John {"
        "  if (i > 0) then {"  // 9
        "    x = x + z + y;"   // 10
        "  } else {"
        "    z = x + y;"  // 11
        "  }"
        "}";
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    return test_pkb;
  }

  PKB GetTestPKBSix() {
    string program =
        "procedure one {"
        "  x = c + d;"          // 1
        "  call two;"           // 2
        "  read a;"             // 3
        "  g = e;"              // 4
        "  read x;"             // 5
        "  end = a + x + d; }"  // 6
        "procedure two {"
        "read d;}";  // 7

    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);
    return test_pkb;
  }

  PKB GetTestPKBSeven() {
    string program =
        "procedure one {"
        "  while (a == b) {"  // 1
        "  if (a==1) then {"  // 2
        "  read a;"           // 3
        "  } else {"
        "  read x;"            // 4
        "  end = a + x + d;}"  // 5
        "}"
        "a = a+1;"  // 6
        "}";

    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);
    return test_pkb;
  }
};
}  // namespace PkbPqlExtractorTests
