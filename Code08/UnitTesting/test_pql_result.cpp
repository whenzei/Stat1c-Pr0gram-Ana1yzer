#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_global.h"
#include "pql_result.h"
#include "stdafx.h"

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;
using ColumnHeader = unordered_map<string, int>;
using ResultTable = vector<vector<int>>;
using ResultRow = vector<int>;
using QueryResultList = vector<int>;
using QueryResultPairList = vector<pair<int, int>>;

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

    test_result_list.push_back(1);
    test_result_list.push_back(2);
    test_result_list.push_back(3);

    pql_result.InitTable(test_result_list, "a");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have one column
    Assert::IsTrue(result_row_column.size() == 1);
    Assert::IsTrue(pql_result.GetColumnCount() == 1);

    // Should have three rows
    Assert::IsTrue(result_table.size() == 3);

    int expected_headera = 0;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);

    int expected_result1 = 1;
    int expected_result3 = 3;

    // First row should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // Last row should contain "3"
    Assert::AreEqual(expected_result3, result_table[2][0]);
  }

  TEST_METHOD(TestInitPairTable) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(result_row_column.size() == 2);
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have three rows
    Assert::IsTrue(result_table.size() == 3);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

    int expected_result1 = 1;
    int expected_result3 = 3;
    int expected_resulta = 7;
    int expected_resultc = 9;

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "7"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // Last row first col should contain "3"
    Assert::AreEqual(expected_result3, result_table[2][0]);
    // Last row sec col should contain "9"
    Assert::AreEqual(expected_resultc, result_table[2][1]);
  }

  TEST_METHOD(TestMergeTableNoConflict) {
    PqlResult pql_result;

    test_result_list.push_back(1);
    test_result_list.push_back(2);
    test_result_list.push_back(3);

    pql_result.InitTable(test_result_list, "a");

    test_merge_list.push_back(7);
    test_merge_list.push_back(8);
    test_merge_list.push_back(9);

    pql_result.MergeResults(test_merge_list, kNoConflict, -1, "b");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(result_row_column.size() == 2);
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have nine rows
    Assert::IsTrue(result_table.size() == 9);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

    int expected_result1 = 1;
    int expected_result3 = 3;
    int expected_resulta = 7;
    int expected_resultb = 8;
    int expected_resultc = 9;

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // Sec row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[1][0]);
    // Sec row sec col should contain "b"
    Assert::AreEqual(expected_resultb, result_table[1][1]);
    // Last row first col should contain "3"
    Assert::AreEqual(expected_result3, result_table[8][0]);
    // Last row sec col should contain "c"
    Assert::AreEqual(expected_resultc, result_table[8][1]);
  }

  TEST_METHOD(TestMergeTableNoConflictPair) {
    PqlResult pql_result;

    test_result_list.push_back(1);
    test_result_list.push_back(2);
    test_result_list.push_back(3);

    pql_result.InitTable(test_result_list, "a");

    test_mergepair_list.push_back(std::make_pair(7, 4));
    test_mergepair_list.push_back(std::make_pair(8, 5));
    test_mergepair_list.push_back(std::make_pair(9, 6));

    pql_result.MergeResults(test_mergepair_list, kNoConflict, -1, -1, "c", "b");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have three column
    Assert::IsTrue(result_row_column.size() == 3);
    Assert::IsTrue(pql_result.GetColumnCount() == 3);

    // Should have nine rows
    Assert::IsTrue(result_table.size() == 9);

    int expected_headera = 0;
    int expected_headerb = 2;
    int expected_headerc = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);
    Assert::AreEqual(expected_headerc, col_header.find("c")->second);

    int expected_result1 = 1;
    int expected_result3 = 3;
    int expected_result4 = 4;
    int expected_result5 = 5;
    int expected_result6 = 6;
    int expected_resulta = 7;
    int expected_resultb = 8;
    int expected_resultc = 9;

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // First row last col should contain "4"
    Assert::AreEqual(expected_result4, result_table[0][2]);
    // Sec row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[1][0]);
    // Sec row sec col should contain "b"
    Assert::AreEqual(expected_resultb, result_table[1][1]);
    // Sec row last col should contain "5"
    Assert::AreEqual(expected_result5, result_table[1][2]);
    // Last row first col should contain "3"
    Assert::AreEqual(expected_result3, result_table[8][0]);
    // Last row sec col should contain "c"
    Assert::AreEqual(expected_resultc, result_table[8][1]);
    // Last row last col should contain "6"
    Assert::AreEqual(expected_result6, result_table[8][2]);
  }

  TEST_METHOD(TestMergeTableWithConflict) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_merge_list.push_back(1);
    test_merge_list.push_back(5);

    // Conflict with column 0
    pql_result.MergeResults(test_merge_list, kConflict, 0, "a");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(result_row_column.size() == 2);
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

    int expected_result1 = 1;
    int expected_result5 = 5;
    int expected_resulta = 7;
    int expected_resulte = 11;

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // Sec row first col should contain "5"
    Assert::AreEqual(expected_result5, result_table[1][0]);
    // Sec row sec col should contain "e"
    Assert::AreEqual(expected_resulte, result_table[1][1]);
  }

  TEST_METHOD(TestMergeTableWithOneConflictLeftPair) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.push_back(std::make_pair(1, 21));
    test_mergepair_list.push_back(std::make_pair(5, 22));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictLeft, 0, -1, "a",
                            "c");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have three column
    Assert::IsTrue(result_row_column.size() == 3);
    Assert::IsTrue(pql_result.GetColumnCount() == 3);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;
    int expected_headerc = 2;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);
    Assert::AreEqual(expected_headerc, col_header.find("c")->second);

    int expected_result1 = 1;
    int expected_result5 = 5;
    int expected_resulta = 7;
    int expected_resulte = 11;
    int expected_resultx = 21;
    int expected_resulty = 22;

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // First row last col should contain "x"
    Assert::AreEqual(expected_resultx, result_table[0][2]);
    // Sec row first col should contain "5"
    Assert::AreEqual(expected_result5, result_table[1][0]);
    // Sec row sec col should contain "e"
    Assert::AreEqual(expected_resulte, result_table[1][1]);
    // Sec row last col should contain "y"
    Assert::AreEqual(expected_resulty, result_table[1][2]);
  }

  TEST_METHOD(TestMergeTableWithOneConflictLeftPairNoResult) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.push_back(std::make_pair(8, 21));
    test_mergepair_list.push_back(std::make_pair(9, 22));
    test_mergepair_list.push_back(std::make_pair(9, 22));
    test_mergepair_list.push_back(std::make_pair(10, 23));

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

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.push_back(std::make_pair(31, 8));
    test_mergepair_list.push_back(std::make_pair(32, 9));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictRight, -1, 1, "c",
                            "b");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have three column
    Assert::IsTrue(result_row_column.size() == 3);
    Assert::IsTrue(pql_result.GetColumnCount() == 3);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;
    int expected_headerc = 2;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);
    Assert::AreEqual(expected_headerc, col_header.find("c")->second);

    int expected_result2 = 2;
    int expected_result3 = 3;
    int expected_resultb = 8;
    int expected_resultc = 9;
    int expected_results = 31;
    int expected_resultp = 32;

    // First row first col should contain "2"
    Assert::AreEqual(expected_result2, result_table[0][0]);
    // First row sec col should contain "b"
    Assert::AreEqual(expected_resultb, result_table[0][1]);
    // First row last col should contain "s"
    Assert::AreEqual(expected_results, result_table[0][2]);
    // Sec row first col should contain "3"
    Assert::AreEqual(expected_result3, result_table[1][0]);
    // Sec row sec col should contain "c"
    Assert::AreEqual(expected_resultc, result_table[1][1]);
    // Sec row last col should contain "p"
    Assert::AreEqual(expected_resultp, result_table[1][2]);
  }

  TEST_METHOD(TestMergeTableWithOneConflictRightPairNoResult) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.push_back(std::make_pair(31, 21));
    test_mergepair_list.push_back(std::make_pair(32, 22));
    test_mergepair_list.push_back(std::make_pair(31, 23));
    test_mergepair_list.push_back(std::make_pair(32, 24));

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

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.push_back(std::make_pair(10, 4));
    test_mergepair_list.push_back(std::make_pair(8, 2));

    // Conflict with column 0 & 1
    pql_result.MergeResults(test_mergepair_list, kTwoConflict, 1, 0, "b", "a");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have two column
    Assert::IsTrue(result_row_column.size() == 2);
    Assert::IsTrue(pql_result.GetColumnCount() == 2);

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    int expected_headera = 0;
    int expected_headerb = 1;

    Assert::AreEqual(expected_headera, col_header.find("a")->second);
    Assert::AreEqual(expected_headerb, col_header.find("b")->second);

    int expected_result2 = 2;
    int expected_result4 = 4;
    int expected_resultb = 8;
    int expected_resultd = 10;

    // First row first col should contain "2"
    Assert::AreEqual(expected_result2, result_table[0][0]);
    // First row sec col should contain "b"
    Assert::AreEqual(expected_resultb, result_table[0][1]);
    // Sec row first col should contain "4"
    Assert::AreEqual(expected_result4, result_table[1][0]);
    // Sec row sec col should contain "d"
    Assert::AreEqual(expected_resultd, result_table[1][1]);
  }

  TEST_METHOD(TestTwoMerge) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.push_back(std::make_pair(1, 21));
    test_mergepair_list.push_back(std::make_pair(5, 22));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictLeft, 0, -1, "a",
                            "c");

    test_mergepair_list2.push_back(std::make_pair(8, 21));
    test_mergepair_list2.push_back(std::make_pair(9, 23));

    // Conflict with column 2
    pql_result.MergeResults(test_mergepair_list2, kOneConflictRight, -1, 2, "d",
                            "c");

    ResultTable result_table = pql_result.GetResultTable();
    ResultRow result_row_column = result_table.front();
    ColumnHeader col_header = pql_result.GetColumnHeader();

    // Should have four column
    Assert::IsTrue(result_row_column.size() == 4);
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

    int expected_result1 = 1;
    int expected_result8 = 8;
    int expected_resulta = 7;
    int expected_resultx = 21;

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // First row third col should contain "x"
    Assert::AreEqual(expected_resultx, result_table[0][2]);
    // First row last col should contain "8"
    Assert::AreEqual(expected_result8, result_table[0][3]);
  }

  TEST_METHOD(TestTwoMergeNoResult) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair(1, 7));
    test_resultpair_list.push_back(std::make_pair(2, 8));
    test_resultpair_list.push_back(std::make_pair(3, 9));
    test_resultpair_list.push_back(std::make_pair(4, 10));
    test_resultpair_list.push_back(std::make_pair(5, 11));

    pql_result.InitTable(test_resultpair_list, "a", "b");

    test_mergepair_list.push_back(std::make_pair(1, 21));
    test_mergepair_list.push_back(std::make_pair(5, 22));

    // Conflict with column 0
    pql_result.MergeResults(test_mergepair_list, kOneConflictLeft, 0, -1, "a",
                            "c");

    test_mergepair_list2.push_back(std::make_pair(8, 21));
    test_mergepair_list2.push_back(std::make_pair(9, 23));

    // Conflict with column 2
    pql_result.MergeResults(test_mergepair_list2, kOneConflictRight, -1, 2, "d",
                            "c");

    // This final constraint 1 b does not match the table with 1 a
    test_mergepair_list3.push_back(std::make_pair(1, 8));
    test_mergepair_list3.push_back(std::make_pair(2, 7));
    test_mergepair_list3.push_back(std::make_pair(1, 8));
    test_mergepair_list3.push_back(std::make_pair(3, 9));

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