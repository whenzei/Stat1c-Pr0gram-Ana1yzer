#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "proc_table.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestProcTable) {
  const int kSuccess = 0;
  const int kFail = -1;
  const string kProcName1 = "simple";
  const string kProcName2 = "SimPle";
  const string kProcName3 = "simple123";
  const string kProcName4 = "";

 public:
  TEST_METHOD(TestInsertSuccess) {
    ProcTable proc_table;
    int status = proc_table.Insert(kProcName1);
    Assert::AreEqual(kSuccess, status);
  }

  TEST_METHOD(TestInsertEmpty) {
    ProcTable proc_table;
    int status = proc_table.Insert(kProcName4);
    Assert::AreEqual(kFail, status);
  }

  TEST_METHOD(TestInsertDuplicate) {
    ProcTable proc_table;
    proc_table.Insert(kProcName1);
    int status = proc_table.Insert(kProcName1);
    Assert::AreEqual(kFail, status);
  }

  TEST_METHOD(TestInsertMultiple) {
    ProcTable proc_table;
    int status_1 = proc_table.Insert(kProcName1);
    int status_2 = proc_table.Insert(kProcName2);
    int status_3 = proc_table.Insert(kProcName3);
    Assert::AreEqual(kSuccess, status_1);
    Assert::AreEqual(kSuccess, status_2);
    Assert::AreEqual(kSuccess, status_3);
  }

  TEST_METHOD(TestList) {
    ProcTable proc_table;
    proc_table.Insert(kProcName1);
    vector<string> proc_table_result = proc_table.List();
    Assert::AreEqual(kProcName1, proc_table_result.front());
  }
  TEST_METHOD(TestListMultiple) {
    ProcTable proc_table;
    proc_table.Insert(kProcName1);
    proc_table.Insert(kProcName2);
    proc_table.Insert(kProcName3);
    vector<string> proc_table_result = proc_table.List();
    Assert::AreEqual(kProcName1, proc_table_result.at(0));
    Assert::AreEqual(kProcName2, proc_table_result.at(1));
    Assert::AreEqual(kProcName3, proc_table_result.at(2));
  }
};
}  // namespace UnitTesting
