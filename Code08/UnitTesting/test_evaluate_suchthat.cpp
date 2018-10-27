#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_evaluate_suchthat.h"
#include "pql_global.h"
#include "pql_query.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestEvaluateSuchthat){
  public : TEST_METHOD(TestCheckSuchthatParamTypeUnderscoreBoth){
      PqlEvaluateSuchthat qe;
PqlSuchthat such_that_clause = PqlSuchthat(
    PqlSuchthatType::kFollows, "_", PqlDeclarationEntity::kUnderscore, "_",
    PqlDeclarationEntity::kUnderscore);
SuchthatParamType result;
SuchthatParamType expected_result = kNoSynonymUnderscoreBoth;

result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

Assert::IsTrue(expected_result == result);
}  // namespace PQLTests

TEST_METHOD(TestCheckSuchthatParamTypeSynonymUnderscoreLeft) {
  PqlEvaluateSuchthat qe;
  PqlSuchthat such_that_clause = PqlSuchthat(
      PqlSuchthatType::kFollows, "_", PqlDeclarationEntity::kUnderscore, "2",
      PqlDeclarationEntity::kInteger);
  SuchthatParamType result;
  SuchthatParamType expected_result = kNoSynonymUnderscoreLeft;

  result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}

TEST_METHOD(TestCheckSuchthatParamTypeNoSynonymUnderscoreRight) {
  PqlEvaluateSuchthat qe;
  PqlSuchthat such_that_clause = PqlSuchthat(PqlSuchthatType::kModifiesP, "One",
                                             PqlDeclarationEntity::kIdent, "_",
                                             PqlDeclarationEntity::kUnderscore);
  SuchthatParamType result;
  SuchthatParamType expected_result = kNoSynonymUnderscoreRight;

  result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}
TEST_METHOD(TestCheckSuchthatParamTypeNoSynonym) {
  PqlEvaluateSuchthat qe;
  PqlSuchthat such_that_clause = PqlSuchthat(
      PqlSuchthatType::kFollows, "1", PqlDeclarationEntity::kInteger, "2",
      PqlDeclarationEntity::kInteger);
  SuchthatParamType result;
  SuchthatParamType expected_result = kNoSynonym;

  result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}

TEST_METHOD(TestCheckSuchthatParamTypeOneSynonymLeft) {
  PqlEvaluateSuchthat qe;
  PqlSuchthat such_that_clause =
      PqlSuchthat(PqlSuchthatType::kFollows, "w", PqlDeclarationEntity::kWhile,
                  "3", PqlDeclarationEntity::kInteger);
  SuchthatParamType result;
  SuchthatParamType expected_result = kOneSynonymLeft;

  result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}

TEST_METHOD(TestCheckSuchthatParamTypeOneSynonymRight) {
  PqlEvaluateSuchthat qe;
  PqlSuchthat such_that_clause = PqlSuchthat(
      PqlSuchthatType::kFollows, "3", PqlDeclarationEntity::kInteger, "a",
      PqlDeclarationEntity::kAssign);
  SuchthatParamType result;
  SuchthatParamType expected_result = kOneSynonymRight;

  result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}

TEST_METHOD(TestCheckSuchthatParamTypeTwoSynonym) {
  PqlEvaluateSuchthat qe;
  PqlSuchthat such_that_clause =
      PqlSuchthat(PqlSuchthatType::kFollows, "w", PqlDeclarationEntity::kWhile,
                  "a", PqlDeclarationEntity::kAssign);
  SuchthatParamType result;
  SuchthatParamType expected_result = kTwoSynonym;

  result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

  Assert::IsTrue(expected_result == result);
}
}
;
}  // namespace PQLTests