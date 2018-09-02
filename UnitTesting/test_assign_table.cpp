#include <string>
#include <sstream>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "assign_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::stringstream;
using std::vector;

namespace UnitTesting {

TEST_CLASS(TestAssignTable) {
  const int kSuccess = 0;
  const int kFail = -1;
  const int kStmtNum1 = 2;
  const int kStmtNum2 = 4;
  const int kStmtNum3 = 5;
  const string kAssignStmt1 = "a = 3";
  const string kAssignStmt2 = "b = 4";
  const string kAssignStmt3 = "c = 5";
  const string kAssignStmt4 = "";

 public:
  TEST_METHOD(TestInsertSuccess) {
    AssignTable assign_table;
    int status = assign_table.Insert(kStmtNum1, kAssignStmt1);
    Assert::AreEqual(kSuccess, status);
  }

  TEST_METHOD(TestInsertEmpty) {
    AssignTable assign_table;
    int status = assign_table.Insert(kStmtNum1, kAssignStmt4);
    Assert::AreEqual(kFail, status);
  }

  TEST_METHOD(TestInsertDuplicate) {
    AssignTable assign_table;
    assign_table.Insert(kStmtNum1, kAssignStmt1);
    int status = assign_table.Insert(kStmtNum1, kAssignStmt2);
    Assert::AreEqual(kFail, status);
  }

  TEST_METHOD(TestInsertMultiple) {
    AssignTable assign_table;
    int status_1 = assign_table.Insert(kStmtNum1, kAssignStmt1);
    int status_2 = assign_table.Insert(kStmtNum2, kAssignStmt2);
    int status_3 = assign_table.Insert(kStmtNum3, kAssignStmt3);
    Assert::AreEqual(kSuccess, status_1);
    Assert::AreEqual(kSuccess, status_2);
    Assert::AreEqual(kSuccess, status_3);
  }

  TEST_METHOD(TestList) {
    AssignTable assign_table;
    assign_table.Insert(kStmtNum1, kAssignStmt1);
    vector<string> assign_table_result = assign_table.List();
    stringstream stmt_num1_ss;
    stmt_num1_ss << kStmtNum1;
    Assert::AreEqual(stmt_num1_ss.str(), assign_table_result.front());
  }

  TEST_METHOD(TestListMultiple) {
    AssignTable assign_table;
    vector<string> assign_table_result;
    assign_table.Insert(kStmtNum1, kAssignStmt1);
    assign_table.Insert(kStmtNum2, kAssignStmt2);
    assign_table.Insert(kStmtNum3, kAssignStmt3);
    assign_table_result = assign_table.List();
    stringstream stmt_num1_ss;
    stringstream stmt_num2_ss;
    stringstream stmt_num3_ss;
    stmt_num1_ss << kStmtNum1;
    stmt_num2_ss << kStmtNum2;
    stmt_num3_ss << kStmtNum3;
    Assert::AreEqual(stmt_num1_ss.str(), assign_table_result.at(0));
    Assert::AreEqual(stmt_num2_ss.str(), assign_table_result.at(1));
    Assert::AreEqual(stmt_num3_ss.str(), assign_table_result.at(2));
  }
};
}  // namespace UnitTesting
