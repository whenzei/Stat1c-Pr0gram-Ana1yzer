#include "stdafx.h"
#include "CPPUnitTest.h"
#include "call_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {
  TEST_CLASS(TestCallTable) {
    const ProcName kProcName1 = "abc";
    const ProcName kProcName2 = "getAllUsers";
    const ProcName kProcName3 = "toByteArray";
    const ProcName kProcName4 = "POP";
    const StmtNum kStmtNum1 = "1";
    const StmtNum kStmtNum2 = "4";
    const StmtNum kStmtNum3 = "893";

    TEST_METHOD(TestInsertDirectCallRelationship) {
      CallTable call_table;
      int result = call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      Assert::IsTrue(result);
      result = call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      Assert::IsFalse(result);
      result = call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      Assert::IsTrue(result);
    }

    TEST_METHOD(TestGetCallingStmts) {
      CallTable call_table;
      call_table.InsertCalls(kStmtNum1, kProcName1);
      call_table.InsertCalls(kStmtNum2, kProcName3);
      call_table.InsertCalls(kStmtNum3, kProcName2);
      StmtNumList calling_stmts = call_table.GetCallingStmts(kProcName3);
      Assert::IsTrue(calling_stmts.size()==1);
      StmtNumList::iterator iter = calling_stmts.begin();
      Assert::AreEqual(kStmtNum2, *iter);
    }

    TEST_METHOD(TestGetAllCallingStmtPairs) {
      CallTable call_table;
      call_table.InsertCalls(kStmtNum1, kProcName1);
      call_table.InsertCalls(kStmtNum2, kProcName3);
      call_table.InsertCalls(kStmtNum3, kProcName2);
      StmtNumProcPairList stmt_proc_pair_list = call_table.GetAllCallingStmtPairs();
      Assert::IsTrue(stmt_proc_pair_list.size() == 3);
      StmtNumProcPairList::iterator iter = stmt_proc_pair_list.begin();
      Assert::AreEqual(kStmtNum1, (*iter).first);
      Assert::AreEqual(kProcName1, (*iter).second);
      iter++;
      Assert::AreEqual(kStmtNum2, (*iter).first);
      Assert::AreEqual(kProcName3, (*iter).second);
      iter++;
      Assert::AreEqual(kStmtNum3, (*iter).first);
      Assert::AreEqual(kProcName2, (*iter).second);
    }

    TEST_METHOD(TestGetCallee) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      ProcNameList direct_callee_list;
      direct_callee_list = call_table.GetCallee(kProcName2);
      Assert::IsTrue(direct_callee_list.size() == 0);
      direct_callee_list = call_table.GetCallee(kProcName1);
      Assert::IsTrue(direct_callee_list.size() == 1);
      Assert::AreEqual(kProcName2, direct_callee_list.front());
      call_table.InsertDirectCallRelationship(kProcName1, kProcName3);
      direct_callee_list = call_table.GetCallee(kProcName1);
      // Direct callee should still be kProcName2
      Assert::AreEqual(kProcName2, direct_callee_list.front());
    }

    TEST_METHOD(TestGetCalleeT) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      ProcNameList callee_list;
      callee_list = call_table.GetCalleeT(kProcName2);
      Assert::IsTrue(callee_list.size() == 0);
      callee_list = call_table.GetCalleeT(kProcName1);
      Assert::IsTrue(callee_list.size() == 1);
      Assert::AreEqual(kProcName2, callee_list.front());
      call_table.InsertDirectCallRelationship(kProcName1, kProcName3);
      callee_list = call_table.GetCalleeT(kProcName1);
      Assert::IsTrue(callee_list.size() == 2);
      Assert::AreEqual(kProcName2, callee_list.front());
      Assert::AreEqual(kProcName3, callee_list.back());
    }

    TEST_METHOD(TestGetCaller) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      ProcNameList direct_caller_list;
      direct_caller_list = call_table.GetCaller(kProcName1);
      Assert::IsTrue(direct_caller_list.size() == 0);
      direct_caller_list = call_table.GetCaller(kProcName2);
      Assert::IsTrue(direct_caller_list.size() == 1);
      Assert::AreEqual(kProcName1, direct_caller_list.front());
      call_table.InsertDirectCallRelationship(kProcName3, kProcName2);
      direct_caller_list = call_table.GetCaller(kProcName2);
      // Direct callee should still be kProcName1
      Assert::AreEqual(kProcName1, direct_caller_list.front());
    }

    TEST_METHOD(TestGetCallerT) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      ProcNameList caller_list;
      caller_list = call_table.GetCallerT(kProcName1);
      //Assert::IsTrue(caller_list.size() == 0);
      caller_list = call_table.GetCallerT(kProcName2);
      Assert::IsTrue(caller_list.size() == 1);
      Assert::AreEqual(kProcName1, caller_list.front());
      call_table.InsertDirectCallRelationship(kProcName3, kProcName2);
      caller_list = call_table.GetCallerT(kProcName2);
      Assert::AreEqual(kProcName1, caller_list.front());
      Assert::AreEqual(kProcName3, caller_list.back());
    }

    TEST_METHOD(TestGetAllCaller) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
      ProcNameList caller_list;
      caller_list = call_table.GetAllCaller();
      Assert::IsTrue(caller_list.size() == 3);
      ProcNameList::iterator iter = caller_list.begin();
      Assert::AreEqual(kProcName1, *iter);
      iter++;
      Assert::AreEqual(kProcName2, *iter);
      iter++;
      Assert::AreEqual(kProcName3, *iter);
    }

    TEST_METHOD(TestGetAllCallee) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
      ProcNameList callee_list;
      callee_list = call_table.GetAllCallee();
      Assert::IsTrue(callee_list.size() == 3);
      ProcNameList::iterator iter = callee_list.begin();
      Assert::AreEqual(kProcName2, *iter);
      iter++;
      Assert::AreEqual(kProcName3, *iter);
      iter++;
      Assert::AreEqual(kProcName4, *iter);
    }

	TEST_METHOD(TestGetAllCalleeTwin) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
      ProcNamePairList callee_list;
      callee_list = call_table.GetAllCalleeTwin();
      Assert::IsTrue(callee_list.size() == 3);
      ProcNamePairList::iterator iter = callee_list.begin();
      Assert::AreEqual(kProcName2, (*iter).first);
      Assert::AreEqual(kProcName2, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName3, (*iter).first);
      Assert::AreEqual(kProcName3, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName4, (*iter).first);
      Assert::AreEqual(kProcName4, (*iter).second);
    }

    TEST_METHOD(TestGetCallPairs) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
      call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
      call_table.InsertIndirectCallRelationship(kProcName1, kProcName4);
      call_table.InsertIndirectCallRelationship(kProcName2, kProcName4);
      StmtNumProcPairList pair_list = call_table.GetAllCallPairs();
      Assert::IsTrue(pair_list.size() == 3);
      StmtNumProcPairList::iterator iter = pair_list.begin();
      Assert::AreEqual(kProcName1, (*iter).first);
      Assert::AreEqual(kProcName2, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName2, (*iter).first);
      Assert::AreEqual(kProcName3, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName3, (*iter).first);
      Assert::AreEqual(kProcName4, (*iter).second);
    }

    TEST_METHOD(TestGetAllCallTPairs) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      call_table.InsertDirectCallRelationship(kProcName3, kProcName4);
      call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
      call_table.InsertIndirectCallRelationship(kProcName1, kProcName4);
      call_table.InsertIndirectCallRelationship(kProcName2, kProcName4);
      StmtNumProcPairList pair_list = call_table.GetAllCallTPairs();
      Assert::IsTrue(pair_list.size() == 6);
      StmtNumProcPairList::iterator iter = pair_list.begin();
      Assert::AreEqual(kProcName1, (*iter).first);
      Assert::AreEqual(kProcName2, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName1, (*iter).first);
      Assert::AreEqual(kProcName3, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName1, (*iter).first);
      Assert::AreEqual(kProcName4, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName2, (*iter).first);
      Assert::AreEqual(kProcName3, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName2, (*iter).first);
      Assert::AreEqual(kProcName4, (*iter).second);
      iter++;
      Assert::AreEqual(kProcName3, (*iter).first);
      Assert::AreEqual(kProcName4, (*iter).second);
    }

    TEST_METHOD(TestIsCall) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
      int result = call_table.IsCall(kProcName1, kProcName2);
      Assert::IsTrue(result);
      result = call_table.IsCallT(kProcName2, kProcName3);
      Assert::IsTrue(result);
      result = call_table.IsCall(kProcName1, kProcName3);
      Assert::IsFalse(result);
    }

    TEST_METHOD(TestIsCallT) {
      CallTable call_table;
      call_table.InsertDirectCallRelationship(kProcName1, kProcName2);
      call_table.InsertDirectCallRelationship(kProcName2, kProcName3);
      call_table.InsertIndirectCallRelationship(kProcName1, kProcName3);
      int result = call_table.IsCallT(kProcName1, kProcName2);
      Assert::IsTrue(result);
      result = call_table.IsCallT(kProcName2, kProcName3);
      Assert::IsTrue(result);
      result = call_table.IsCallT(kProcName1, kProcName3);
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
}