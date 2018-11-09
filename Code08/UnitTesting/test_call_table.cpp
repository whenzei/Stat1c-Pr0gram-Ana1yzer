#include "stdafx.h"
#include "CPPUnitTest.h"
#include "call_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {
TEST_CLASS(TestCallTable) {
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
        call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    Assert::IsTrue(result);
    result = call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    Assert::IsFalse(result);
    result = call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestGetCallingStmts) {
    CallTable call_table;
    call_table.InsertCalls(kStmtNum1, kProcIndex1);
    call_table.InsertCalls(kStmtNum2, kProcIndex3);
    call_table.InsertCalls(kStmtNum3, kProcIndex2);
    StmtNumSet calling_stmts = call_table.GetCallingStmts(kProcIndex3);
    Assert::IsTrue(calling_stmts.size() == 1);
    StmtNumSet expected_results = StmtNumSet{kStmtNum2};
    Assert::IsTrue(expected_results == calling_stmts);
  }

  TEST_METHOD(TestGetAllCallingStmtPairs) {
    CallTable call_table;
    call_table.InsertCalls(kStmtNum1, kProcIndex1);
    call_table.InsertCalls(kStmtNum2, kProcIndex3);
    call_table.InsertCalls(kStmtNum3, kProcIndex2);
    StmtNumProcPairSet stmt_proc_pair_set =
        call_table.GetAllCallingStmtPairs();
    Assert::IsTrue(stmt_proc_pair_set.size() == 3);
    StmtNumProcPairSet expected_results = StmtNumProcPairSet{ {kStmtNum1, kProcIndex1},
                                                              {kStmtNum2, kProcIndex3},
                                                              {kStmtNum3, kProcIndex2} };
    Assert::IsTrue(expected_results == stmt_proc_pair_set);
  }

  TEST_METHOD(TestGetCallee) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    ProcIndexSet direct_callee_set;
    direct_callee_set = call_table.GetCallee(kProcIndex2);
    Assert::IsTrue(direct_callee_set.size() == 0);
    direct_callee_set = call_table.GetCallee(kProcIndex1);
    Assert::IsTrue(direct_callee_set.size() == 1);
    ProcIndexSet expected_results_1 = { kProcIndex2 };
    Assert::IsTrue(expected_results_1 == direct_callee_set);
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex3);
    direct_callee_set = call_table.GetCallee(kProcIndex1);
    Assert::IsTrue(direct_callee_set.size() == 2);
    ProcIndexSet expected_results_2 = { kProcIndex2, kProcIndex3 };
    Assert::IsTrue(expected_results_2 == direct_callee_set);
  }

  TEST_METHOD(TestGetCalleeT) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    ProcIndexSet callee_set;
    callee_set = call_table.GetCalleeT(kProcIndex2);
    Assert::IsTrue(callee_set.size() == 0);
    callee_set = call_table.GetCalleeT(kProcIndex1);
    Assert::IsTrue(callee_set.size() == 1);
    ProcIndexSet expected_results_1 = ProcIndexSet{ kProcIndex2 };
    Assert::IsTrue(expected_results_1 == callee_set);
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex3);
    callee_set = call_table.GetCalleeT(kProcIndex1);
    Assert::IsTrue(callee_set.size() == 2);
    ProcIndexSet expected_results_2 = ProcIndexSet{ kProcIndex2, kProcIndex3 };
    Assert::IsTrue(expected_results_2 == callee_set);
  }

  TEST_METHOD(TestGetCaller) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    ProcIndexSet direct_caller_set; 
    direct_caller_set = call_table.GetCaller(kProcIndex1);
    Assert::IsTrue(direct_caller_set.size() == 0);
    direct_caller_set = call_table.GetCaller(kProcIndex2);
    Assert::IsTrue(direct_caller_set.size() == 1);
    ProcIndexSet expected_results_1 = ProcIndexSet{ kProcIndex1 };
    Assert::IsTrue(expected_results_1 == direct_caller_set);
    call_table.InsertDirectCallRelationship(kProcIndex3, kProcIndex2);
    direct_caller_set = call_table.GetCaller(kProcIndex2);
    Assert::IsTrue(direct_caller_set.size() == 2);
    ProcIndexSet expected_results_2 = ProcIndexSet{ kProcIndex1, kProcIndex3 };
    Assert::IsTrue(expected_results_2 == direct_caller_set);
  }

  TEST_METHOD(TestGetCallerT) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    ProcIndexSet caller_set;
    caller_set = call_table.GetCallerT(kProcIndex1);
    caller_set = call_table.GetCallerT(kProcIndex2);
    Assert::IsTrue(caller_set.size() == 1);
    ProcIndexSet expected_results_1 = ProcIndexSet{ kProcIndex1 };
    Assert::IsTrue(expected_results_1 == caller_set);
    call_table.InsertDirectCallRelationship(kProcIndex3, kProcIndex2);
    caller_set = call_table.GetCallerT(kProcIndex2);
    ProcIndexSet expected_results_2 = ProcIndexSet{ kProcIndex1, kProcIndex3 };
    Assert::IsTrue(expected_results_2 == caller_set);
  }

  TEST_METHOD(TestGetAllCaller) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    call_table.InsertDirectCallRelationship(kProcIndex3, kProcIndex4);
    ProcIndexSet caller_set;
    caller_set = call_table.GetAllCaller();
    Assert::IsTrue(caller_set.size() == 3);
    ProcIndexSet expected_results = ProcIndexSet{ kProcIndex1, kProcIndex2, kProcIndex3 };
    Assert::IsTrue(expected_results == caller_set);
  }

  TEST_METHOD(TestGetAllCallee) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    call_table.InsertDirectCallRelationship(kProcIndex3, kProcIndex4);
    ProcIndexSet callee_set;
    callee_set = call_table.GetAllCallee();
    Assert::IsTrue(callee_set.size() == 3);
    ProcIndexSet expected_results = ProcIndexSet{ kProcIndex2, kProcIndex3, kProcIndex4 };
    Assert::IsTrue(expected_results == callee_set);
  }

  TEST_METHOD(TestGetAllCalleeTwin) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    call_table.InsertDirectCallRelationship(kProcIndex3, kProcIndex4);
    ProcIndexPairSet callee_set;
    callee_set = call_table.GetAllCalleeTwin();
    Assert::IsTrue(callee_set.size() == 3);
    ProcIndexPairSet expected_results = ProcIndexPairSet{ {kProcIndex2, kProcIndex2},
                                                          {kProcIndex3, kProcIndex3},
                                                          {kProcIndex4, kProcIndex4} };
    Assert::IsTrue(expected_results == callee_set);
  }

  TEST_METHOD(TestGetCallPairs) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    call_table.InsertDirectCallRelationship(kProcIndex3, kProcIndex4);
    call_table.InsertIndirectCallRelationship(kProcIndex1, kProcIndex3);
    call_table.InsertIndirectCallRelationship(kProcIndex1, kProcIndex4);
    call_table.InsertIndirectCallRelationship(kProcIndex2, kProcIndex4);
    ProcIndexPairSet pair_set = call_table.GetAllCallPairs();
    Assert::IsTrue(pair_set.size() == 3);
    ProcIndexPairSet expected_results = ProcIndexPairSet{ {kProcIndex1, kProcIndex2},
                                                          {kProcIndex2, kProcIndex3},
                                                          {kProcIndex3, kProcIndex4} };
    Assert::IsTrue(expected_results == pair_set);
  }

  TEST_METHOD(TestGetAllCallTPairs) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    call_table.InsertDirectCallRelationship(kProcIndex3, kProcIndex4);
    call_table.InsertIndirectCallRelationship(kProcIndex1, kProcIndex3);
    call_table.InsertIndirectCallRelationship(kProcIndex1, kProcIndex4);
    call_table.InsertIndirectCallRelationship(kProcIndex2, kProcIndex4);
    ProcIndexPairSet pair_set = call_table.GetAllCallTPairs();
    Assert::IsTrue(pair_set.size() == 6);
    ProcIndexPairSet expected_results = ProcIndexPairSet{ {kProcIndex1, kProcIndex2},
                                                          {kProcIndex1, kProcIndex3},
                                                          {kProcIndex1, kProcIndex4},
                                                          {kProcIndex2, kProcIndex3},
                                                          {kProcIndex2, kProcIndex4},
                                                          {kProcIndex3, kProcIndex4} };
    Assert::IsTrue(expected_results == pair_set);
  }

  TEST_METHOD(TestIsCall) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex4);
    call_table.InsertIndirectCallRelationship(kProcIndex1, kProcIndex3);
    int result = call_table.IsCall(kProcIndex1, kProcIndex2);
    Assert::IsTrue(result);
    result = call_table.IsCall(kProcIndex1, kProcIndex4);
    Assert::IsTrue(result);
    result = call_table.IsCall(kProcIndex2, kProcIndex3);
    Assert::IsTrue(result);
    result = call_table.IsCall(kProcIndex1, kProcIndex3);
    Assert::IsFalse(result);
  }

  TEST_METHOD(TestIsCallT) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    call_table.InsertDirectCallRelationship(kProcIndex2, kProcIndex3);
    call_table.InsertIndirectCallRelationship(kProcIndex1, kProcIndex3);
    int result = call_table.IsCallT(kProcIndex1, kProcIndex2);
    Assert::IsTrue(result);
    result = call_table.IsCallT(kProcIndex2, kProcIndex3);
    Assert::IsTrue(result);
    result = call_table.IsCallT(kProcIndex1, kProcIndex3);
    Assert::IsTrue(result);
  }

  TEST_METHOD(TestIsCalledProc) {
    CallTable call_table;
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    int result = call_table.IsCalledProc(kProcIndex2);
    Assert::IsTrue(result);
    result = call_table.IsCalledProc(kProcIndex1);
    Assert::IsFalse(result);
    call_table.InsertIndirectCallRelationship(kProcIndex1, kProcIndex3);
    result = call_table.IsCalledProc(kProcIndex3);
    Assert::IsTrue(result);
  }

  TEST_METHOD(HasCallsRelationship) {
    CallTable call_table;
    int result = call_table.HasCallsRelationship();
    Assert::IsFalse(result);
    call_table.InsertDirectCallRelationship(kProcIndex1, kProcIndex2);
    result = call_table.HasCallsRelationship();
    Assert::IsTrue(result);
  }
};
}  // namespace PKBTests