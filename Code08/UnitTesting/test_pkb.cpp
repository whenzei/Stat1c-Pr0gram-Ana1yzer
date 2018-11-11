#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using TokenType = Tokenizer::TokenType;

namespace PKBTests {
TEST_CLASS(TestPKB) {
  const ProcName kProcName1 = "one";
  const ProcName kProcName2 = "two";
  const ProcName kProcName3 = "three";
  const ProcIndex kProcIndex1 = 0;
  const ProcIndex kProcIndex2 = 1;
  const ProcIndex kProcIndex3 = 2;
  const StmtNum kStmtNum1 = 1;
  const StmtNum kStmtNum2 = 2;
  const StmtNum kStmtNum3 = 3;
  const StmtNum kStmtNum4 = 4;
  const StmtNum kStmtNum5 = 5;
  const StmtNum kStmtNum6 = 6;
  const StmtNum kStmtNum7 = 7;
  const VarName kVarName1 = "a";
  const VarName kVarName2 = "b";
  const VarName kVarName3 = "c";
  const VarIndex kVarIndex1 = 0;
  const VarIndex kVarIndex2 = 1;
  const VarIndex kVarIndex3 = 2;
  const ConstValue kConstValue1 = 12;
  const ConstValue kConstValue2 = 345;
  const VarNameSet kVarNameSet1 = {"b", "c"};
  const VarNameSet kVarNameSet2 = {"d", "e"};
  const VarNameSet kVarNameSet3 = {"f", "g"};
  const ConstValueSet kConstValueSet1 = {1, 2};
  const Token kToken32 = {TokenType::kDigit, "32"};
  const Token kTokenA = {TokenType::kName, "a"};
  const Token kTokenPlus = {TokenType::kOperator, "+"};
  const Token kTokenB = {TokenType::kName, "b"};
  const Token kTokenMult = {TokenType::kOperator, "*"};
  const TokenList kTokenList = TokenList();
  const TokenList kTokenList1 = {kToken32, kTokenA, kTokenPlus};
  const TokenList kTokenList2 = {kToken32, kTokenA, kTokenPlus, kTokenB,
                                 kTokenMult};
  const TokenList kTokenList3 = {kToken32};
  PKB pkb;

 public:
  TEST_METHOD(TestGetAllProc) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertProcName(kProcName2);
    ProcIndexList proc_indices = pkb.GetAllProcIndices();
    Assert::IsTrue(proc_indices.size() == 2);
    Assert::AreEqual(kProcIndex1, proc_indices[0]);
    Assert::AreEqual(kProcIndex2, proc_indices[1]);
    ProcNameList proc_names = pkb.GetAllProcNames();
    Assert::IsTrue(proc_names.size() == 2);
    Assert::AreEqual(kProcName1, proc_names[0]);
    Assert::AreEqual(kProcName2, proc_names[1]);
    ProcIndexPairList proc_index_pairs = pkb.GetAllProcIndexTwin();
    Assert::IsTrue(proc_index_pairs.size() == 2);
    Assert::AreEqual(kProcIndex1, proc_index_pairs[0].first);
    Assert::AreEqual(kProcIndex1, proc_index_pairs[0].second);
    Assert::AreEqual(kProcIndex2, proc_index_pairs[1].first);
    Assert::AreEqual(kProcIndex2, proc_index_pairs[1].second);
    ProcNamePairList proc_name_pairs = pkb.GetAllProcNameTwin();
    Assert::IsTrue(proc_name_pairs.size() == 2);
    Assert::AreEqual(kProcName1, proc_name_pairs[0].first);
    Assert::AreEqual(kProcName1, proc_name_pairs[0].second);
    Assert::AreEqual(kProcName2, proc_name_pairs[1].first);
    Assert::AreEqual(kProcName2, proc_name_pairs[1].first);
    Assert::IsTrue(pkb.IsProcName(kProcName1));
    Assert::IsTrue(pkb.IsProcName(kProcName2));
    Assert::IsFalse(pkb.IsProcName("not a proc"));
    Assert::IsTrue(pkb.IsProcIndex(kProcIndex1));
    Assert::IsTrue(pkb.IsProcIndex(kProcIndex2));
    Assert::IsFalse(pkb.IsProcIndex(100));
  }

  TEST_METHOD(TestProcMapping) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertProcName(kProcName2);
    ProcIndexMap proc_index_map = pkb.GetProcToIndexMapping();
    Assert::AreEqual(kProcIndex1, proc_index_map[kProcName1]);
    Assert::AreEqual(kProcIndex2, proc_index_map[kProcName2]);
    IndexProcMap index_proc_map = pkb.GetIndexToProcMapping();
    Assert::AreEqual(kProcName1, index_proc_map[kProcIndex1]);
    Assert::AreEqual(kProcName2, index_proc_map[kProcIndex2]);
    Assert::AreEqual(kProcName1, pkb.GetProcName(kProcIndex1));
    Assert::AreEqual(kProcName2, pkb.GetProcName(kProcIndex2));
    Assert::AreEqual(kProcIndex1, pkb.GetProcIndex(kProcName1));
    Assert::AreEqual(kProcIndex2, pkb.GetProcIndex(kProcName2));
  }

  TEST_METHOD(TestGetAllVar) {
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    VarIndexList var_indices = pkb.GetAllVarIndices();
    Assert::IsTrue(var_indices.size() == 3);
    Assert::AreEqual(kVarIndex1, var_indices[0]);
    Assert::AreEqual(kVarIndex2, var_indices[1]);
    Assert::AreEqual(kVarIndex3, var_indices[2]);
    VarIndexPairList var_index_pairs = pkb.GetAllVarIndexTwin();
    Assert::IsTrue(var_index_pairs.size() == 3);
    Assert::AreEqual(kVarIndex1, var_index_pairs[0].first);
    Assert::AreEqual(kVarIndex1, var_index_pairs[0].second);
    Assert::AreEqual(kVarIndex2, var_index_pairs[1].first);
    Assert::AreEqual(kVarIndex2, var_index_pairs[1].second);
    Assert::AreEqual(kVarIndex3, var_index_pairs[2].first);
    Assert::AreEqual(kVarIndex3, var_index_pairs[2].second);
    Assert::IsTrue(pkb.IsVarIndex(kVarIndex1));
    Assert::IsTrue(pkb.IsVarIndex(kVarIndex2));
    Assert::IsTrue(pkb.IsVarIndex(kVarIndex3));
    Assert::IsFalse(pkb.IsVarIndex(5));
    Assert::IsTrue(pkb.IsVarName(kVarName1));
    Assert::IsTrue(pkb.IsVarName(kVarName2));
    Assert::IsTrue(pkb.IsVarName(kVarName3));
    Assert::IsFalse(pkb.IsVarName("not a var"));
  }

  TEST_METHOD(TestVarMapping) {
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    VarIndexMap var_index_map = pkb.GetVarToIndexMapping();
    Assert::IsTrue(var_index_map.size() == 3);
    Assert::AreEqual(kVarIndex1, var_index_map[kVarName1]);
    Assert::AreEqual(kVarIndex2, var_index_map[kVarName2]);
    Assert::AreEqual(kVarIndex3, var_index_map[kVarName3]);
    IndexVarMap index_var_map = pkb.GetIndexToVarMapping();
    Assert::IsTrue(index_var_map.size() == 3);
    Assert::AreEqual(kVarName1, index_var_map[kVarIndex1]);
    Assert::AreEqual(kVarName2, index_var_map[kVarIndex2]);
    Assert::AreEqual(kVarName3, index_var_map[kVarIndex3]);
    Assert::AreEqual(kVarName1, pkb.GetVarName(kVarIndex1));
    Assert::AreEqual(kVarName2, pkb.GetVarName(kVarIndex2));
    Assert::AreEqual(kVarName3, pkb.GetVarName(kVarIndex3));
    Assert::AreEqual(kVarIndex1, pkb.GetVarIndex(kVarName1));
    Assert::AreEqual(kVarIndex2, pkb.GetVarIndex(kVarName2));
    Assert::AreEqual(kVarIndex3, pkb.GetVarIndex(kVarName3));
  }

  TEST_METHOD(TestReadPrintVar) {
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum1, kProcIndex1, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum2, kProcIndex1, kVarName2));
    Assert::AreEqual(kVarIndex1, pkb.GetReadVar(kStmtNum1));
    Assert::AreEqual(-1, pkb.GetReadVar(kStmtNum2));
    Assert::AreEqual(kVarIndex2, pkb.GetPrintVar(kStmtNum2));
    Assert::AreEqual(-1, pkb.GetPrintVar(kStmtNum3));
    Assert::AreEqual(kStmtNum1, pkb.GetReadStmt(kVarName1).front());
    Assert::IsTrue(pkb.GetReadStmt(kVarName2).empty());
    Assert::AreEqual(kStmtNum2, pkb.GetPrintStmt(kVarName2).front());
    Assert::IsTrue(pkb.GetPrintStmt(kVarName1).empty());
    Assert::IsTrue(pkb.IsReadVar(kVarName1));
    Assert::IsFalse(pkb.IsReadVar(kVarName2));
    Assert::IsTrue(pkb.IsPrintVar(kVarName2));
    Assert::IsFalse(pkb.IsPrintVar(kVarName1));
    VarIndexList read_vars = pkb.GetAllReadVar();
    Assert::IsTrue(read_vars.size() == 1);
    Assert::AreEqual(kVarIndex1, read_vars.front());
    VarIndexList print_vars = pkb.GetAllPrintVar();
    Assert::IsTrue(print_vars.size() == 1);
    Assert::AreEqual(kVarIndex2, print_vars.front());
    VarIndexPairList read_var_pairs = pkb.GetAllReadVarTwin();
    Assert::IsTrue(read_var_pairs.size() == 1);
    Assert::AreEqual(kVarIndex1, read_var_pairs.front().first);
    Assert::AreEqual(kVarIndex1, read_var_pairs.front().second);
    VarIndexPairList print_var_pairs = pkb.GetAllPrintVarTwin();
    Assert::IsTrue(print_var_pairs.size() == 1);
    Assert::AreEqual(kVarIndex2, print_var_pairs.front().first);
    Assert::AreEqual(kVarIndex2, print_var_pairs.front().second);
  }

  TEST_METHOD(TestGetAllConstValue) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    ConstValueList result = pkb.GetAllConstValue();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(1, result.front());
    Assert::AreEqual(2, result.back());
    ConstValuePairList result2 = pkb.GetAllConstValueTwin();
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(1, result2.front().first);
    Assert::AreEqual(1, result2.front().second);
    Assert::AreEqual(2, result2.back().first);
    Assert::AreEqual(2, result2.back().second);
    Assert::IsTrue(pkb.IsConstValue(1));
    Assert::IsTrue(pkb.IsConstValue(2));
    Assert::IsFalse(pkb.IsConstValue(3));
  }

  TEST_METHOD(TestGetStmtType) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum3, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum4, kProcIndex1, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum5, kProcIndex1, kVarName1));
    pkb.InsertCallStmt(
        &CallStmtData(kStmtNum6, kProcIndex1, kProcName1, kProcName2));
    Assert::IsTrue(PqlDeclarationEntity::kAssign == pkb.GetStmtType(kStmtNum1));
    Assert::IsTrue(PqlDeclarationEntity::kWhile == pkb.GetStmtType(kStmtNum2));
    Assert::IsTrue(PqlDeclarationEntity::kIf == pkb.GetStmtType(kStmtNum3));
    Assert::IsTrue(PqlDeclarationEntity::kRead == pkb.GetStmtType(kStmtNum4));
    Assert::IsTrue(PqlDeclarationEntity::kPrint == pkb.GetStmtType(kStmtNum5));
    Assert::IsTrue(PqlDeclarationEntity::kCall == pkb.GetStmtType(kStmtNum6));
  }

  TEST_METHOD(TestGetProcOfStmt) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertProcName(kProcName2);
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex2, kVarNameSet1, kConstValueSet1));
    Assert::AreEqual(kProcName1, pkb.GetProcOfStmt(kStmtNum1));
    Assert::AreEqual(kProcName2, pkb.GetProcOfStmt(kStmtNum2));
  }

  TEST_METHOD(TestCallGraph) {
    pkb.InsertEdgeInCallGraph(kProcName1, kProcName2);
    Assert::IsTrue(pkb.GetCallGraph()->GetSize() == 2);

    ProcIndexList actual_sorted_calls = pkb.GetToposortedCalls();
    // expect 2 first, then 1
    ProcIndexList expected_calls = ProcIndexList{kProcIndex2, kProcIndex1};
    Assert::IsTrue(actual_sorted_calls == expected_calls);
  }

  TEST_METHOD(TestGetAllAssignStmt) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex2, kVarName2,
                                         kVarNameSet2, kConstValueSet1,
                                         kTokenList));
    StmtNumList result = pkb.GetAllAssignStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
    Assert::IsTrue(pkb.IsAssignStmt(kStmtNum1));
    Assert::IsTrue(pkb.IsAssignStmt(kStmtNum2));
    Assert::IsFalse(pkb.IsAssignStmt(kStmtNum3));
  }

  TEST_METHOD(TestGetAllWhileStmt) {
    PKB pkb;
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum1, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex2, kVarNameSet1, kConstValueSet1));
    StmtNumList result = pkb.GetAllWhileStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
    Assert::IsTrue(pkb.IsWhileStmt(kStmtNum1));
    Assert::IsTrue(pkb.IsWhileStmt(kStmtNum2));
    Assert::IsFalse(pkb.IsWhileStmt(kStmtNum3));
  }

  TEST_METHOD(TestGetAllIfStmt) {
    PKB pkb;
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum1, kProcIndex1, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum2, kProcIndex2, kVarNameSet1, kConstValueSet1));
    StmtNumList result = pkb.GetAllIfStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
    Assert::IsTrue(pkb.IsIfStmt(kStmtNum1));
    Assert::IsTrue(pkb.IsIfStmt(kStmtNum2));
    Assert::IsFalse(pkb.IsIfStmt(kStmtNum3));
  }

  TEST_METHOD(TestGetAllReadStmt) {
    PKB pkb;
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum1, kProcIndex1, kVarName1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum2, kProcIndex1, kVarName2));
    StmtNumList result = pkb.GetAllReadStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
    Assert::IsTrue(pkb.IsReadStmt(kStmtNum1));
    Assert::IsTrue(pkb.IsReadStmt(kStmtNum2));
    Assert::IsFalse(pkb.IsReadStmt(kStmtNum3));
  }

  TEST_METHOD(TestGetAllPrintStmt) {
    PKB pkb;
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum1, kProcIndex1, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum2, kProcIndex2, kVarName2));
    StmtNumList result = pkb.GetAllPrintStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
    Assert::IsTrue(pkb.IsPrintStmt(kStmtNum1));
    Assert::IsTrue(pkb.IsPrintStmt(kStmtNum2));
    Assert::IsFalse(pkb.IsPrintStmt(kStmtNum3));
  }

  TEST_METHOD(TestGetAllCallStmt) {
    PKB pkb;
    pkb.InsertCallStmt(&CallStmtData(kStmtNum1, kProcIndex1, "one", "two"));
    pkb.InsertCallStmt(&CallStmtData(kStmtNum2, kProcIndex2, "one", "three"));
    StmtNumList result = pkb.GetAllCallStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
    Assert::IsTrue(pkb.IsCallStmt(kStmtNum1));
    Assert::IsTrue(pkb.IsCallStmt(kStmtNum2));
    Assert::IsFalse(pkb.IsCallStmt(kStmtNum3));
  }

  TEST_METHOD(TestGetAllStmt) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex2, kVarNameSet1, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum3, kProcIndex2, kVarNameSet1, kConstValueSet1));
    pkb.InsertReadStmt(&ReadStmtData(kStmtNum4, kProcIndex2, kVarName1));
    pkb.InsertPrintStmt(&PrintStmtData(kStmtNum5, kProcIndex2, kVarName2));
    StmtNumList result = pkb.GetAllStmt();
    Assert::IsTrue(result.size() == 5);
    Assert::AreEqual(kStmtNum1, result.front());
    StmtNumList::iterator iter = result.begin();
    iter++;
    Assert::AreEqual(kStmtNum2, *iter);
    iter++;
    Assert::AreEqual(kStmtNum3, *iter);
    iter++;
    Assert::AreEqual(kStmtNum4, *iter);
    iter++;
    Assert::AreEqual(kStmtNum5, *iter);
    Assert::IsTrue(pkb.IsStmtNum(kStmtNum1));
    Assert::IsTrue(pkb.IsStmtNum(kStmtNum2));
    Assert::IsTrue(pkb.IsStmtNum(kStmtNum3));
    Assert::IsTrue(pkb.IsStmtNum(kStmtNum4));
    Assert::IsTrue(pkb.IsStmtNum(kStmtNum5));
    Assert::IsFalse(pkb.IsStmtNum(100));
  }

  TEST_METHOD(TestIsFollows) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    Assert::IsTrue(pkb.IsFollows(kStmtNum1, kStmtNum2));
    Assert::IsTrue(pkb.IsFollows(kStmtNum2, kStmtNum3));
    Assert::IsFalse(pkb.IsFollows(kStmtNum1, kStmtNum3));
  }

  TEST_METHOD(TestIsFollowsT) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    Assert::IsTrue(pkb.IsFollowsT(kStmtNum1, kStmtNum2));
    Assert::IsTrue(pkb.IsFollowsT(kStmtNum1, kStmtNum3));
    Assert::IsTrue(pkb.IsFollowsT(kStmtNum2, kStmtNum3));
  }

  TEST_METHOD(TestGetFollows) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    StmtNumList result = pkb.GetFollows(kStmtNum1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
    result = pkb.GetFollows(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum3, result.front());
    result = pkb.GetFollows(kStmtNum3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetFollowsT) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    StmtNumList result = pkb.GetFollowsT(kStmtNum1);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum3, result.back());
    result = pkb.GetFollows(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum3, result.front());
    result = pkb.GetFollows(kStmtNum3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetAllFollows) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    StmtNumList result = pkb.GetAllFollows();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum3, result.back());
  }

  TEST_METHOD(TestGetFollowedBy) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    StmtNumList result = pkb.GetFollowedBy(kStmtNum1);
    Assert::IsTrue(result.empty());
    result = pkb.GetFollowedBy(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetFollowedBy(kStmtNum3);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
  }

  TEST_METHOD(TestGetFollowedByT) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    StmtNumList result = pkb.GetFollowedByT(kStmtNum1);
    Assert::IsTrue(result.empty());
    result = pkb.GetFollowedByT(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetFollowedByT(kStmtNum3);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum1, result.back());
  }

  TEST_METHOD(TestGetAllFollowedBy) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    StmtNumList result = pkb.GetAllFollowedBy();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllFollowsPair) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    StmtNumPairList result = pkb.GetAllFollowsPair();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kStmtNum2, result.front().second);
    Assert::AreEqual(kStmtNum2, result.back().first);
    Assert::AreEqual(kStmtNum3, result.back().second);
  }

  TEST_METHOD(TestGetAllFollowsTPair) {
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    pkb.InsertFollows(kStmtNum2, kStmtNum3);
    pkb.InsertFollows(kStmtNum1, kStmtNum3);
    StmtNumPairList result = pkb.GetAllFollowsTPair();
    Assert::IsTrue(result.size() == 3);
    Assert::AreEqual(kStmtNum1, result[0].first);
    Assert::AreEqual(kStmtNum2, result[0].second);
    Assert::AreEqual(kStmtNum1, result[1].first);
    Assert::AreEqual(kStmtNum3, result[1].second);
    Assert::AreEqual(kStmtNum2, result[2].first);
    Assert::AreEqual(kStmtNum3, result[2].second);
  }

  TEST_METHOD(TestHasFollows) {
    Assert::IsFalse(pkb.HasFollowsRelationship());
    pkb.InsertFollows(kStmtNum1, kStmtNum2);
    Assert::IsTrue(pkb.HasFollowsRelationship());
  }

  TEST_METHOD(TestIsParent) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    Assert::IsTrue(pkb.IsParent(kStmtNum1, kStmtNum2));
    Assert::IsTrue(pkb.IsParent(kStmtNum2, kStmtNum3));
    Assert::IsFalse(pkb.IsParent(kStmtNum1, kStmtNum3));
    Assert::IsFalse(pkb.IsParent(kStmtNum2, kStmtNum1));
  }

  TEST_METHOD(TestIsParentT) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    Assert::IsTrue(pkb.IsParentT(kStmtNum1, kStmtNum2));
    Assert::IsTrue(pkb.IsParentT(kStmtNum2, kStmtNum3));
    Assert::IsTrue(pkb.IsParentT(kStmtNum1, kStmtNum3));
    Assert::IsFalse(pkb.IsParentT(kStmtNum2, kStmtNum1));
  }

  TEST_METHOD(TestGetParent) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumList result = pkb.GetParent(kStmtNum1);
    Assert::IsTrue(result.empty());
    result = pkb.GetParent(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetParent(kStmtNum3);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
  }

  TEST_METHOD(TestGetParentT) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumList result = pkb.GetParentT(kStmtNum1);
    Assert::IsTrue(result.empty());
    result = pkb.GetParentT(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetParentT(kStmtNum3);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum1, result.back());
  }

  TEST_METHOD(TestGetAllParent) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumList result = pkb.GetAllParent();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetChild) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumList result = pkb.GetChild(kStmtNum1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
    result = pkb.GetChild(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum3, result.front());
    result = pkb.GetChild(kStmtNum3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetChildT) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumList result = pkb.GetChildT(kStmtNum1);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum3, result.back());
    result = pkb.GetChildT(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum3, result.front());
    result = pkb.GetChildT(kStmtNum3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetAllChild) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumList result = pkb.GetAllChild();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum2, result.front());
    Assert::AreEqual(kStmtNum3, result.back());
  }

  TEST_METHOD(TestGetAllParentPair) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumPairList result = pkb.GetAllParentPair();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kStmtNum2, result.front().second);
    Assert::AreEqual(kStmtNum2, result.back().first);
    Assert::AreEqual(kStmtNum3, result.back().second);
  }

  TEST_METHOD(TestGetAllParentTPair) {
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    pkb.InsertParent(kStmtNum2, kStmtNum3);
    pkb.InsertParentT(kStmtNum1, kStmtNum3);
    StmtNumPairList result = pkb.GetAllParentTPair();
    Assert::IsTrue(result.size() == 3);
    Assert::AreEqual(kStmtNum1, result[0].first);
    Assert::AreEqual(kStmtNum2, result[0].second);
    Assert::AreEqual(kStmtNum2, result[1].first);
    Assert::AreEqual(kStmtNum3, result[1].second);
    Assert::AreEqual(kStmtNum1, result[2].first);
    Assert::AreEqual(kStmtNum3, result[2].second);
  }

  TEST_METHOD(TestHasParentRelationship) {
    Assert::IsFalse(pkb.HasParentRelationship());
    pkb.InsertParent(kStmtNum1, kStmtNum2);
    Assert::IsTrue(pkb.HasParentRelationship());
  }

  TEST_METHOD(TestIsModifiedByS) {
    pkb.InsertModifiesS(kStmtNum1, kVarName1);
    pkb.InsertModifiesS(kStmtNum2, kVarName2);
    Assert::IsTrue(pkb.IsModifiedByS(kStmtNum1, kVarName1));
    Assert::IsTrue(pkb.IsModifiedByS(kStmtNum2, kVarName2));
    Assert::IsFalse(pkb.IsModifiedByS(kStmtNum1, kVarName2));
  }

  TEST_METHOD(TestIsModifiedByP) {
    pkb.InsertModifiesP(kProcName1, kVarName1);
    pkb.InsertModifiesP(kProcName2, kVarName2);
    Assert::IsTrue(pkb.IsModifiedByP(kProcName1, kVarName1));
    Assert::IsTrue(pkb.IsModifiedByP(kProcName2, kVarName2));
    Assert::IsFalse(pkb.IsModifiedByP(kProcName1, kVarName2));
  }

  TEST_METHOD(TestGetModifiedVarS) {
    pkb.InsertModifiesS(kStmtNum1, kVarName1);
    pkb.InsertModifiesS(kStmtNum2, kVarName2);
    VarIndexList result = pkb.GetModifiedVarS(kStmtNum1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex1, result.front());
    result = pkb.GetModifiedVarS(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex2, result.front());
    result = pkb.GetModifiedVarS(kStmtNum3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetModifiedVarP) {
    pkb.InsertModifiesP(kProcName1, kVarName1);
    pkb.InsertModifiesP(kProcName2, kVarName2);
    VarIndexList result = pkb.GetModifiedVarP(kProcName1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex1, result.front());
    result = pkb.GetModifiedVarP(kProcName2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex2, result.front());
  }

  TEST_METHOD(TestGetModifyingS) {
    pkb.InsertModifiesS(kStmtNum1, kVarName1);
    pkb.InsertModifiesS(kStmtNum2, kVarName2);
    StmtNumList result = pkb.GetModifyingS(kVarName1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetModifyingS(kVarName2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
    result = pkb.GetModifyingS(kVarName3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetModifyingP) {
    pkb.InsertModifiesP(kProcName1, kVarName1);
    pkb.InsertModifiesP(kProcName2, kVarName2);
    ProcIndexList result = pkb.GetModifyingP(kVarName1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex1, result.front());
    result = pkb.GetModifyingP(kVarName2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex2, result.front());
  }

  TEST_METHOD(TestGetAllModifyingS) {
    pkb.InsertModifiesS(kStmtNum1, kVarName1);
    pkb.InsertModifiesS(kStmtNum2, kVarName2);
    StmtNumList result = pkb.GetAllModifyingS();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllModifyingP) {
    pkb.InsertModifiesP(kProcName1, kVarName1);
    pkb.InsertModifiesP(kProcName2, kVarName2);
    ProcIndexList result = pkb.GetAllModifyingP();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex1, result.front());
    Assert::AreEqual(kProcIndex2, result.back());
  }

  TEST_METHOD(TestGetAllModifiesPairS) {
    pkb.InsertModifiesS(kStmtNum1, kVarName1);
    pkb.InsertModifiesS(kStmtNum2, kVarName2);
    StmtVarPairList result = pkb.GetAllModifiesPairS();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kVarIndex1, result.front().second);
    Assert::AreEqual(kStmtNum2, result.back().first);
    Assert::AreEqual(kVarIndex2, result.back().second);
  }

  TEST_METHOD(TestGetAllModifiesPairP) {
    pkb.InsertModifiesP(kProcName1, kVarName1);
    pkb.InsertModifiesP(kProcName2, kVarName2);
    ProcVarPairList result = pkb.GetAllModifiesPairP();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex1, result.front().first);
    Assert::AreEqual(kVarIndex1, result.front().second);
    Assert::AreEqual(kProcIndex2, result.back().first);
    Assert::AreEqual(kVarIndex2, result.back().second);
  }

  TEST_METHOD(TestIsUsedByS) {
    pkb.InsertUsesS(kStmtNum1, kVarName1);
    pkb.InsertUsesS(kStmtNum2, kVarName2);
    Assert::IsTrue(pkb.IsUsedByS(kStmtNum1, kVarName1));
    Assert::IsTrue(pkb.IsUsedByS(kStmtNum2, kVarName2));
    Assert::IsFalse(pkb.IsUsedByS(kStmtNum1, kVarName2));
  }

  TEST_METHOD(TestIsUsedByP) {
    pkb.InsertUsesP(kProcName1, kVarName1);
    pkb.InsertUsesP(kProcName2, kVarName2);
    Assert::IsTrue(pkb.IsUsedByP(kProcName1, kVarName1));
    Assert::IsTrue(pkb.IsUsedByP(kProcName2, kVarName2));
    Assert::IsFalse(pkb.IsUsedByP(kProcName1, kVarName2));
  }

  TEST_METHOD(TestGetUsedVarS) {
    pkb.InsertUsesS(kStmtNum1, kVarName1);
    pkb.InsertUsesS(kStmtNum2, kVarName2);
    VarIndexList result = pkb.GetUsedVarS(kStmtNum1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex1, result.front());
    result = pkb.GetUsedVarS(kStmtNum2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex2, result.front());
    result = pkb.GetUsedVarS(kStmtNum3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetUsedVarP) {
    pkb.InsertUsesP(kProcName1, kVarName1);
    pkb.InsertUsesP(kProcName2, kVarName2);
    VarIndexList result = pkb.GetUsedVarP(kProcName1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex1, result.front());
    result = pkb.GetUsedVarP(kProcName2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kVarIndex2, result.front());
  }

  TEST_METHOD(TestGetUsingS) {
    pkb.InsertUsesS(kStmtNum1, kVarName1);
    pkb.InsertUsesS(kStmtNum2, kVarName2);
    StmtNumList result = pkb.GetUsingStmt(kVarName1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetUsingStmt(kVarName2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
    result = pkb.GetUsingStmt(kVarName3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetUsingP) {
    pkb.InsertUsesP(kProcName1, kVarName1);
    pkb.InsertUsesP(kProcName2, kVarName2);
    ProcIndexList result = pkb.GetUsingProc(kVarName1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex1, result.front());
    result = pkb.GetUsingProc(kVarName2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex2, result.front());
  }

  TEST_METHOD(TestGetAllUsingS) {
    pkb.InsertUsesS(kStmtNum1, kVarName1);
    pkb.InsertUsesS(kStmtNum2, kVarName2);
    StmtNumList result = pkb.GetAllUsingStmt();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllUsingP) {
    pkb.InsertUsesP(kProcName1, kVarName1);
    pkb.InsertUsesP(kProcName2, kVarName2);
    ProcIndexList result = pkb.GetAllUsingProc();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex1, result.front());
    Assert::AreEqual(kProcIndex2, result.back());
  }

  TEST_METHOD(TestGetAllUsesPairS) {
    pkb.InsertUsesS(kStmtNum1, kVarName1);
    pkb.InsertUsesS(kStmtNum2, kVarName2);
    StmtVarPairList result = pkb.GetAllUsesPairS();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kVarIndex1, result.front().second);
    Assert::AreEqual(kStmtNum2, result.back().first);
    Assert::AreEqual(kVarIndex2, result.back().second);
  }

  TEST_METHOD(TestGetAllUsesPairP) {
    pkb.InsertUsesP(kProcName1, kVarName1);
    pkb.InsertUsesP(kProcName2, kVarName2);
    ProcVarPairList result = pkb.GetAllUsesPairP();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex1, result.front().first);
    Assert::AreEqual(kVarIndex1, result.front().second);
    Assert::AreEqual(kProcIndex2, result.back().first);
    Assert::AreEqual(kVarIndex2, result.back().second);
  }

  TEST_METHOD(TestGetAssignWithPattern) {
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtNumList result1 = pkb.GetAssignWithPattern(kVarName1, {kToken32});
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum3, result1.back());
    StmtNumList result2 = pkb.GetAssignWithPattern(kVarName2, kTokenList);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum2, result2.front());
    StmtNumList result3 = pkb.GetAssignWithPattern("", {kTokenA});
    Assert::IsTrue(result3.size() == 3);
    StmtNumList::iterator iter1 = result3.begin();
    Assert::AreEqual(kStmtNum1, *iter1);
    iter1++;
    Assert::AreEqual(kStmtNum2, *iter1);
    iter1++;
    Assert::AreEqual(kStmtNum3, *iter1);
    StmtNumList result4 = pkb.GetAssignWithPattern("", kTokenList);
    Assert::IsTrue(result4.size() == 3);
    StmtNumList::iterator iter2 = result4.begin();
    Assert::AreEqual(kStmtNum1, *iter2);
    iter2++;
    Assert::AreEqual(kStmtNum2, *iter2);
    iter2++;
    Assert::AreEqual(kStmtNum3, *iter2);
  }

  TEST_METHOD(TestGetAssignWithExactPattern) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtNumList result1 = pkb.GetAssignWithExactPattern(kVarName1, kTokenList1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum1, result1.front());
    StmtNumList result2 = pkb.GetAssignWithExactPattern(kVarName1, {kTokenA});
    Assert::IsTrue(result2.empty());
    StmtNumList result3 = pkb.GetAssignWithExactPattern("", kTokenList1);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(kStmtNum1, result3.front());
    Assert::AreEqual(kStmtNum2, result3.back());
    StmtNumList result4 = pkb.GetAssignWithExactPattern("", kTokenList2);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum3, result4.front());
    StmtNumList result5 = pkb.GetAssignWithExactPattern(kVarName1, kTokenList);
    Assert::IsTrue(result5.empty());
  }

  TEST_METHOD(TestGetAllAssignPatternPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtVarPairList result1 = pkb.GetAllAssignPatternPair({kTokenB});
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum3, result1.front().first);
    Assert::AreEqual(kVarIndex1, result1.front().second);
    StmtVarPairList result2 = pkb.GetAllAssignPatternPair({kTokenA});
    Assert::IsTrue(result2.size() == 3);
    StmtVarPairList::iterator iter = result2.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarIndex1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarIndex2, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum3, (*iter).first);
    Assert::AreEqual(kVarIndex1, (*iter).second);
    StmtVarPairList result3 = pkb.GetAllAssignPatternPair(kTokenList2);
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum3, result3.front().first);
    Assert::AreEqual(kVarIndex1, result3.front().second);
    StmtVarPairList result4 = pkb.GetAllAssignPatternPair(kTokenList);
    Assert::IsTrue(result4.size() == 3);
    StmtVarPairList::iterator iter1 = result4.begin();
    Assert::AreEqual(kStmtNum1, (*iter1).first);
    Assert::AreEqual(kVarIndex1, (*iter1).second);
    iter1++;
    Assert::AreEqual(kStmtNum2, (*iter1).first);
    Assert::AreEqual(kVarIndex2, (*iter1).second);
    iter1++;
    Assert::AreEqual(kStmtNum3, (*iter1).first);
    Assert::AreEqual(kVarIndex1, (*iter1).second);
    StmtVarPairList result5 = pkb.GetAllAssignPatternPair({kTokenPlus});
    Assert::IsTrue(result5.empty());
  }

  TEST_METHOD(TestGetAllAssignExactPatternPair) {
    PKB pkb;
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList1));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum3, kProcIndex2, kVarName1,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList2));
    StmtVarPairList result1 = pkb.GetAllAssignExactPatternPair(kTokenList2);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum3, result1.front().first);
    Assert::AreEqual(kVarIndex1, result1.front().second);
    StmtVarPairList result2 = pkb.GetAllAssignExactPatternPair(kTokenList1);
    Assert::IsTrue(result2.size() == 2);
    StmtVarPairList::iterator iter = result2.begin();
    Assert::AreEqual(kStmtNum1, (*iter).first);
    Assert::AreEqual(kVarIndex1, (*iter).second);
    iter++;
    Assert::AreEqual(kStmtNum2, (*iter).first);
    Assert::AreEqual(kVarIndex2, (*iter).second);
    StmtVarPairList result3 = pkb.GetAllAssignExactPatternPair({kToken32});
    Assert::IsTrue(result3.empty());
    StmtVarPairList result4 = pkb.GetAllAssignExactPatternPair(kTokenList);
    Assert::IsTrue(result4.empty());
  }

  TEST_METHOD(TestGetWhileWithPattern) {
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum1, kProcIndex1, {"a", "b"}, kConstValueSet1));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex1, {"a", "c"}, kConstValueSet1));
    StmtNumList result = pkb.GetWhileWithPattern("b");
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetWhileWithPattern("c");
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
    result = pkb.GetWhileWithPattern("a");
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllWhilePatternPair) {
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum1, kProcIndex1, {"a", "b"}, kConstValueSet1));
    pkb.InsertWhileStmt(
        &WhileStmtData(kStmtNum2, kProcIndex1, {"a", "c"}, kConstValueSet1));
    StmtVarPairList result = pkb.GetAllWhilePatternPair();
    Assert::IsTrue(result.size() == 4);
    Assert::AreEqual(kStmtNum1, result[0].first);
    Assert::AreEqual(kVarIndex1, result[0].second);
    Assert::AreEqual(kStmtNum2, result[1].first);
    Assert::AreEqual(kVarIndex1, result[1].second);
    Assert::AreEqual(kStmtNum1, result[2].first);
    Assert::AreEqual(kVarIndex2, result[2].second);
    Assert::AreEqual(kStmtNum2, result[3].first);
    Assert::AreEqual(kVarIndex3, result[3].second);
  }

  TEST_METHOD(TestGetIfWithPattern) {
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum1, kProcIndex1, {"a", "b"}, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum2, kProcIndex1, {"a", "c"}, kConstValueSet1));
    StmtNumList result = pkb.GetIfWithPattern("b");
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetIfWithPattern("c");
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
    result = pkb.GetIfWithPattern("a");
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front());
    Assert::AreEqual(kStmtNum2, result.back());
  }

  TEST_METHOD(TestGetAllIfPatternPair) {
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum1, kProcIndex1, {"a", "b"}, kConstValueSet1));
    pkb.InsertIfStmt(
        &IfStmtData(kStmtNum2, kProcIndex1, {"a", "c"}, kConstValueSet1));
    StmtVarPairList result = pkb.GetAllIfPatternPair();
    Assert::IsTrue(result.size() == 4);
    Assert::AreEqual(kStmtNum1, result[0].first);
    Assert::AreEqual(kVarIndex1, result[0].second);
    Assert::AreEqual(kStmtNum2, result[1].first);
    Assert::AreEqual(kVarIndex1, result[1].second);
    Assert::AreEqual(kStmtNum1, result[2].first);
    Assert::AreEqual(kVarIndex2, result[2].second);
    Assert::AreEqual(kStmtNum2, result[3].first);
    Assert::AreEqual(kVarIndex3, result[3].second);
  }

  TEST_METHOD(TestGetCallingStmts) {
    pkb.InsertCalls(kStmtNum1, kProcName1);
    pkb.InsertCalls(kStmtNum2, kProcName2);
    StmtNumList result = pkb.GetCallingStmts(kProcIndex1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum1, result.front());
    result = pkb.GetCallingStmts(kProcIndex2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kStmtNum2, result.front());
  }

  TEST_METHOD(TestGetAllCallingStmtPairs) {
    pkb.InsertCalls(kStmtNum1, kProcName1);
    pkb.InsertCalls(kStmtNum2, kProcName2);
    StmtNumProcPairList result = pkb.GetAllCallingStmtPairs();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kProcIndex1, result.front().second);
    Assert::AreEqual(kStmtNum2, result.back().first);
    Assert::AreEqual(kProcIndex2, result.back().second);
  }

  TEST_METHOD(TestGetCallee) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexList result = pkb.GetCallee(kProcIndex1);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex2, result.front());
    result = pkb.GetCallee(kProcIndex2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex3, result.front());
    result = pkb.GetCallee(kProcIndex3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetCalleeT) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexList result = pkb.GetCalleeT(kProcIndex1);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex2, result.front());
    Assert::AreEqual(kProcIndex3, result.back());
    result = pkb.GetCalleeT(kProcIndex2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex3, result.front());
    result = pkb.GetCalleeT(kProcIndex3);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetCaller) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexList result = pkb.GetCaller(kProcIndex2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex1, result.front());
    result = pkb.GetCaller(kProcIndex3);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex2, result.front());
    result = pkb.GetCaller(kProcIndex1);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetCallerT) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexList result = pkb.GetCallerT(kProcIndex3);
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex2, result.front());
    Assert::AreEqual(kProcIndex1, result.back());
    result = pkb.GetCallerT(kProcIndex2);
    Assert::IsTrue(result.size() == 1);
    Assert::AreEqual(kProcIndex1, result.front());
    result = pkb.GetCallerT(kProcIndex1);
    Assert::IsTrue(result.empty());
  }

  TEST_METHOD(TestGetAllCaller) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexList result = pkb.GetAllCaller();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex1, result.front());
    Assert::AreEqual(kProcIndex2, result.back());
  }

  TEST_METHOD(TestGetAllCallerName) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcNameList result = pkb.GetAllCallerName();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcName1, result.front());
    Assert::AreEqual(kProcName2, result.back());
  }

  TEST_METHOD(TestGetAllCallee) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexList result = pkb.GetAllCallee();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex2, result.front());
    Assert::AreEqual(kProcIndex3, result.back());
  }

  TEST_METHOD(TestGetAllCalleeName) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcNameList result = pkb.GetAllCalleeName();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcName2, result.front());
    Assert::AreEqual(kProcName3, result.back());
  }

  TEST_METHOD(TestGetAllCallPairs) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexPairList result = pkb.GetAllCallPairs();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kProcIndex1, result.front().first);
    Assert::AreEqual(kProcIndex2, result.front().second);
    Assert::AreEqual(kProcIndex2, result.back().first);
    Assert::AreEqual(kProcIndex3, result.back().second);
  }

  TEST_METHOD(TestGetAllCallTPairs) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    ProcIndexPairList result = pkb.GetAllCallTPairs();
    Assert::IsTrue(result.size() == 3);
    Assert::AreEqual(kProcIndex1, result[0].first);
    Assert::AreEqual(kProcIndex2, result[0].second);
    Assert::AreEqual(kProcIndex1, result[1].first);
    Assert::AreEqual(kProcIndex3, result[1].second);
    Assert::AreEqual(kProcIndex2, result[2].first);
    Assert::AreEqual(kProcIndex3, result[2].second);
  }

  TEST_METHOD(TestIsCall) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    Assert::IsTrue(pkb.IsCall(kProcName1, kProcName2));
    Assert::IsTrue(pkb.IsCall(kProcName2, kProcName3));
    Assert::IsFalse(pkb.IsCall(kProcName1, kProcName3));
  }

  TEST_METHOD(TestIsCallT) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    Assert::IsTrue(pkb.IsCallT(kProcName1, kProcName2));
    Assert::IsTrue(pkb.IsCallT(kProcName2, kProcName3));
    Assert::IsTrue(pkb.IsCallT(kProcName1, kProcName3));
  }

  TEST_METHOD(TestIsCalledProc) {
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    Assert::IsTrue(pkb.IsCalledProc(kProcName2));
    Assert::IsTrue(pkb.IsCalledProc(kProcName3));
    Assert::IsFalse(pkb.IsCalledProc(kProcName1));
  }

  TEST_METHOD(TestHasCallsRelationship) {
    Assert::IsFalse(pkb.HasCallsRelationship());
    pkb.InsertDirectCallRelationship(kProcName1, kProcName2);
    pkb.InsertDirectCallRelationship(kProcName2, kProcName3);
    pkb.InsertIndirectCallRelationship(kProcName1, kProcName3);
    Assert::IsTrue(pkb.HasCallsRelationship());
  }

  TEST_METHOD(TestGetCalledProc) {
    pkb.InsertCalls(kStmtNum1, kProcName1);
    pkb.InsertCalls(kStmtNum2, kProcName2);
    Assert::AreEqual(kProcIndex1, pkb.GetCalledProcedure(kStmtNum1));
    Assert::AreEqual(kProcIndex2, pkb.GetCalledProcedure(kStmtNum2));
  }

  TEST_METHOD(TestIsNextRelationship) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(pkb.IsNext(kStmtNum1, kStmtNum2));
    Assert::IsTrue(pkb.IsNext(kStmtNum2, kStmtNum3));
    Assert::IsTrue(pkb.IsNext(kStmtNum2, kStmtNum4));
    Assert::IsFalse(pkb.IsNext(kStmtNum1, kStmtNum3));
    Assert::IsFalse(pkb.IsNext(kStmtNum1, kStmtNum4));
  }

  TEST_METHOD(TestIsNext) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(pkb.IsNext(kStmtNum2));
    Assert::IsTrue(pkb.IsNext(kStmtNum3));
    Assert::IsTrue(pkb.IsNext(kStmtNum4));
    Assert::IsFalse(pkb.IsNext(kStmtNum1));
  }

  TEST_METHOD(TestIsPrevious) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(pkb.IsPrevious(kStmtNum1));
    Assert::IsTrue(pkb.IsPrevious(kStmtNum2));
    Assert::IsFalse(pkb.IsPrevious(kStmtNum3));
    Assert::IsFalse(pkb.IsPrevious(kStmtNum4));
  }

  TEST_METHOD(TestGetNext) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = pkb.GetNext(kStmtNum1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(kStmtNum2, result1.front());
    StmtNumList result2 = pkb.GetNext(kStmtNum2);
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(kStmtNum3, result2.front());
    Assert::AreEqual(kStmtNum4, result2.back());
    StmtNumList result3 = pkb.GetNext(kStmtNum3);
    Assert::IsTrue(result3.empty());
    StmtNumList result4 = pkb.GetNext(kStmtNum4);
    Assert::IsTrue(result4.empty());
  }

  TEST_METHOD(TestGetPrevious) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = pkb.GetPrevious(kStmtNum1);
    Assert::IsTrue(result1.empty());
    StmtNumList result2 = pkb.GetPrevious(kStmtNum2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(kStmtNum1, result2.front());
    StmtNumList result3 = pkb.GetPrevious(kStmtNum3);
    Assert::IsTrue(result3.size() == 1);
    Assert::AreEqual(kStmtNum2, result3.front());
    StmtNumList result4 = pkb.GetPrevious(kStmtNum4);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(kStmtNum2, result4.front());
  }

  TEST_METHOD(TestGetAllNext) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = pkb.GetAllNext();
    Assert::IsTrue(result1.size() == 3);
    StmtNumList::iterator iter = result1.begin();
    Assert::AreEqual(kStmtNum2, *iter);
    iter++;
    Assert::AreEqual(kStmtNum3, *iter);
    iter++;
    Assert::AreEqual(kStmtNum4, *iter);
  }

  TEST_METHOD(TestGetAllPrevious) {
    pkb.InsertProcName(kProcName1);
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    StmtNumList result1 = pkb.GetAllPrevious();
    Assert::IsTrue(result1.size() == 2);
    Assert::AreEqual(kStmtNum1, result1.front());
    Assert::AreEqual(kStmtNum2, result1.back());
  }

  TEST_METHOD(TestGetAllNextPairs) {
    pkb.InsertProcName(kProcName1);
    pkb.SetAllNextPairs({{kStmtNum1, kStmtNum2}, {kStmtNum2, kStmtNum3}});
    StmtNumPairList result = pkb.GetAllNextPairs();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(kStmtNum1, result.front().first);
    Assert::AreEqual(kStmtNum2, result.front().second);
    Assert::AreEqual(kStmtNum2, result.back().first);
    Assert::AreEqual(kStmtNum3, result.back().second);
  }

  TEST_METHOD(TestHasNextRelationship) {
    pkb.InsertProcName(kProcName1);
    Assert::IsFalse(pkb.HasNextRelationship());
    pkb.InsertNext(kProcName1, kStmtNum1, kStmtNum2);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum3);
    pkb.InsertNext(kProcName1, kStmtNum2, kStmtNum4);
    Assert::IsTrue(pkb.HasNextRelationship());
  }

  TEST_METHOD(TestIsDominates) {
    pkb.InsertDominates(1, {1, 2, 3, 4});
    pkb.InsertDominates(2, {2});
    pkb.InsertDominates(3, {3, 4});
    pkb.InsertDominates(4, {4});
    Assert::IsTrue(pkb.IsDominates(1, 1));
    Assert::IsTrue(pkb.IsDominates(1, 2));
    Assert::IsTrue(pkb.IsDominates(1, 3));
    Assert::IsTrue(pkb.IsDominates(1, 4));
    Assert::IsFalse(pkb.IsDominates(2, 1));
    Assert::IsTrue(pkb.IsDominates(2, 2));
    Assert::IsFalse(pkb.IsDominates(2, 3));
    Assert::IsFalse(pkb.IsDominates(2, 4));
    Assert::IsFalse(pkb.IsDominates(3, 1));
    Assert::IsFalse(pkb.IsDominates(3, 2));
    Assert::IsTrue(pkb.IsDominates(3, 3));
    Assert::IsTrue(pkb.IsDominates(3, 4));
    Assert::IsFalse(pkb.IsDominates(4, 1));
    Assert::IsFalse(pkb.IsDominates(4, 2));
    Assert::IsFalse(pkb.IsDominates(4, 3));
    Assert::IsTrue(pkb.IsDominates(4, 4));
  }

  TEST_METHOD(TestIsDominating) {
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertDominates(1, {1, 2});
    pkb.InsertDominates(2, {2});
    Assert::IsTrue(pkb.IsDominating(1));
    Assert::IsTrue(pkb.IsDominating(2));
  }

  TEST_METHOD(TestIsDominated) {
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertDominates(1, {1, 2});
    pkb.InsertDominates(2, {2});
    Assert::IsTrue(pkb.IsDominated(1));
    Assert::IsTrue(pkb.IsDominated(2));
  }

  TEST_METHOD(TestGetDominating) {
    pkb.InsertDominates(1, {1, 2, 3, 4});
    pkb.InsertDominates(2, {2});
    pkb.InsertDominates(3, {3, 4});
    pkb.InsertDominates(4, {4});
    StmtNumList result1 = pkb.GetDominating(1);
    Assert::IsTrue(result1.size() == 1);
    Assert::AreEqual(1, result1.front());
    StmtNumList result2 = pkb.GetDominating(2);
    Assert::IsTrue(result2.size() == 2);
    Assert::AreEqual(1, result2.front());
    Assert::AreEqual(2, result2.back());
    StmtNumList result3 = pkb.GetDominating(3);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(1, result3.front());
    Assert::AreEqual(3, result3.back());
    StmtNumList result4 = pkb.GetDominating(4);
    Assert::IsTrue(result4.size() == 3);
    StmtNumList::iterator iter = result4.begin();
    Assert::AreEqual(1, (*iter++));
    Assert::AreEqual(3, (*iter++));
    Assert::AreEqual(4, (*iter));
  }

  TEST_METHOD(TestGetDominated) {
    pkb.InsertDominates(1, {1, 2, 3, 4});
    pkb.InsertDominates(2, {2});
    pkb.InsertDominates(3, {3, 4});
    pkb.InsertDominates(4, {4});
    StmtNumList result1 = pkb.GetDominated(1);
    Assert::IsTrue(result1.size() == 4);
    StmtNumList::iterator iter = result1.begin();
    Assert::AreEqual(1, (*iter++));
    Assert::AreEqual(2, (*iter++));
    Assert::AreEqual(3, (*iter++));
    Assert::AreEqual(4, (*iter));
    StmtNumList result2 = pkb.GetDominated(2);
    Assert::IsTrue(result2.size() == 1);
    Assert::AreEqual(2, result2.front());
    StmtNumList result3 = pkb.GetDominated(3);
    Assert::IsTrue(result3.size() == 2);
    Assert::AreEqual(3, result3.front());
    Assert::AreEqual(4, result3.back());
    StmtNumList result4 = pkb.GetDominated(4);
    Assert::IsTrue(result4.size() == 1);
    Assert::AreEqual(4, result4.front());
  }

  TEST_METHOD(TestGetAllDominating) {
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertDominates(1, {1, 2});
    pkb.InsertDominates(2, {2});
    StmtNumList result = pkb.GetAllDominating();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(1, result.front());
    Assert::AreEqual(2, result.back());
  }

  TEST_METHOD(TestGetAllDominated) {
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum1, kProcIndex1, kVarName1,
                                         VarNameSet(), kConstValueSet1,
                                         kTokenList));
    pkb.InsertAssignStmt(&AssignStmtData(kStmtNum2, kProcIndex1, kVarName2,
                                         kVarNameSet1, kConstValueSet1,
                                         kTokenList));
    pkb.InsertDominates(1, {1, 2});
    pkb.InsertDominates(2, {2});
    StmtNumList result = pkb.GetAllDominated();
    Assert::IsTrue(result.size() == 2);
    Assert::AreEqual(1, result.front());
    Assert::AreEqual(2, result.back());
  }

  TEST_METHOD(TestGetAllDominatesPairs) {
    pkb.InsertDominates(1, {1, 2, 3, 4});
    pkb.InsertDominates(2, {2});
    pkb.InsertDominates(3, {3, 4});
    pkb.InsertDominates(4, {4});
    StmtNumPairList result = pkb.GetAllDominatesPairs();
    StmtNumPairList expected = StmtNumPairList(
        {make_pair(1, 1), make_pair(1, 2), make_pair(1, 3), make_pair(1, 4),
         make_pair(2, 2), make_pair(3, 3), make_pair(3, 4), make_pair(4, 4)});
    Assert::IsTrue(expected == result);
  }

  TEST_METHOD(TestHasDominatesRelationship) {
    Assert::IsFalse(pkb.HasDominatesRelationship());
    pkb.InsertDominates(1, {1});
    Assert::IsTrue(pkb.HasDominatesRelationship());
  }
};
}  // namespace PKBTests