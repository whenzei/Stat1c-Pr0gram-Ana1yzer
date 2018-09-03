#include "stdafx.h"
#include "CppUnitTest.h"
#include "proc_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestProcTable) {
  const int kSuccess = 0;
  const int kFail = -1;
  const ProcName kProcName1 = "simple";
  const ProcName kProcName2 = "SimPle";
  const ProcName kProcName3 = "simple123";
  const ProcName kProcName4 = "";

 public:
  TEST_METHOD(TestInsertSuccess) {
    ProcTable proc_table;
    int status = proc_table.InsertProcName(kProcName1);
    Assert::AreEqual(kSuccess, status);
  }

  TEST_METHOD(TestInsertEmpty) {
    ProcTable proc_table;
    int status = proc_table.InsertProcName(kProcName4);
    Assert::AreEqual(kFail, status);
  }

  TEST_METHOD(TestInsertDuplicate) {
    ProcTable proc_table;
    proc_table.InsertProcName(kProcName1);
    int status = proc_table.InsertProcName(kProcName1);
    Assert::AreEqual(kFail, status);
  }

  TEST_METHOD(TestInsertMultiple) {
    ProcTable proc_table;
    int status_1 = proc_table.InsertProcName(kProcName1);
    int status_2 = proc_table.InsertProcName(kProcName2);
    int status_3 = proc_table.InsertProcName(kProcName3);
    Assert::AreEqual(kSuccess, status_1);
    Assert::AreEqual(kSuccess, status_2);
    Assert::AreEqual(kSuccess, status_3);
  }

  TEST_METHOD(TestGetProcTable) {
    ProcTable proc_table;
    proc_table.InsertProcName(kProcName1);
    ProcList proc_table_result = proc_table.GetProcTable();
    Assert::AreEqual(kProcName1, proc_table_result.front());
  }
  TEST_METHOD(TestGetProcTableMultiple) {
    ProcTable proc_table;
    proc_table.InsertProcName(kProcName1);
    proc_table.InsertProcName(kProcName2);
    proc_table.InsertProcName(kProcName3);
    ProcList proc_table_result = proc_table.GetProcTable();
    Assert::AreEqual(kProcName1, proc_table_result.front());
    ProcList::iterator iter = proc_table_result.begin();
    iter++;
    Assert::AreEqual(kProcName2, *iter);
    Assert::AreEqual(kProcName3, proc_table_result.back());
  }
};
}  // namespace UnitTesting
