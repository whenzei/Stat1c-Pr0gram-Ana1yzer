#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_global.h"
#include "pql_query.h"

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestQueryEvaluator) {
 public:
  PqlDeclarationEntity keyword_assign = PqlDeclarationEntity::kAssign;
  PqlDeclarationEntity keyword_variable = PqlDeclarationEntity::kVariable;
  PqlDeclarationEntity keyword_procedure = PqlDeclarationEntity::kProcedure;
  PqlDeclarationEntity keyword_stmt = PqlDeclarationEntity::kStmt;
  PqlDeclarationEntity keyword_constant = PqlDeclarationEntity::kConstant;
  PqlDeclarationEntity keyword_read = PqlDeclarationEntity::kRead;
  PqlDeclarationEntity keyword_print = PqlDeclarationEntity::kPrint;
  PqlDeclarationEntity keyword_if = PqlDeclarationEntity::kIf;
  PqlDeclarationEntity keyword_while = PqlDeclarationEntity::kWhile;
  PqlDeclarationEntity keyword_progline = PqlDeclarationEntity::kProgline;

  TEST_METHOD(TestCheckWithParamTypeTwoSyn) {
    PqlEvaluator qe;
    PqlWith with_clause = PqlWith("a", PqlDeclarationEntity::kAssign, "b",
                                  PqlDeclarationEntity::kRead);
    WithParamType result;
    WithParamType expected_result = kWithTwoSynonym;

    result = qe.CheckWithParamType(with_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckWithParamTypeNoSyn) {
    PqlEvaluator qe;
    PqlWith with_clause = PqlWith("1", PqlDeclarationEntity::kInteger, "2",
                                  PqlDeclarationEntity::kInteger);
    WithParamType result;
    WithParamType expected_result = kWithNoSynonym;

    result = qe.CheckWithParamType(with_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckWithParamTypeOneSyn) {
    PqlEvaluator qe;
    PqlWith with_clause = PqlWith("a", PqlDeclarationEntity::kAssign, "2",
                                  PqlDeclarationEntity::kInteger);
    WithParamType result;
    WithParamType expected_result = kWithOneSynonymLeft;

    result = qe.CheckWithParamType(with_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }
  TEST_METHOD(TestCheckSuchthatParamTypeUnderscoreBoth) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "_", PqlDeclarationEntity::kUnderscore, "_",
        PqlDeclarationEntity::kUnderscore);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonymUnderscoreBoth;

    result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeSynonymUnderscoreLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "_", PqlDeclarationEntity::kUnderscore, "2",
        PqlDeclarationEntity::kInteger);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonymUnderscoreLeft;

    result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeNoSynonymUnderscoreRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kModifiesP, "One", PqlDeclarationEntity::kIdent, "_",
        PqlDeclarationEntity::kUnderscore);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonymUnderscoreRight;

    result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }
  TEST_METHOD(TestCheckSuchthatParamTypeNoSynonym) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "1", PqlDeclarationEntity::kInteger, "2",
        PqlDeclarationEntity::kInteger);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonym;

    result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeOneSynonymLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "w", PqlDeclarationEntity::kWhile, "3",
        PqlDeclarationEntity::kInteger);
    SuchthatParamType result;
    SuchthatParamType expected_result = kOneSynonymLeft;

    result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeOneSynonymRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "3", PqlDeclarationEntity::kInteger, "a",
        PqlDeclarationEntity::kAssign);
    SuchthatParamType result;
    SuchthatParamType expected_result = kOneSynonymRight;

    result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeTwoSynonym) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "w", PqlDeclarationEntity::kWhile, "a",
        PqlDeclarationEntity::kAssign);
    SuchthatParamType result;
    SuchthatParamType expected_result = kTwoSynonym;

    result = qe.CheckSuchthatParamType(such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }
};
}  // namespace PQLTests