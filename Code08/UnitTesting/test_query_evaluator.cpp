#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"
#include "pql_query.h"

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestQueryEvaluator) {
 public:
  TEST_METHOD(TestTupleCrossProduct) {
    PqlEvaluator qe;
    vector<FinalResult> test_items = {{"1", "2"}, {"4", "5"}};
    FinalResult fr;
    string temp;
    qe.TupleCrossProduct(fr, temp, test_items.begin(), test_items.end());

    string expectedstart = "1 4";
    string expectedend = "2 5";

    Assert::IsTrue(fr.size() == 4);

    Assert::AreEqual(expectedstart, fr.front());
    Assert::AreEqual(expectedend, fr.back());
  }

  TEST_METHOD(TestTupleCrossProduct2) {
    PqlEvaluator qe;
    vector<FinalResult> test_items = {
        {"1", "2", "3"}, {"4", "5"}, {"7", "8", "9", "10"}};
    FinalResult fr;
    string temp;
    qe.TupleCrossProduct(fr, temp, test_items.begin(), test_items.end());

    string expectedstart = "1 4 7";
    string expectedend = "3 5 10";

    Assert::IsTrue(fr.size() == 24);

    Assert::AreEqual(expectedstart, fr.front());
    Assert::AreEqual(expectedend, fr.back());
  }

  TEST_METHOD(TestTrim) {
    string smt = "abc ";
    string empty = "";
    PqlEvaluator qe;
    string expected = "abc";
    string result = qe.Trim(smt);
    Assert::AreEqual(expected, result);

    string expected2 = "";
    string result2 = qe.Trim(empty);
    Assert::AreEqual(expected2, result2);
  }

};
}  // namespace PQLTests