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

  TEST_METHOD(TestGetAllProcName) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    proc_list.InsertProcName(kProcName3);
	// duplicate
    proc_list.InsertProcName(kProcName1);
    ProcNameList proc_list_result = proc_list.GetAllProcName();
    Assert::IsTrue(proc_list_result.size() == 3);
    Assert::AreEqual(kProcName1, proc_list_result.front());
    ProcNameList::iterator iter = proc_list_result.begin();
    iter++;
    Assert::AreEqual(kProcName2, *iter);
    Assert::AreEqual(kProcName3, proc_list_result.back());
  }

  TEST_METHOD(TestIsProcName) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    bool result = proc_list.IsProcName(kProcName1);
    Assert::IsTrue(result);
    result = proc_list.IsProcName(kProcName2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetAllProcNameTwin) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    // duplicate
    proc_list.InsertProcName(kProcName1);
    ProcNamePairList proc_list_result = proc_list.GetAllProcNameTwin();
    Assert::IsTrue(proc_list_result.size() == 2);
    Assert::AreEqual(kProcName1, proc_list_result.front().first);
    Assert::AreEqual(kProcName1, proc_list_result.front().second);
    Assert::AreEqual(kProcName2, proc_list_result.back().first);
    Assert::AreEqual(kProcName2, proc_list_result.back().second);
  }

  TEST_METHOD(TestGetIndexForProc) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    int proc_index1 = proc_list.GetIndexForProc(kProcName1);
    Assert::AreEqual(1,proc_index1);
    int proc_index2 = proc_list.GetIndexForProc(kProcName2);
    Assert::AreEqual(2, proc_index2);
    // Duplicate. Shouldn't affect index
    proc_list.InsertProcName(kProcName2);
    proc_index2 = proc_list.GetIndexForProc(kProcName2);
    Assert::AreEqual(2, proc_index2);
  }

  TEST_METHOD(TestGetProcForIndex) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    ProcName proc1 = proc_list.GetProcForIndex(1);
    Assert::AreEqual(proc1, kProcName1);
    ProcName proc2 = proc_list.GetProcForIndex(2);
    Assert::AreEqual(proc2, kProcName2);
    // Duplicate. Shouldn't affect index
    proc_list.InsertProcName(kProcName2);
    proc2 = proc_list.GetProcForIndex(2);
    Assert::AreEqual(proc2, kProcName2);
  }
};
}  // namespace UnitTesting
