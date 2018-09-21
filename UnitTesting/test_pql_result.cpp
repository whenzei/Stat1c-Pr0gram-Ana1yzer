#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_enum.h"
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
using ResultTable = vector<vector<string>>;
using ResultRow = vector<string>;
using QueryResultList = vector<string>;
using QueryResultPairList = vector<pair<string, string>>;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestPqlResult) {
 public:
  QueryResultList test_result_list;

  TEST_METHOD(TestInitTable) {
    PqlResult pql_result;

    test_result_list.push_back("1");
    test_result_list.push_back("2");
    test_result_list.push_back("3");

    pql_result.InitTable(test_result_list);

    ResultTable result_table = pql_result.GetResultTable();

    // Should have three rows
    Assert::IsTrue(result_table.size() == 3);

    string expected_result = "1";

    // First row should contain "1"
    Assert::AreEqual(expected_result, result_table.front().front());

    expected_result = "3";

    // Last row should contain "3"
    Assert::AreEqual(expected_result, result_table.back().front());
  }



};
}  // namespace PQLTests