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
};
}  // namespace UnitTesting
