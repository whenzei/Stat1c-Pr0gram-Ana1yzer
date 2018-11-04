#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_evaluate_with.h"
#include "pql_global.h"
#include "pql_query.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestEvaluateWith){

  public : TEST_METHOD(TestCheckWithParamTypeTwoSyn){
	  PqlEvaluateWith qe;
PqlWith with_clause = PqlWith("a", PqlDeclarationEntity::kAssign, "b",
                              PqlDeclarationEntity::kRead);
WithParamType result;
WithParamType expected_result = kWithTwoSynonym;

result = qe.CheckWithParamType(with_clause.GetParameters());

Assert::IsTrue(expected_result == result);
}  // namespace PQLTests

TEST_METHOD(TestCheckWithParamTypeNoSyn) {
  PqlEvaluateWith qe;
  PqlWith with_clause = PqlWith("1", PqlDeclarationEntity::kInteger, "2",
                                PqlDeclarationEntity::kInteger);
  WithParamType result;
  WithParamType expected_result = kWithNoSynonym;

  result = qe.CheckWithParamType(with_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}

TEST_METHOD(TestCheckWithParamTypeOneSyn) {
  PqlEvaluateWith qe;
  PqlWith with_clause = PqlWith("a", PqlDeclarationEntity::kAssign, "2",
                                PqlDeclarationEntity::kInteger);
  WithParamType result;
  WithParamType expected_result = kWithOneSynonymLeft;

  result = qe.CheckWithParamType(with_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}
}
;
}  // namespace PQLTests