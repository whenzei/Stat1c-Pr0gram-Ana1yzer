#include "stdafx.h"
#include "CppUnitTest.h"
#include "proc_list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestProcList) {
  const ProcName kProcName1 = "simple";
  const ProcName kProcName2 = "SimPle";
  const ProcName kProcName3 = "simple123";
  const ProcIndex kProcIndex1 = 0;
  const ProcIndex kProcIndex2 = 1;
  const ProcIndex kProcIndex3 = 2;

 public:

  TEST_METHOD(TestInsertProc) {
    ProcList proc_list;
    int result = proc_list.InsertProcName(kProcName1);
    Assert::AreEqual(0, result);
    result = proc_list.InsertProcName(kProcName2);
    Assert::AreEqual(1, result);
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
    ProcIndexList proc_list_result = proc_list.GetAllProcIndices();
    Assert::IsTrue(proc_list_result.size() == 3);
    Assert::AreEqual(kProcIndex1, proc_list_result.front());
    ProcIndexList::iterator iter = proc_list_result.begin();
    iter++;
    Assert::AreEqual(kProcIndex2, *iter);
    Assert::AreEqual(kProcIndex3, proc_list_result.back());
  }

  TEST_METHOD(TestIsProcName) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    bool result = proc_list.IsProcName(kProcName1);
    Assert::IsTrue(result);
    result = proc_list.IsProcName(kProcName2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestIsProcIndex) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    bool result = proc_list.IsProcIndex(kProcIndex1);
    Assert::IsTrue(result);
    result = proc_list.IsProcIndex(kProcIndex2);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestGetAllProcNameTwin) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    // duplicate
    proc_list.InsertProcName(kProcName1);
    ProcIndexPairList proc_list_result = proc_list.GetAllProcIndexTwin();
    Assert::IsTrue(proc_list_result.size() == 2);
    Assert::AreEqual(kProcIndex1, proc_list_result.front().first);
    Assert::AreEqual(kProcIndex1, proc_list_result.front().second);
    Assert::AreEqual(kProcIndex2, proc_list_result.back().first);
    Assert::AreEqual(kProcIndex2, proc_list_result.back().second);
  }

  TEST_METHOD(TestGetIndexToProcMapping) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    IndexProcMap index_proc_map = proc_list.GetIndexToProcMapping();
    Assert::AreEqual(kProcName1, index_proc_map[0]);
    index_proc_map = proc_list.GetIndexToProcMapping();
    Assert::AreEqual(kProcName2, index_proc_map[1]);
    // Duplicate. Shouldn't affect index
    proc_list.InsertProcName(kProcName2);
    index_proc_map = proc_list.GetIndexToProcMapping();    
    Assert::AreEqual(kProcName2, index_proc_map[1]);
  }

  TEST_METHOD(TestGetProcToIndexMapping) {
    ProcList proc_list;
    proc_list.InsertProcName(kProcName1);
    proc_list.InsertProcName(kProcName2);
    ProcIndexMap proc_index_map = proc_list.GetProcToIndexMapping();
    Assert::AreEqual(0, proc_index_map[kProcName1]);
    proc_index_map = proc_list.GetProcToIndexMapping();
    Assert::AreEqual(1, proc_index_map[kProcName2]);
    // Duplicate. Shouldn't affect index
    proc_list.InsertProcName(kProcName2);
    proc_index_map = proc_list.GetProcToIndexMapping();
    Assert::AreEqual(1, proc_index_map[kProcName2]);
  }
};
}  // namespace UnitTesting
