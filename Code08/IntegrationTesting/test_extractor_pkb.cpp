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
  const PKB pkb1 = GetTestPKBOne();
  const PKB pkb2 = GetTestPKBTwo();
  const PKB pkb3 = GetTestPKBThree();
  const PKB pkb4 = GetTestPKBFour();
  const PKB pkb5 = GetTestPKBFive();
  const PKB pkb6 = GetTestPKBSix();
  const PKB pkb7 = GetTestPKBSeven();

 public:
  TEST_METHOD(IsNextTTwoParams) {
    PqlExtractor extractor = PqlExtractor(pkb1);

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

  TEST_METHOD(IsNextTOneParam) {
    PqlExtractor extractor = PqlExtractor(pkb1);

    // Positives
    Assert::IsTrue(extractor.IsNextT(12));
    Assert::IsTrue(extractor.IsNextT(6));
    Assert::IsTrue(extractor.IsNextT(4));

    // Negatives
    Assert::IsFalse(extractor.IsNextT(13));
    Assert::IsFalse(extractor.IsNextT(2222));
  }

  TEST_METHOD(IsPreviousT) {
    PqlExtractor extractor = PqlExtractor(pkb1);

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

  TEST_METHOD(GetPreviousT) {
    PqlExtractor extractor = PqlExtractor(pkb2);

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

  TEST_METHOD(GetNextT) {
    PqlExtractor extractor = PqlExtractor(pkb2);

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

  TEST_METHOD(GetNextTPair) {
    PqlExtractor extractor = PqlExtractor(pkb7);

    // Positive
    StmtNumPairList test_result_1 = extractor.GetAllNextTPairs();
    std::set<pair<int, int>> test_result_1_set;
    for (pair<int, int> res : test_result_1) {
      test_result_1_set.emplace(res);
    }

    StmtNumPairList expected_result_1 = StmtNumPairList{
        make_pair(1, 2), make_pair(1, 3), make_pair(1, 4), make_pair(1, 5),
        make_pair(1, 1), make_pair(1, 6), make_pair(2, 3), make_pair(2, 4),
        make_pair(2, 5), make_pair(2, 1), make_pair(2, 2), make_pair(2, 6),
        make_pair(3, 3), make_pair(3, 2), make_pair(3, 4), make_pair(3, 5),
        make_pair(3, 1), make_pair(3, 6), make_pair(4, 4), make_pair(4, 5),
        make_pair(4, 6), make_pair(4, 1), make_pair(4, 2), make_pair(4, 3),
        make_pair(5, 5), make_pair(5, 1), make_pair(5, 2), make_pair(5, 3),
        make_pair(5, 4), make_pair(5, 6)};

    std::set<pair<int, int>> expected_result_1_set;

    for (pair<int, int> res : expected_result_1) {
      expected_result_1_set.emplace(res);
    }

    Assert::IsTrue(expected_result_1_set == test_result_1_set);
  }

  TEST_METHOD(IsAffectsTwoParams) {
    PqlExtractor extractor = PqlExtractor(pkb1);

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
  TEST_METHOD(IsAffectsOneParam) {
    PqlExtractor extractor = PqlExtractor(pkb1);

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
  TEST_METHOD(IsAffected) {
    PqlExtractor extractor = PqlExtractor(pkb3);

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
    extractor = PqlExtractor(pkb6);

    bool test_result_8 = extractor.IsAffected(6);
    Assert::IsFalse(test_result_8);
  }

  TEST_METHOD(GetAffects) {
    PqlExtractor extractor = PqlExtractor(pkb2);

    // Positives**************************************
    StmtNumList test_result_1 = extractor.GetAffects(1);
    StmtNumList expected_result_1 = StmtNumList{5, 6, 10};
    Assert::IsTrue(test_result_1 == expected_result_1);

    StmtNumList test_result_2 = extractor.GetAffects(12);
    StmtNumList expected_result_2 = StmtNumList{12, 13};
    Assert::IsTrue(test_result_2 == expected_result_2);

    StmtNumList test_result_3 = extractor.GetAffects(10);
    StmtNumList expected_result_3 = StmtNumList{9, 10};
    Assert::IsTrue(test_result_3 == expected_result_3);

    // Negatives***************************************

    // Stmt 2 is a read stmt
    StmtNumList test_result_4 = extractor.GetAffects(2);
    StmtNumList expected_result_4 = StmtNumList{};
    Assert::IsTrue(test_result_4 == expected_result_4);

    // Stmt 9 rhs var is modified
    StmtNumList test_result_5 = extractor.GetAffects(9);
    StmtNumList expected_result_5 = StmtNumList{};
    Assert::IsTrue(test_result_5 == expected_result_5);

    // Stmt 8 is a while stmt
    StmtNumList test_result_6 = extractor.GetAffects(8);
    StmtNumList expected_result_6 = StmtNumList{};
    Assert::IsTrue(test_result_6 == expected_result_6);

    // Stmt is not found in program
    StmtNumList test_result_7 = extractor.GetAffects(55);
    StmtNumList expected_result_7 = StmtNumList{};
    Assert::IsTrue(test_result_7 == expected_result_7);
  }
  TEST_METHOD(GetAffectedByWhileInIf) {
    PqlExtractor extractor = PqlExtractor(pkb3);

    // Positives**************************************
    StmtNumList test_result_1 = extractor.GetAffectedBy(15);
    StmtNumList expected_result_1 = StmtNumList{6, 5, 3, 1, 14, 11, 9, 7};
    Assert::IsTrue(expected_result_1 == test_result_1);

    StmtNumList test_result_2 = extractor.GetAffectedBy(12);
    StmtNumList expected_result_2 = StmtNumList{12, 9};
    Assert::IsTrue(expected_result_2 == test_result_2);

    StmtNumList test_result_3 = extractor.GetAffectedBy(6);
    StmtNumList expected_result_3 = StmtNumList{5};
    Assert::IsTrue(expected_result_3 == test_result_3);

    StmtNumList test_result_4 = extractor.GetAffectedBy(11);
    StmtNumList expected_result_4 = StmtNumList{12};
    Assert::IsTrue(expected_result_4 == test_result_4);

    // Negatives**************************************

    // First statement is not affected by any other statement
    StmtNumList test_result_5 = extractor.GetAffectedBy(1);
    StmtNumList expected_result_5 = StmtNumList{};
    Assert::IsTrue(expected_result_5 == test_result_5);

    // If statement
    StmtNumList test_result_6 = extractor.GetAffectedBy(4);
    StmtNumList expected_result_6 = StmtNumList{};
    Assert::IsTrue(expected_result_6 == test_result_6);

    // Stmt 7 not affected by any other statement
    StmtNumList test_result_7 = extractor.GetAffectedBy(7);
    StmtNumList expected_result_7 = StmtNumList{};
    Assert::IsTrue(expected_result_7 == test_result_7);

    // Additional test: all vars modified by non assignment statement before
    // reaching an affecting assignment statement
    extractor = PqlExtractor(pkb6);
    StmtNumList test_result_8 = extractor.GetAffectedBy(6);
    StmtNumList expected_result_8 = StmtNumList{};
    Assert::IsTrue(expected_result_8 == test_result_8);
  }

  TEST_METHOD(GetAllAffects) {
    // test while-if loop
    PqlExtractor extractor = PqlExtractor(pkb1);
    AffectsTable actual_results_1 = extractor.GetAffectsTable();
    AffectsTable expected_results_1;
    // variable t is modified at 4 / 6, used at 4, 6, 9, 12 without being
    // modified again
    expected_results_1[4] = VertexSet{4, 6, 9, 12};
    expected_results_1[6] = VertexSet{4, 6, 9, 12};
    // variable g is modified at 8 and used at 12 without being modified again
    expected_results_1[8] = VertexSet{12};
    // variable a and f are both modified again by non-assign statements and
    // thus have no results
    Assert::IsTrue(actual_results_1 == expected_results_1);

    // test if-while loop
    extractor = PqlExtractor(pkb2);
    AffectsTable actual_results_2 = extractor.GetAffectsTable();
    AffectsTable expected_results_2;
    expected_results_2[1] = VertexSet{5, 6, 10};
    expected_results_2[6] = VertexSet{13};
    expected_results_2[7] = VertexSet{10};
    expected_results_2[10] = VertexSet{9, 10};
    expected_results_2[12] = VertexSet{12, 13};
    Assert::IsTrue(actual_results_2 == expected_results_2);

    // test while-while loop
    extractor = PqlExtractor(pkb4);
    AffectsTable actual_results_3 = extractor.GetAffectsTable();
    AffectsTable expected_results_3;
    expected_results_3[1] = VertexSet{3, 12};
    expected_results_3[5] = VertexSet{3, 12};
    expected_results_3[7] = VertexSet{5, 9};
    expected_results_3[8] = VertexSet{7};
    expected_results_3[9] = VertexSet{8};
    Assert::IsTrue(actual_results_3 == expected_results_3);
  }

  TEST_METHOD(TestGetAffectsBipTable) {
    PqlExtractor extractor = PqlExtractor(pkb5);
    AffectsTable actual_results = extractor.GetAffectsBipTable();
    AffectsTable expected_results;
    expected_results[1] = VertexSet{6, 10, 11, 8, 3};
    expected_results[3] = VertexSet{5};
    expected_results[4] = VertexSet{5};
    expected_results[6] = VertexSet{11, 8, 10};
    expected_results[10] = VertexSet{8, 3};
    Assert::IsTrue(actual_results == expected_results);
  }

  TEST_METHOD(IsAffectsBipTwoParam) {
    PqlExtractor extractor = PqlExtractor(pkb5);

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

  TEST_METHOD(IsAffectsBipOneParam) {
    PqlExtractor extractor = PqlExtractor(pkb5);

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

  TEST_METHOD(IsAffectedBip) {
    /* PqlExtractor extractor = PqlExtractor(pkb5);

      // Positives***********************************
     bool test_result_1 = extractor.IsAffectedBip(11);
     Assert::IsTrue(test_result_1);

     bool test_result_2 = extractor.IsAffectedBip(10);
     Assert::IsTrue(test_result_2);

     bool test_result_3 = extractor.IsAffectedBip(6);
     Assert::IsTrue(test_result_3);

     bool test_result_4 = extractor.IsAffectedBip(5);
     Assert::IsTrue(test_result_4);

     //Negatives***********************************
     bool test_result_5 = extractor.IsAffectedBip(2);
     Assert::IsFalse(test_result_5);

     bool test_result_6 = extractor.IsAffectedBip(1);
     Assert::IsFalse(test_result_6);

     bool test_result_7 = extractor.IsAffectedBip(6);
     Assert::IsFalse(test_result_7);

     bool test_result_8 = extractor.IsAffectedBip(4);
     Assert::IsFalse(test_result_8);
   */
  }

  TEST_METHOD(GetAffectedByIfInWhile) {
    PqlExtractor extractor = PqlExtractor(pkb1);

    // Positives**************************************
    StmtNumList test_result_1 = extractor.GetAffectedBy(9);
    StmtNumList expected_result_1 = StmtNumList{4, 6};
    Assert::IsTrue(expected_result_1 == test_result_1);

    StmtNumList test_result_2 = extractor.GetAffectedBy(12);
    StmtNumList expected_result_2 = StmtNumList{8, 4, 6};
    Assert::IsTrue(expected_result_2 == test_result_2);

    StmtNumList test_result_3 = extractor.GetAffectedBy(4);
    StmtNumList expected_result_3 = StmtNumList{4, 6};
    Assert::IsTrue(expected_result_3 == test_result_3);

    // Negatives**************************************

    // Print statement
    StmtNumList test_result_4 = extractor.GetAffectedBy(5);
    StmtNumList expected_result_4 = StmtNumList{};
    Assert::IsTrue(expected_result_4 == test_result_4);

    // RHS var of stmt 8 is modified by statement before
    StmtNumList test_result_5 = extractor.GetAffectedBy(8);
    StmtNumList expected_result_5 = StmtNumList{};
    Assert::IsTrue(expected_result_5 == test_result_5);

    // While statement
    StmtNumList test_result_6 = extractor.GetAffectedBy(1);
    StmtNumList expected_result_6 = StmtNumList{};
    Assert::IsTrue(expected_result_6 == test_result_6);
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
