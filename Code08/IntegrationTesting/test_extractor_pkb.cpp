#include "stdafx.h"
#include "CppUnitTest.h"
#include "parser.h"
#include "pkb.h"
#include "pql_extractor.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PkbPqlExtractorTests {
TEST_CLASS(TestPkbPqlExtractor) {
  const string kProcName1 = "one";
  const string kProcName2 = "two";
  const PKB pkb1 = GetTestPKBOne();
  const PKB pkb2 = GetTestPKBTwo();
  const PKB pkb3 = GetTestPKBThree();
  const PKB pkb4 = GetTestPKBFour();
  const PKB next_pkb1 = GetNextPKBOne();
  const PKB next_pkb2 = GetNextPKBTwo();
  const PKB next_pkb3 = GetNextPKBThree();

 public:
  TEST_METHOD(IsNextTTwoParams) {
    PqlExtractor extractor = PqlExtractor(next_pkb1);

    // Positives
    Assert::IsTrue(extractor.IsNextT(1, 2));
    Assert::IsTrue(extractor.IsNextT(1, 7));
    Assert::IsTrue(extractor.IsNextT(6, 3));
    Assert::IsTrue(extractor.IsNextT(6, 7));

    // Negatives
    Assert::IsFalse(extractor.IsNextT(1, 1));
    Assert::IsFalse(extractor.IsNextT(1, 8));
    Assert::IsFalse(extractor.IsNextT(3, 5));
  }

  TEST_METHOD(IsNextTOneParam) {
    PqlExtractor extractor = PqlExtractor(next_pkb1);

    // Positives
    Assert::IsTrue(extractor.IsNextT(3));
    Assert::IsTrue(extractor.IsNextT(7));

    // Negatives
    Assert::IsFalse(extractor.IsNextT(1));
    Assert::IsFalse(extractor.IsNextT(10));
  }

  TEST_METHOD(IsPreviousT) {
    PqlExtractor extractor = PqlExtractor(next_pkb1);

    // Positive
    Assert::IsTrue(extractor.IsPreviousT(1));
    Assert::IsTrue(extractor.IsPreviousT(3));

    // Negatives
    Assert::IsFalse(extractor.IsPreviousT(7));
    Assert::IsFalse(extractor.IsPreviousT(8));
  }

  TEST_METHOD(GetNextT) {
    PqlExtractor extractor = PqlExtractor(next_pkb2);

    // GetNextT of stmt 1
    StmtNumList expected_result_1 = StmtNumList{2, 3, 5, 4, 7, 6, 8};
    StmtNumList test_result_1 = extractor.GetNextT(1);
    Assert::IsTrue(test_result_1 == expected_result_1);

    // GetNextT of stmt 9
    StmtNumList expected_result_2 = StmtNumList{3, 10, 4, 11, 6, 8};
    StmtNumList test_result_2 = extractor.GetNextT(9);
    Assert::IsTrue(test_result_2 == expected_result_2);

    // GetNextT of stmt 8
    StmtNumList expected_result_3 = StmtNumList{};
    StmtNumList test_result_3 = extractor.GetNextT(8);
    Assert::IsTrue(test_result_3 == expected_result_3);
  }
  TEST_METHOD(GetPreviousT) {
    PqlExtractor extractor = PqlExtractor(next_pkb2);

    // GetPreviousT of stmt 8
    StmtNumList expected_result_1 = StmtNumList{6, 7, 4, 11, 5, 3, 10, 2, 1, 9};
    StmtNumList test_result_1 = extractor.GetPreviousT(8);
    Assert::IsTrue(test_result_1 == expected_result_1);

    // GetPreviousT of stmt 8
    StmtNumList expected_result_2 = StmtNumList{5, 2, 1};
    StmtNumList test_result_2 = extractor.GetPreviousT(7);
    Assert::IsTrue(test_result_2 == expected_result_2);

    // GetPreviousT of stmt 9
    StmtNumList expected_result_3 = StmtNumList{};
    StmtNumList test_result_3 = extractor.GetPreviousT(9);
    Assert::IsTrue(test_result_3 == expected_result_3);
  }
  TEST_METHOD(GetAllNextTPairs) {
    PqlExtractor extractor = PqlExtractor(next_pkb3);

    StmtNumPairList test_result_1 = extractor.GetAllNextTPairs();
    StmtNumPairList expected_result_1 =
        StmtNumPairList{make_pair(1, 2),   make_pair(1, 3),  make_pair(1, 5),
                        make_pair(1, 4),   make_pair(2, 5),  make_pair(3, 4),
                        make_pair(3, 5),   make_pair(4, 5),  make_pair(9, 10),
                        make_pair(9, 6),   make_pair(9, 11), make_pair(9, 7),
                        make_pair(9, 12),  make_pair(9, 8),  make_pair(10, 6),
                        make_pair(10, 11), make_pair(10, 7), make_pair(10, 12),
                        make_pair(10, 8),  make_pair(11, 7), make_pair(11, 12),
                        make_pair(11, 8),  make_pair(7, 8)};

    Assert::IsTrue(test_result_1 == expected_result_1);
  }

  TEST_METHOD(IsAffects) {
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

  PKB GetNextPKBOne() {
    /*
       1 -> 2 -> 5 -> 7
       \ \_          /
        \   3  -->  4
         \_ ^/
          6
    */

    PKB dummy_pkb;
    dummy_pkb.InsertNext(kProcName1, 1, 2);
    dummy_pkb.InsertNext(kProcName1, 1, 3);
    dummy_pkb.InsertNext(kProcName1, 3, 4);
    dummy_pkb.InsertNext(kProcName1, 2, 5);
    dummy_pkb.InsertNext(kProcName1, 5, 7);
    dummy_pkb.InsertNext(kProcName1, 4, 7);
    dummy_pkb.InsertNext(kProcName1, 1, 6);
    dummy_pkb.InsertNext(kProcName1, 6, 3);

    return dummy_pkb;
  }
  PKB GetNextPKBTwo() {
    /*
       1 -> 2 -> 5 -> 7
        \_              \_
          3 -> 4 -> 6 -> 8
        ^/          /^
        9 -> 10 -> 11
     */

    PKB dummy_pkb;
    dummy_pkb.InsertNext(kProcName1, 1, 2);
    dummy_pkb.InsertNext(kProcName1, 1, 3);
    dummy_pkb.InsertNext(kProcName1, 2, 5);
    dummy_pkb.InsertNext(kProcName1, 5, 7);
    dummy_pkb.InsertNext(kProcName1, 3, 4);
    dummy_pkb.InsertNext(kProcName1, 4, 6);
    dummy_pkb.InsertNext(kProcName1, 6, 8);
    dummy_pkb.InsertNext(kProcName1, 9, 3);
    dummy_pkb.InsertNext(kProcName1, 9, 10);
    dummy_pkb.InsertNext(kProcName1, 7, 8);
    dummy_pkb.InsertNext(kProcName1, 10, 11);
    dummy_pkb.InsertNext(kProcName1, 11, 6);

    return dummy_pkb;
  }
  PKB GetNextPKBThree() {
    /*
      proc one

       1 ->   2 ->  5
        \_         /^
          3  ->   4


      proc two

        9 -> 10 -> 11 -> 12
              \_    \_
                6     7 -> 8
     */

    PKB dummy_pkb;
    dummy_pkb.InsertNext(kProcName1, 1, 2);
    dummy_pkb.InsertNext(kProcName1, 1, 3);
    dummy_pkb.InsertNext(kProcName1, 2, 5);
    dummy_pkb.InsertNext(kProcName1, 3, 4);
    dummy_pkb.InsertNext(kProcName1, 4, 5);
    dummy_pkb.InsertNext(kProcName2, 9, 10);
    dummy_pkb.InsertNext(kProcName2, 10, 6);
    dummy_pkb.InsertNext(kProcName2, 10, 11);
    dummy_pkb.InsertNext(kProcName2, 11, 7);
    dummy_pkb.InsertNext(kProcName2, 11, 12);
    dummy_pkb.InsertNext(kProcName2, 7, 8);

    return dummy_pkb;
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
        "c = 22 + a;"    // 7
        "a = z * 2;"     // 8
        "a = b + 100;"   // 9
        "while(k==1) {"  // 10
        "b=z;"           // 11
        "z=a+t+z;"       // 12
        "read a;"        // 13
        "y =a + y;"      // 14
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
};
}  // namespace PkbPqlExtractorTests
