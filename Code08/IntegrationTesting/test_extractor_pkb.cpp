#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_extractor.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PkbPqlExtractorTests {
TEST_CLASS(TestPkbPqlExtractor){
public :
  TEST_METHOD(IsNextTTwoParams){
    PKB dummy_pkb = GetDummyPKBOne();
    PqlExtractor extractor =  PqlExtractor(dummy_pkb);
    
    // Positives
    Assert::IsTrue(extractor.IsNextT("1", "2"));
    Assert::IsTrue(extractor.IsNextT("1", "7"));
    Assert::IsTrue(extractor.IsNextT("6", "3"));
    Assert::IsTrue(extractor.IsNextT("6", "7"));

    // Negatives
    Assert::IsFalse(extractor.IsNextT("1", "1"));
    Assert::IsFalse(extractor.IsNextT("1", "8"));
    Assert::IsFalse(extractor.IsNextT("3", "5"));
  }

  TEST_METHOD(IsNextTOneParam) {
    PKB dummy_pkb = GetDummyPKBOne();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

    // Positives
    Assert::IsTrue(extractor.IsNextT("3"));
    Assert::IsTrue(extractor.IsNextT("7"));

    // Negatives
    Assert::IsFalse(extractor.IsNextT("1"));
    Assert::IsFalse(extractor.IsNextT("10"));    
  }

  TEST_METHOD(IsPreviousT) {
    PKB dummy_pkb = GetDummyPKBOne();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

    // Positive
    Assert::IsTrue(extractor.IsPreviousT("1"));
    Assert::IsTrue(extractor.IsPreviousT("3"));

    // Negatives
    Assert::IsFalse(extractor.IsPreviousT("7"));
    Assert::IsFalse(extractor.IsPreviousT("8"));
  }

  TEST_METHOD(GetNextT) {
    PKB dummy_pkb = GetDummyPKBTwo();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);


    // GetNextT of stmt 1
    StmtNumList expected_result_1 = StmtNumList{"2", "3", "5", "4", "7", "6", "8"}; 
    StmtNumList test_result_1 = extractor.GetNextT("1");
    Assert::IsTrue( test_result_1 == expected_result_1);

    // GetNextT of stmt 9
    StmtNumList expected_result_2 =
        StmtNumList{"3", "10", "4", "11", "6", "8"}; 
    StmtNumList test_result_2 = extractor.GetNextT("9");
    Assert::IsTrue(test_result_2 == expected_result_2);

    // GetNextT of stmt 8
    StmtNumList expected_result_3 = StmtNumList{};
    StmtNumList test_result_3 = extractor.GetNextT("8");
    Assert::IsTrue(test_result_3 == expected_result_3);

  }
  TEST_METHOD(GetPreviousT) {
    PKB dummy_pkb = GetDummyPKBTwo();
    PqlExtractor extractor = PqlExtractor(dummy_pkb);

    // GetPreviousT of stmt 8
    StmtNumList expected_result_1 = StmtNumList{"6", "7", "4", "11", "5", "3", "10","2", "1", "9"}; 
    StmtNumList test_result_1 = extractor.GetPreviousT("8");
    Assert::IsTrue( test_result_1 == expected_result_1);

    // GetPreviousT of stmt 8
    StmtNumList expected_result_2 =
        StmtNumList{"5", "2", "1"};
    StmtNumList test_result_2 = extractor.GetPreviousT("7");
    Assert::IsTrue(test_result_2 == expected_result_2);


    // GetPreviousT of stmt 9
    StmtNumList expected_result_3 = StmtNumList{};
    StmtNumList test_result_3 = extractor.GetPreviousT("9");
    Assert::IsTrue(test_result_3 == expected_result_3);

  }

  private:
    PKB GetDummyPKBOne() {
    /*
       1 -> 2 -> 5 -> 7
       \ \           /
        \  3  -->   4
         \ /
          6
     */

     PKB dummy_pkb;
     dummy_pkb.InsertNext("one", 1, 2);
     dummy_pkb.InsertNext("one", 1, 3);
     dummy_pkb.InsertNext("one", 3, 4);
     dummy_pkb.InsertNext("one", 2, 5);
     dummy_pkb.InsertNext("one", 5, 7);
     dummy_pkb.InsertNext("one", 4, 7);
     dummy_pkb.InsertNext("one", 1, 6);
     dummy_pkb.InsertNext("one", 6, 3);

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
     dummy_pkb.InsertNext("one", 1, 2);
     dummy_pkb.InsertNext("one", 1, 3);
     dummy_pkb.InsertNext("one", 2, 5);
     dummy_pkb.InsertNext("one", 5, 7);
     dummy_pkb.InsertNext("one", 3, 4);
     dummy_pkb.InsertNext("one", 4, 6);
     dummy_pkb.InsertNext("one", 6, 8);
     dummy_pkb.InsertNext("one", 9, 3);
     dummy_pkb.InsertNext("one", 9, 10);
     dummy_pkb.InsertNext("one", 7, 8);
     dummy_pkb.InsertNext("one", 10, 11);
     dummy_pkb.InsertNext("one", 11, 6);

     return dummy_pkb;
    }
}
;
}