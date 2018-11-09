#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_global.h"
#include "pql_result.h"

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;
using ColumnHeader = unordered_map<string, int>;
using ResultTable = unordered_set<vector<int>>;
using ResultRow = vector<int>;
using QueryResultList = unordered_set<int>;
using QueryResultPairList = unordered_set<pair<int, int>>;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestPqlResult) {
 public:
  QueryResultList test_result_list;
  QueryResultList test_merge_list;
  QueryResultPairList test_resultpair_list;
  QueryResultPairList test_mergepair_list;
  QueryResultPairList test_mergepair_list2;
  QueryResultPairList test_mergepair_list3;
  TEST_METHOD(TestInitTable) {
    PqlResult pql_result;

    test_result_list.emplace(1);
    test_result_list.emplace(2);
    test_result_list.emplace(3);

    pql_result.InitTable(test_result_list, "a");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have one column
    Assert::IsTrue(pql_result.GetColumnCount() == 1);

    // Should have three rows
    Assert::IsTrue(result_table.size() == 3);

    int expected_headera = 0;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);

	vector<int> expected_result1{1};
    vector<int> expected_result3{3};

    // Row should contain "1"
    Assert::IsTrue(result_table.count(expected_result1));
    // Row should contain "3"
    Assert::IsTrue(result_table.count(expected_result3));
  }

  TEST_METHOD(TestInitPairTable) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have three rows
    Assert::IsTrue(result_table.size() == 3);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

    vector<int> expected_result1{1, 7};
    vector<int> expected_result2{3, 9};

    // Row should contain "1 7"
    Assert::IsTrue(result_table.count(expected_result1));
    // Row should contain "3 9"
    Assert::IsTrue(result_table.count(expected_result2));
  }

  TEST_METHOD(TestMergeTableNoConflict) {
    PqlResult pql_result;

    test_result_list.emplace(1);
    test_result_list.emplace(2);
    test_result_list.emplace(3);

    pql_result.InitTable(test_result_list, "a");

    test_merge_list.emplace(7);
    test_merge_list.emplace(8);
    test_merge_list.emplace(9);

    pql_result.MergeResults(test_merge_list, kNoConflict, -1, "b");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have nine rows
    Assert::IsTrue(result_table.size() == 9);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

	vector<int> expected_result1{1, 7};
    vector<int> expected_result2{1, 8};
    vector<int> expected_result3{3, 9};

    // Row should contain "1 7"
    Assert::IsTrue(result_table.count(expected_result1));
    // Row should contain "1 8"
    Assert::IsTrue(result_table.count(expected_result2));
    // Row should contain "3 9"
    Assert::IsTrue(result_table.count(expected_result3));
  }

  TEST_METHOD(TestMergeTableNoConflictPair) {
    PqlResult pql_result;

    test_result_list.emplace(1);
    test_result_list.emplace(2);
    test_result_list.emplace(3);

    pql_result.InitTable(test_result_list, "a");

    test_mergepair_list.emplace(std::make_pair(7, 4));
    test_mergepair_list.emplace(std::make_pair(8, 5));
    test_mergepair_list.emplace(std::make_pair(9, 6));

    pql_result.MergeResults(test_mergepair_list, kNoConflict, -1, -1, "c", "b");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have three column
    Assert::IsTrue(pql_result.GetColumnCount() == 3);

    // Should have nine rows
    Assert::IsTrue(result_table.size() == 9);

    int expected_headera = 0;
    int expected_headerb = 2;
    int expected_headerc = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);
    Assert::AreEqual(expected_headerc, col_header.find("c")->second);

    vector<int> expected_result1{1, 7, 4};
    vector<int> expected_result2{1, 8, 5};
    vector<int> expected_result3{3, 9, 6};

    // Row should contain "1 7 4"
    Assert::IsTrue(result_table.count(expected_result1));
    // Row should contain "1 8 5"
    Assert::IsTrue(result_table.count(expected_result2));
    // Row should contain "3 9 6"
    Assert::IsTrue(result_table.count(expected_result3));
  }

  TEST_METHOD(TestMergeTableWithConflict) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_merge_list.emplace(1);
    test_merge_list.emplace(5);

    // Conflict with column 0
    pql_result.MergeResults(test_merge_list, kConflict, 0, "a");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

	vector<int> expected_result1{1, 7};
    vector<int> expected_result2{5, 11};

    // Row should contain "1 7"
    Assert::IsTrue(result_table.count(expected_result1));
    // Row should contain "5 11"
    Assert::IsTrue(result_table.count(expected_result2));
  }

  TEST_METHOD(TestMergeTableWithOneConflictLeftPair) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.emplace(std::make_pair(1, 21));
    test_mergepair_list.emplace(std::make_pair(5, 22));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictLeft, 0, -1, "a",
                            "c");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have three column
    Assert::IsTrue(pql_result.GetColumnCount() == 3);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;
    int expected_headerc = 2;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);
    Assert::AreEqual(expected_headerc, col_header.find("c")->second);

	vector<int> expected_result1{1, 7, 21};
    vector<int> expected_result2{5, 11, 22};

    // Row should contain "1 7 21"
    Assert::IsTrue(result_table.count(expected_result1));
    // Row should contain "5 11 22"
    Assert::IsTrue(result_table.count(expected_result2));
  }

  TEST_METHOD(TestMergeTableWithOneConflictLeftPairNoResult) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.emplace(std::make_pair(8, 21));
    test_mergepair_list.emplace(std::make_pair(9, 22));
    test_mergepair_list.emplace(std::make_pair(9, 22));
    test_mergepair_list.emplace(std::make_pair(10, 23));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictLeft, 0, -1, "a",
                            "c");

    // Should be an empty table
    ResultTable result_table = pql_result.GetResultTable();

    // Should have no row
    Assert::IsTrue(result_table.empty());
    Assert::IsTrue(pql_result.GetColumnHeader().empty());
    Assert::IsTrue(pql_result.GetColumnCount() == 0);
  }

  TEST_METHOD(TestMergeTableWithOneConflictRightPair) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.emplace(std::make_pair(31, 8));
    test_mergepair_list.emplace(std::make_pair(32, 9));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictRight, -1, 1, "c",
                            "b");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have three column
    Assert::IsTrue(pql_result.GetColumnCount() == 3);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;
    int expected_headerc = 2;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);
    Assert::AreEqual(expected_headerc, col_header.find("c")->second);

	vector<int> expected_result1{2, 8, 31};
    vector<int> expected_result2{3, 9, 32};

    // Row should contain "2 8 31"
    Assert::IsTrue(result_table.count(expected_result1));
    // Row should contain "3 9 32"
    Assert::IsTrue(result_table.count(expected_result2));
  }

  TEST_METHOD(TestMergeTableWithOneConflictRightPairNoResult) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.emplace(std::make_pair(31, 21));
    test_mergepair_list.emplace(std::make_pair(32, 22));
    test_mergepair_list.emplace(std::make_pair(31, 23));
    test_mergepair_list.emplace(std::make_pair(32, 24));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictRight, -1, 1, "c",
                            "b");

    // Should be an empty table
    ResultTable result_table = pql_result.GetResultTable();

    // Should have no row
    Assert::IsTrue(result_table.empty());
    Assert::IsTrue(pql_result.GetColumnHeader().empty());
    Assert::IsTrue(pql_result.GetColumnCount() == 0);
  }

  TEST_METHOD(TestMergeTableWithTwoConflict) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.emplace(std::make_pair(10, 4));
    test_mergepair_list.emplace(std::make_pair(8, 2));

    // Conflict with column 0 & 1
    pql_result.MergeResults(test_mergepair_list, kTwoConflict, 1, 0, "b", "a");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

	vector<int> expected_result1{2, 8};
    vector<int> expected_result2{4, 10};

    // First row first col should contain "2 8"
    Assert::IsTrue(result_table.count(expected_result1));
    // Sec row first col should contain "4 10"
    Assert::IsTrue(result_table.count(expected_result2));
  }

  TEST_METHOD(TestTwoMerge) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.emplace(std::make_pair(1, 21));
    test_mergepair_list.emplace(std::make_pair(5, 22));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictLeft, 0, -1, "a",
                            "c");

    test_mergepair_list2.emplace(std::make_pair(8, 21));
    test_mergepair_list2.emplace(std::make_pair(9, 23));

    // Conflict with column 2
    pql_result.MergeResults(test_mergepair_list2, kOneConflictRight, -1, 2, "d",
                            "c");

    ResultTable result_table = pql_result.GetResultTable();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have four column
    Assert::IsTrue(pql_result.GetColumnCount() == 4);

    // Should have one row
    Assert::IsTrue(result_table.size() == 1);

    int expected_headera = 0;
    int expected_headerb = 1;
    int expected_headerc = 2;
    int expected_headerd = 3;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);
    Assert::AreEqual(expected_headerc, col_header.find("c")->second);
    Assert::AreEqual(expected_headerd, col_header.find("d")->second);

    vector<int> expected_result1{1, 7, 21, 8};

    // First row first col should contain "1 7 21 8"
    Assert::IsTrue(result_table.count(expected_result1));
  }

  TEST_METHOD(TestTwoMergeNoResult) {
    PqlResult pql_result;

    test_resultpair_list.emplace(std::make_pair(1, 7));
    test_resultpair_list.emplace(std::make_pair(2, 8));
    test_resultpair_list.emplace(std::make_pair(3, 9));
    test_resultpair_list.emplace(std::make_pair(4, 10));
    test_resultpair_list.emplace(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.emplace(std::make_pair(1, 21));
    test_mergepair_list.emplace(std::make_pair(5, 22));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictLeft, 0, -1, "a",
                            "c");

    test_mergepair_list2.emplace(std::make_pair(8, 21));
    test_mergepair_list2.emplace(std::make_pair(9, 23));

    // Conflict with column 2
    pql_result.MergeResults(test_mergepair_list2, kOneConflictRight, -1, 2, "d",
                            "c");

    // This final constraint 1 b does not match the table with 1 a
    test_mergepair_list3.emplace(std::make_pair(1, 8));
    test_mergepair_list3.emplace(std::make_pair(2, 7));
    test_mergepair_list3.emplace(std::make_pair(1, 8));
    test_mergepair_list3.emplace(std::make_pair(3, 9));

    // Conflict with column 0, 1
    pql_result.MergeResults(test_mergepair_list3, kTwoConflict, 0, 1, "a", "b");

    // Should be an empty table
    ResultTable result_table = pql_result.GetResultTable();

    // Should have no row
    Assert::IsTrue(result_table.empty());
    Assert::IsTrue(pql_result.GetColumnHeader().empty());
    Assert::IsTrue(pql_result.GetColumnCount() == 0);
  }
};

}  // namespace PQLTests
