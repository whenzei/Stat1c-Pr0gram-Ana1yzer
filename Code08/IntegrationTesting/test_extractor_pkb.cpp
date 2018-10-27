#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_extractor.h"
#include "parser.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PkbPqlExtractorTests {
TEST_CLASS(TestPkbPqlExtractor) {
  const string kProcName1 = "one";
  const string kProcName2 = "two";

 public:
  TEST_METHOD(IsNextTTwoParams) {
    PKB dummy_pkb = GetDummyPKBOne();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

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
    PKB dummy_pkb = GetDummyPKBOne();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

    // Positives
    Assert::IsTrue(extractor.IsNextT(3));
    Assert::IsTrue(extractor.IsNextT(7));

    // Negatives
    Assert::IsFalse(extractor.IsNextT(1));
    Assert::IsFalse(extractor.IsNextT(10));
  }

  TEST_METHOD(IsPreviousT) {
    PKB dummy_pkb = GetDummyPKBOne();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

    // Positive
    Assert::IsTrue(extractor.IsPreviousT(1));
    Assert::IsTrue(extractor.IsPreviousT(3));

    // Negatives
    Assert::IsFalse(extractor.IsPreviousT(7));
    Assert::IsFalse(extractor.IsPreviousT(8));
  }

  TEST_METHOD(GetNextT) {
    PKB dummy_pkb = GetDummyPKBTwo();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

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
    PKB dummy_pkb = GetDummyPKBTwo();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

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
    PKB dummy_pkb = GetDummyPKBThree();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

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
    PKB test_pkb = GetTestPKBOne();
    PqlExtractor extractor = PqlExtractor(test_pkb);

    bool test_result_1 = extractor.isAffects(4, 4);
    Assert::IsTrue(test_result_1);

    bool test_result_2 = extractor.isAffects(4, 6);
    Assert::IsTrue(test_result_2);

    bool test_result_3 = extractor.isAffects(4, 9);
    Assert::IsTrue(test_result_3);

    bool test_result_4 = extractor.isAffects(2, 8);
    Assert::IsFalse(test_result_4);

    bool test_result_5 = extractor.isAffects(9, 11);
    Assert::IsFalse(test_result_4);
  }


 private:
  PKB GetDummyPKBOne() {
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
  PKB GetDummyPKBTwo() {
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
  PKB GetDummyPKBThree() {
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
        "t = t * 2;"           // 6
        "}"
        "}"
        "call two;"           // 7
        "g = a;"                // 8
        "f = t + a + c;"        //9
        "read f;"              //10
        "w = f;"                //1
        "}"
        "procedure two {"
        "read a;"             //12
        "}";

    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    return test_pkb;
  }
};
}  // namespace PkbPqlExtractorTests
