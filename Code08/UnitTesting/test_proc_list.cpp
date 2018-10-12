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
};
}  // namespace UnitTesting
