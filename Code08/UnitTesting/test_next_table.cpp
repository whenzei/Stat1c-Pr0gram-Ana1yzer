#include "stdafx.h"
#include "CppUnitTest.h"
#include "next_table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBTests {

TEST_CLASS(TestNextTable) {
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 2;
  const StmtNum kStmtNum3 = 3;
  const StmtNum kStmtNum4 = 4;
  const ProcName kProcName1 = "One";
  
  TEST_METHOD(TestIsNextRelationship) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(next_table.IsNext(kStmtNum1, kStmtNum2));
    Assert::IsTrue(next_table.IsNext(kStmtNum2, kStmtNum3));
    Assert::IsTrue(next_table.IsNext(kStmtNum2, kStmtNum4));
    Assert::IsFalse(next_table.IsNext(kStmtNum1, kStmtNum3));
    Assert::IsFalse(next_table.IsNext(kStmtNum1, kStmtNum4));
  }

  TEST_METHOD(TestIsNext) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(next_table.IsNext(kStmtNum2));
    Assert::IsTrue(next_table.IsNext(kStmtNum3));
    Assert::IsTrue(next_table.IsNext(kStmtNum4));
    Assert::IsFalse(next_table.IsNext(kStmtNum1));
  }

  TEST_METHOD(TestIsPrevious) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(next_table.IsPrevious(kStmtNum1));
    Assert::IsTrue(next_table.IsPrevious(kStmtNum2));
    Assert::IsFalse(next_table.IsPrevious(kStmtNum3));
    Assert::IsFalse(next_table.IsPrevious(kStmtNum4));
  }

  TEST_METHOD(TestGetNext) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = next_table.GetNext(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum2, result1.front());
    StmtNumList result2 = next_table.GetNext(kStmtNum2);
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum3, result2.front());
    Assert::AreEqual(kStmtNum4, result2.back());
    StmtNumList result3 = next_table.GetNext(kStmtNum3);
    Assert::IsTrue(result3.empty());
    StmtNumList result4 = next_table.GetNext(kStmtNum4);
    Assert::IsTrue(result4.empty());
  }

  TEST_METHOD(TestGetPrevious) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = next_table.GetPrevious(kStmtNum1);
    Assert::IsTrue(result1.empty());
    StmtNumList result2 = next_table.GetPrevious(kStmtNum2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    StmtNumList result3 = next_table.GetPrevious(kStmtNum3);
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum2, result3.front());
    StmtNumList result4 = next_table.GetPrevious(kStmtNum4);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }

  TEST_METHOD(TestGetAllNext) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = next_table.GetAllNext();
    Assert::IsTrue(result1.size() == 3);
    StmtNumList::iterator iter = result1.begin();
    Assert::AreEqual(kStmtNum2, *iter);
    iter++;
    Assert::AreEqual(kStmtNum3, *iter);
    iter++;
    Assert::AreEqual(kStmtNum4, *iter);
  }

  TEST_METHOD(TestGetAllPrevious) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = next_table.GetAllPrevious();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestHasNextRelationship) {
    NextTable next_table;
    next_table.InsertCFG(kProcName1);
    Assert::IsFalse(next_table.HasNextRelationship());
    next_table.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    next_table.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(next_table.HasNextRelationship());
  }
};
}  // namespace UnitTesting