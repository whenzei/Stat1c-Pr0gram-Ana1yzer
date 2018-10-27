#include "stdafx.h"
#include "CPPUnitTest.h"
#include "call_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {
TEST_CLASS(TestCallTable) {
  const ProcName kProcName1 = "one";
  const ProcName kProcName2 = "two";
  const ProcName kProcName3 = "three";
  const ProcName kProcName4 = "four";
  const ProcIndex kProcIndex1 = 0;
  const ProcIndex kProcIndex2 = 1;
  const ProcIndex kProcIndex3 = 2;
  const ProcIndex kProcIndex4 = 3;
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 4;
  const StmtNum kStmtNum3 = 893;

  TEST_METHOD(TestInsertDirectCallRelationship) {
    CallTable call_table;
    int result =
        call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    Assert::IsTrue(result);
    result = call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    Assert::IsFalse(result);
    result = call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetCallingStmts) {
    CallTable call_table;
    call_table.InsertCalls(kStmtNum1, kProcIndex1);
    call_table.InsertCalls(kStmtNum2, kProcIndex3);
    call_table.InsertCalls(kStmtNum3, kProcIndex2);
    StmtNumList calling_stmts = call_table.GetCallingStmts(kProcIndex3);
    Assert::IsTrue(calling_stmts.size() == 1);
    StmtNumList::iterator iter = calling_stmts.begin();
    Assert::AreEqual(kStmtNum2, *iter);
  }

  TEST_METHOD(TestGetAllCallingStmtPairs) {
    CallTable call_table;
    call_table.InsertCalls(kStmtNum1, kProcIndex1);
    call_table.InsertCalls(kStmtNum2, kProcIndex3);
    call_table.InsertCalls(kStmtNum3, kProcIndex2);
    StmtNumProcPairList stmt_proc_pair_list =
        call_table.GetAllCallingStmtPairs();
    Assert::IsTrue(stmt_proc_pair_list.size() == 3);
    StmtNumProcPairList::iterator iter = stmt_proc_pair_list.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kProcIndex1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kProcIndex3, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kProcIndex2, (*iter).second);
  }

  TEST_METHOD(TestGetCallee) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    ProcIndexList direct_callee_list;
    direct_callee_list = call_table.GetCallee(kProcIndex2);
    Assert::IsTrue(direct_callee_list.size() == 0);
    direct_callee_list = call_table.GetCallee(kProcIndex1);
    Assert::IsTrue(direct_callee_list.size() == 1);
    Assert::AreEqual(kProcIndex2, direct_callee_list.front());
    call_table.InsertDirectCallRelationship(kProcName1, kProcName3);
    direct_callee_list = call_table.GetCallee(kProcIndex1);
    Assert::IsTrue(direct_callee_list.size() == 2);
    Assert::AreEqual(kProcIndex2, direct_callee_list.front());
    Assert::AreEqual(kProcIndex3, direct_callee_list.back());
  }

  TEST_METHOD(TestGetCalleeT) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    ProcIndexList callee_list;
    callee_list = call_table.GetCalleeT(kProcIndex2);
    Assert::IsTrue(callee_list.size() == 0);
    callee_list = call_table.GetCalleeT(kProcIndex1);
    Assert::IsTrue(callee_list.size() == 1);
    Assert::AreEqual(kProcIndex2, callee_list.front());
    call_table.InsertDirectCallRelationship(kProcName1, kProcName3);
    callee_list = call_table.GetCalleeT(kProcIndex1);
    Assert::IsTrue(callee_list.size() == 2);
    Assert::AreEqual(kProcIndex2, callee_list.front());
    Assert::AreEqual(kProcIndex3, callee_list.back());
  }

  TEST_METHOD(TestGetCaller) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    ProcIndexList direct_caller_list;
    direct_caller_list = call_table.GetCaller(kProcIndex1);
    Assert::IsTrue(direct_caller_list.size() == 0);
    direct_caller_list = call_table.GetCaller(kProcIndex2);
    Assert::IsTrue(direct_caller_list.size() == 1);
    Assert::AreEqual(kProcIndex1, direct_caller_list.front());
    call_table.InsertDirectCallRelationship(kProcName3, kProcName2);
    direct_caller_list = call_table.GetCaller(kProcIndex2);
    Assert::IsTrue(direct_caller_list.size() == 2);
    Assert::AreEqual(kProcIndex1, direct_caller_list.front());
    Assert::AreEqual(kProcIndex3, direct_caller_list.back());
  }

  TEST_METHOD(TestGetCallerT) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    ProcIndexList caller_list;
    caller_list = call_table.GetCallerT(kProcIndex1);
    // Assert::IsTrue(caller_list.size() == 0);
    caller_list = call_table.GetCallerT(kProcIndex2);
    Assert::IsTrue(caller_list.size() == 1);
    Assert::AreEqual(kProcIndex1, caller_list.front());
    call_table.InsertDirectCallRelationship(kProcName3, kProcName2);
    caller_list = call_table.GetCallerT(kProcIndex2);
    Assert::AreEqual(kProcIndex1, caller_list.front());
    Assert::AreEqual(kProcIndex3, caller_list.back());
  }

  TEST_METHOD(TestGetAllCaller) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
    ProcIndexList caller_list;
    caller_list = call_table.GetAllCaller();
    Assert::IsTrue(caller_list.size() == 3);
    ProcIndexList::iterator iter = caller_list.begin();
    Assert::AreEqual(kProcIndex1, *iter);
    iter++;
    Assert::AreEqual(kProcIndex2, *iter);
    iter++;
    Assert::AreEqual(kProcIndex3, *iter);
  }

  TEST_METHOD(TestGetAllCallee) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
    ProcIndexList callee_list;
    callee_list = call_table.GetAllCallee();
    Assert::IsTrue(callee_list.size() == 3);
    ProcIndexList::iterator iter = callee_list.begin();
    Assert::AreEqual(kProcIndex2, *iter);
    iter++;
    Assert::AreEqual(kProcIndex3, *iter);
    iter++;
    Assert::AreEqual(kProcIndex4, *iter);
  }

  TEST_METHOD(TestGetAllCalleeTwin) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
    ProcIndexPairList callee_list;
    callee_list = call_table.GetAllCalleeTwin();
    Assert::IsTrue(callee_list.size() == 3);
    ProcIndexPairList::iterator iter = callee_list.begin();
    Assert::AreEqual(kProcIndex2, (*iter).first);
    Assert::AreEqual(kProcIndex2, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex3, (*iter).first);
    Assert::AreEqual(kProcIndex3, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex4, (*iter).first);
    Assert::AreEqual(kProcIndex4, (*iter).second);
  }

  TEST_METHOD(TestGetCallPairs) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
    call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
    call_table.InsertIndirectCallRelationship(kProcName1, kProcName4);
    call_table.InsertIndirectCallRelationship(kProcName2, kProcName4);
    ProcIndexPairList pair_list = call_table.GetAllCallPairs();
    Assert::IsTrue(pair_list.size() == 3);
    ProcIndexPairList::iterator iter = pair_list.begin();
    Assert::AreEqual(kProcIndex1, (*iter).first);
    Assert::AreEqual(kProcIndex2, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex2, (*iter).first);
    Assert::AreEqual(kProcIndex3, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex3, (*iter).first);
    Assert::AreEqual(kProcIndex4, (*iter).second);
  }

  TEST_METHOD(TestGetAllCallTPairs) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
    call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
    call_table.InsertIndirectCallRelationship(kProcName1, kProcName4);
    call_table.InsertIndirectCallRelationship(kProcName2, kProcName4);
    ProcIndexPairList pair_list = call_table.GetAllCallTPairs();
    Assert::IsTrue(pair_list.size() == 6);
    ProcIndexPairList::iterator iter = pair_list.begin();
    Assert::AreEqual(kProcIndex1, (*iter).first);
    Assert::AreEqual(kProcIndex2, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex1, (*iter).first);
    Assert::AreEqual(kProcIndex3, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex1, (*iter).first);
    Assert::AreEqual(kProcIndex4, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex2, (*iter).first);
    Assert::AreEqual(kProcIndex3, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex2, (*iter).first);
    Assert::AreEqual(kProcIndex4, (*iter).second);
    iter++;
    Assert::AreEqual(kProcIndex3, (*iter).first);
    Assert::AreEqual(kProcIndex4, (*iter).second);
  }

  TEST_METHOD(TestIsCall) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
    int result = call_table.IsCall(kProcIndex1, kProcIndex2);
    Assert::IsTrue(result);
    result = call_table.IsCallT(kProcIndex2, kProcIndex3);
    Assert::IsTrue(result);
    result = call_table.IsCall(kProcIndex1, kProcIndex3);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestIsCallT) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
    call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
    int result = call_table.IsCallT(kProcIndex1, kProcIndex2);
    Assert::IsTrue(result);
    result = call_table.IsCallT(kProcIndex2, kProcIndex3);
    Assert::IsTrue(result);
    result = call_table.IsCallT(kProcIndex1, kProcIndex3);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestIsCalledProc) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    int result = call_table.IsCalledProc(kProcIndex2);
    Assert::IsTrue(result);
    result = call_table.IsCalledProc(kProcIndex1);
    Assert::IsFalse(result);
    call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
    result = call_table.IsCalledProc(kProcIndex3);
    Assert::IsTrue(result);
  }

  TEST_METHOD(HasCallsRelationship) {
    CallTable call_table;
    int result = call_table.HasCallsRelationship();
    Assert::IsFalse(result);
    call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
    result = call_table.HasCallsRelationship();
    Assert::IsTrue(result);
  }
};
}  // namespace PKBTests