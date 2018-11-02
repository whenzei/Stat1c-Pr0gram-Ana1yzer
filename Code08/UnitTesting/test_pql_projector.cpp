#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_projector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestPqlProjector) {
 public:
  QueryResultPairList kResultPairList1 = {make_pair(1, 2), make_pair(3, 4)};
  QueryResultPairList kResultPairList2 = {make_pair(5, 6), make_pair(7, 8)};
  vector<Synonym> kSelections1 = {
      make_pair("a2", PqlDeclarationEntity::kAssign),
      make_pair("v", PqlDeclarationEntity::kVariable),
      make_pair("v", PqlDeclarationEntity::kVariable),
      make_pair("w", PqlDeclarationEntity::kWhile)};
  vector<Synonym> kSelections2 = {
      make_pair("a", PqlDeclarationEntity::kAssign),
      make_pair("c", PqlDeclarationEntity::kAssign)};
  AssignStmtData kStmtData =
      AssignStmtData(1, 0, "x", {"x", "y", "z", "a", "b", "c"},
                     unordered_set<int>(), TokenList());

  TEST_METHOD(TestOneIntermediateResult) {
    PqlResult pql_result;
    PqlProjector pql_projector;
    PKB pkb;
    pql_result.InitTable(kResultPairList1, "a", "b");
    pql_result.MergeResults(kResultPairList2, kNoConflict, -1, -1, "c", "d");
    FinalResult result = pql_projector.GetFinalResult(PqlResultList{pql_result},
                                                      kSelections2, pkb);
    Assert::IsTrue(
        result ==
        FinalResult{"1 5", "1 7", "3 5", "3 7"});
  }

  TEST_METHOD(TestTwoIntermediateResults) {
    PqlResult pql_result1;
    PqlResult pql_result2;
    PqlProjector pql_projector;
    PKB pkb;
    pql_result1.InitTable(kResultPairList1, "a", "b");
    pql_result2.InitTable(kResultPairList2, "c", "d");
    FinalResult result = pql_projector.GetFinalResult(PqlResultList{pql_result1, pql_result2},
                                                      kSelections2, pkb);
    Assert::IsTrue(result == FinalResult{"1 5", "1 7", "3 5", "3 7"});
  }
  // TODO: add more tests
};

}  // namespace PQLTests