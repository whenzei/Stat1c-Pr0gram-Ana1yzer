#include "stdafx.h"
#include "CppUnitTest.h"
#include "proc_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestProcList) {
  const ProcName kProcName1 = "simple";
  const ProcName kProcName2 = "SimPle";
  const ProcName kProcName3 = "simple123";

 public:
  TEST_METHOD(TestInsertSuccess) {
    ProcList proc_list;
    bool status = proc_list.InsertProcName(kProcName1);
    Assert::IsTrue(status);
  }

  TEST_METHOD(TestInsertDuplicate) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    bool status = proc_list.InsertProcName(kProcName1);
    Assert::IsFalse(status);
  }

  TEST_METHOD(TestInsertMultiple) {
    ProcList proc_list;
    bool status_1 = proc_list.InsertProcName(kProcName1);
    bool status_2 = proc_list.InsertProcName(kProcName2);
    bool status_3 = proc_list.InsertProcName(kProcName3);
    Assert::IsTrue(status_1);
    Assert::IsTrue(status_2);
    Assert::IsTrue(status_3);
  }

  TEST_METHOD(TestGetProcList) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    ProcNameList proc_list_result = proc_list.GetAllProcName();
    Assert::AreEqual(kProcName1, proc_list_result.front());
  }
  TEST_METHOD(TestGetProcListMultiple) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    proc_list.InsertProcName(kProcName3);
    ProcNameList proc_list_result = proc_list.GetAllProcName();
    Assert::AreEqual(kProcName1, proc_list_result.front());
    ProcNameList::iterator iter = proc_list_result.begin();
    iter++;
    Assert::AreEqual(kProcName2, *iter);
    Assert::AreEqual(kProcName3, proc_list_result.back());
  }
};
}  // namespace UnitTesting
