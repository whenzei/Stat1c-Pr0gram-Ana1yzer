#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_projector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestPqlProjector) {
 public:
  ResultTableList kIntermediateResultTable = {{{1, 2, 3}, {3, 4, 5}},
                                              {{2, 4}, {6, 8}}};
  ResultTableColumnHeader kColumnHeader = {{"a1", make_pair(0, 0)},
                                           {"v", make_pair(0, 1)},
                                           {"s", make_pair(0, 2)},
                                           {"w", make_pair(1, 0)},
                                           {"a2", make_pair(1, 1)}};
  vector<Synonym> kSelections = {
      make_pair("a2", PqlDeclarationEntity::kAssign),
      make_pair("v", PqlDeclarationEntity::kVariable),
      make_pair("v", PqlDeclarationEntity::kVariable),
      make_pair("w", PqlDeclarationEntity::kWhile)};
  AssignStmtData kStmtData =
      AssignStmtData(1, 0, "x", {"x", "y", "z", "a", "b", "c"},
                     unordered_set<int>(), TokenList());

  TEST_METHOD(TestGetFinalResult) {
    PqlProjector pql_projector;
    PKB pkb;
    pkb.InsertAssignStmt(&kStmtData);
    FinalResult result = pql_projector.GetFinalResult(
        kIntermediateResultTable, kColumnHeader, kSelections, pkb);
    VarName v1 = pkb.GetVarName(2) + " ";
    VarName v2 = pkb.GetVarName(4) + " ";
    Assert::IsTrue(result ==
                   FinalResult{"4 " + v1 + v1 + "2", "4 " + v2 + v2 + "2",
                               "8 " + v1 + v1 + "6", "8 " + v2 + v2 + "6"});
  }
  // TODO: add more tests
};

}  // namespace PQLTests