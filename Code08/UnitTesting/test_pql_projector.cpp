#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_projector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestPqlProjector) {
 public:
  ResultTableList kIntermediateResultTable1 = {{{1, 2, 3}, {4, 5, 6}}};
  ResultTableList kIntermediateResultTable2 = {{{1, 2, 3}, {4, 5, 6}},
                                               {{11, 12}, {13, 14}, {15, 16}},
                                               {{21, 22, 23, 24}}};
  ResultTableColumnHeader kColumnHeader1 = {
      {"a", make_pair(0, 0)}, {"b", make_pair(0, 1)}, {"c", make_pair(0, 2)}};
  ResultTableColumnHeader kColumnHeader2 = {
      {"a", make_pair(0, 0)}, {"b", make_pair(0, 1)}, {"c", make_pair(0, 2)},
      {"d", make_pair(1, 0)}, {"e", make_pair(1, 1)}, {"f", make_pair(2, 0)},
      {"g", make_pair(2, 1)}, {"h", make_pair(2, 2)}, {"i", make_pair(2, 3)}};
  SynonymList kSelections1 = {make_pair("b", PqlDeclarationEntity::kAssign)};
  SynonymList kSelections2 = {make_pair("c", PqlDeclarationEntity::kAssign),
                              make_pair("a", PqlDeclarationEntity::kProgline),
                              make_pair("b", PqlDeclarationEntity::kConstant)};
  SynonymList kSelections3 = {make_pair("c", PqlDeclarationEntity::kAssign),
                              make_pair("a", PqlDeclarationEntity::kProgline),
                              make_pair("b", PqlDeclarationEntity::kConstant),
                              make_pair("a", PqlDeclarationEntity::kProgline)};
  SynonymList kSelections4 = {make_pair("c", PqlDeclarationEntity::kVariable),
                              make_pair("a", PqlDeclarationEntity::kProgline),
                              make_pair("b", PqlDeclarationEntity::kProcedure),
                              make_pair("a", PqlDeclarationEntity::kProgline)};
  SynonymList kSelections5 = {make_pair("c", PqlDeclarationEntity::kStmt),
                              make_pair("a", PqlDeclarationEntity::kProgline),
                              make_pair("b", PqlDeclarationEntity::kConstant),
                              make_pair("z", PqlDeclarationEntity::kAssign)};
  SynonymList kSelections6 = {make_pair("b", PqlDeclarationEntity::kAssign),
                              make_pair("g", PqlDeclarationEntity::kStmt),
                              make_pair("e", PqlDeclarationEntity::kIf),
                              make_pair("c", PqlDeclarationEntity::kWhile)};
  SynonymList kSelections7 = {make_pair("b", PqlDeclarationEntity::kAssign),
                              make_pair("g", PqlDeclarationEntity::kStmt),
                              make_pair("e", PqlDeclarationEntity::kIf),
                              make_pair("c", PqlDeclarationEntity::kWhile),
                              make_pair("b", PqlDeclarationEntity::kAssign)};
  SynonymList kSelections8 = {make_pair("b", PqlDeclarationEntity::kCallName),
                              make_pair("g", PqlDeclarationEntity::kStmt),
                              make_pair("e", PqlDeclarationEntity::kIf),
                              make_pair("c", PqlDeclarationEntity::kVariable),
                              make_pair("b", PqlDeclarationEntity::kCallName)};
  SynonymList kSelections9 = {make_pair("d", PqlDeclarationEntity::kStmt),
                              make_pair("z", PqlDeclarationEntity::kProcedure)};
  AssignStmtData kStmtData =
      AssignStmtData(100, 0, "i", {"x", "y", "z", "a", "b", "c"},
                     unordered_set<int>(), TokenList());
  ProcName kProcName0 = "zero";
  ProcName kProcName1 = "one";
  ProcName kProcName2 = "two";
  ProcName kProcName3 = "three";
  ProcName kProcName4 = "four";
  ProcName kProcName5 = "five";
  PKB pkb;

  TEST_METHOD(TestOneGroupSelectOne) {
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable1, kColumnHeader1, kSelections1, pkb);
    Assert::IsTrue(result == FinalResult{"2", "5"});
  }

  TEST_METHOD(TestOneGroupSelectMultiple) {
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable1, kColumnHeader1, kSelections2, pkb);
    Assert::IsTrue(result == FinalResult{"3 1 2", "6 4 5"});
  }

  TEST_METHOD(TestOneGroupSelectRepeat) {
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable1, kColumnHeader1, kSelections3, pkb);
    Assert::IsTrue(result == FinalResult{"3 1 2 1", "6 4 5 4"});
  }

  TEST_METHOD(TestOneGroupSelectVarNameProcName) {
    pkb.InsertAssignStmt(&kStmtData);
    pkb.InsertProcName(kProcName0);
    pkb.InsertProcName(kProcName1);
    pkb.InsertProcName(kProcName2);
    pkb.InsertProcName(kProcName3);
    pkb.InsertProcName(kProcName4);
    pkb.InsertProcName(kProcName5);
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable1, kColumnHeader1, kSelections4, pkb);
    VarName v3 = pkb.GetVarName(3);
    VarName v6 = pkb.GetVarName(6);
    Assert::IsTrue(result == FinalResult{v3 + " 1 two 1", v6 + " 4 five 4"});
  }

  TEST_METHOD(TestOneGroupSelectNotInClause) {
    pkb.InsertAssignStmt(&kStmtData);
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable1, kColumnHeader1, kSelections5, pkb);
    Assert::IsTrue(result.size() == 2);
    Assert::IsTrue(result == FinalResult{"3 1 2 100", "6 4 5 100"});
  }

  TEST_METHOD(TestMultipleGroupSelectOne) {
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable2, kColumnHeader2, kSelections1, pkb);
    Assert::IsTrue(result == FinalResult{"2", "5"});
  }

  TEST_METHOD(TestMultipleGroupSelectMultiple) {
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable2, kColumnHeader2, kSelections6, pkb);
    Assert::IsTrue(result == FinalResult{"2 22 12 3", "2 22 14 3", "2 22 16 3",
                                         "5 22 12 6", "5 22 14 6",
                                         "5 22 16 6"});
  }

  TEST_METHOD(TestMultipleGroupSelectRepeat) {
    PqlProjector pql_projector;
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable2, kColumnHeader2, kSelections7, pkb);
    Assert::IsTrue(result == FinalResult{"2 22 12 3 2", "2 22 14 3 2",
                                         "2 22 16 3 2", "5 22 12 6 5",
                                         "5 22 14 6 5", "5 22 16 6 5"});
  }

  TEST_METHOD(TestMultipleGroupSelectVarNameProcName) {
    PqlProjector pql_projector;
    pkb.InsertAssignStmt(&kStmtData);
    pkb.InsertProcName(kProcName0);
    pkb.InsertProcName(kProcName1);
    pkb.InsertProcName(kProcName2);
    pkb.InsertProcName(kProcName3);
    pkb.InsertProcName(kProcName4);
    pkb.InsertProcName(kProcName5);
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable2, kColumnHeader2, kSelections8, pkb);
    ProcName p2 = pkb.GetProcName(2);
    ProcName p5 = pkb.GetProcName(5);
    VarName v3 = pkb.GetVarName(3);
    VarName v6 = pkb.GetVarName(6);
    Assert::IsTrue(result == FinalResult{p2 + " 22 12 " + v3 + " " + p2,
                                         p2 + " 22 14 " + v3 + " " + p2,
                                         p2 + " 22 16 " + v3 + " " + p2,
                                         p5 + " 22 12 " + v6 + " " + p5,
                                         p5 + " 22 14 " + v6 + " " + p5,
                                         p5 + " 22 16 " + v6 + " " + p5});
  }

  TEST_METHOD(TestMultipleGroupSelectNotInClause) {
    PqlProjector pql_projector;
    pkb.InsertProcName(kProcName0);
    pkb.InsertProcName(kProcName1);
    pkb.InsertProcName(kProcName2);
    pkb.InsertProcName(kProcName3);
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable2, kColumnHeader2, kSelections9, pkb);
    Assert::IsTrue(result == FinalResult{"11 " + kProcName0, "11 " + kProcName1,
                                         "11 " + kProcName2, "11 " + kProcName3,
                                         "13 " + kProcName0, "13 " + kProcName1,
                                         "13 " + kProcName2, "13 " + kProcName3,
                                         "15 " + kProcName0, "15 " + kProcName1,
                                         "15 " + kProcName2,
                                         "15 " + kProcName3});
  }

  TEST_METHOD(TestBooleanResult) {
    PqlProjector pql_projector1;
    FinalResult result1 = pql_projector1.GetFinalResult(true);
    Assert::IsTrue(result1 == FinalResult{"true"});
    PqlProjector pql_projector2;
    FinalResult result2 = pql_projector2.GetFinalResult(false);
    Assert::IsTrue(result2 == FinalResult{"false"});
  }
};

}  // namespace PQLTests