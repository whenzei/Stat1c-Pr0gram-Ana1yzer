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

  TEST_METHOD(TestInsertProc) {
    ProcList proc_list;
    int result = proc_list.InsertProcName(kProcName1);
    Assert::AreEqual(1, result);
    result = proc_list.InsertProcName(kProcName2);
    Assert::AreEqual(2, result);
    result = proc_list.InsertProcName(kProcName2);
    Assert::AreEqual(-1, result);
  }

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

  TEST_METHOD(TestGetIndexToProcMapping) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    IndexProcMap index_proc_map = proc_list.GetIndexToProcMapping();
    Assert::AreEqual(kProcName1,index_proc_map[1]);
    index_proc_map = proc_list.GetIndexToProcMapping();
    Assert::AreEqual(kProcName2, index_proc_map[2]);
    // Duplicate. Shouldn't affect index
    proc_list.InsertProcName(kProcName2);
    index_proc_map = proc_list.GetIndexToProcMapping();    
    Assert::AreEqual(kProcName2, index_proc_map[2]);
  }

  TEST_METHOD(TestGetProcToIndexMapping) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    ProcIndexMap proc_index_map = proc_list.GetProcToIndexMapping();
    Assert::AreEqual(1, proc_index_map[kProcName1]);
    proc_index_map = proc_list.GetProcToIndexMapping();
    Assert::AreEqual(2, proc_index_map[kProcName2]);
    // Duplicate. Shouldn't affect index
    proc_list.InsertProcName(kProcName2);
    proc_index_map = proc_list.GetProcToIndexMapping();
    Assert::AreEqual(2, proc_index_map[kProcName2]);
  }
};
}  // namespace UnitTesting
