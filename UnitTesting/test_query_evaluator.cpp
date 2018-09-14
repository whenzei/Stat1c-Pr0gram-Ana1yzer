#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_enum.h"
#include "pql_evaluator.h"
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

  TEST_METHOD(
      TestCheckArrangementOfSynonymInSuchthatParamNoSynonymUnderscoreBoth) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kFollows, "_", kUnderscore, "_", kUnderscore);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result =
        kNoSynonymUnderscoreBoth;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(
      TestCheckArrangementOfSynonymInSuchthatParamNoSynonymUnderscoreLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kFollows, "_", kUnderscore, "2", kInteger);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result =
        kNoSynonymUnderscoreLeft;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(
      TestCheckArrangementOfSynonymInSuchthatParamNoSynonymUnderscoreRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kModifiesP, "One", kIdent, "_", kUnderscore);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result =
        kNoSynonymUnderscoreRight;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }
  TEST_METHOD(TestCheckArrangementOfSynonymInSuchthatParamNoSynonym) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kFollows, "1", kInteger, "2", kInteger);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result = kNoSynonym;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckArrangementOfSynonymInSuchthatParamOneSynonymLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kFollows, "w", kWhile, "3", kInteger);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result = kOneSynonymLeft;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(
      TestCheckArrangementOfSynonymInSuchthatParamOneSynonymSelectLeftUnderscoreRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kFollows, "a", kAssign, "_", kUnderscore);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result =
        kOneSynonymSelectLeftUnderscoreRight;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(
      TestCheckArrangementOfSynonymInSuchthatParamOneSynonymSelectLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kFollows, "a", kAssign, "2", kInteger);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result =
        kOneSynonymSelectLeft;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(
      TestCheckArrangementOfSynonymInSuchthatParamTwoSynonymSelectRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(kFollows, "if", kIf, "a", kAssign);
    PqlArrangementOfSynonymInSuchthatParam result;
    PqlArrangementOfSynonymInSuchthatParam expected_result =
        kTwoSynonymSelectRight;

    result = qe.CheckArrangementOfSynonymInSuchthatParam(
        "a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }
  TEST_METHOD(TestGetResultFromQueryAssign) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertAssignStmt(1, 0, "x", unordered_set<string>());

    PqlQuery* q = new PqlQuery();
    q->SetVarName("a");
    q->AddDeclaration(keyword_assign, "a");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "1";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestGetResultFromQueryProc) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertProcName("testprocedure");

    PqlQuery* q = new PqlQuery();
    q->SetVarName("p");
    q->AddDeclaration(keyword_assign, "a");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "testprocedure";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestGetResultFromQueryConst) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertConstValue(8);

    PqlQuery* q = new PqlQuery();
    q->SetVarName("c");
    q->AddDeclaration(keyword_constant, "c");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "8";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestGetResultFromQueryStmt) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertAssignStmt(1, 0, "x", unordered_set<string>());

    PqlQuery* q = new PqlQuery();
    q->SetVarName("s");
    q->AddDeclaration(keyword_stmt, "s");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "1";

    Assert::AreEqual(expected_result, first_result);
  } /*
  TEST_METHOD(TestGetResultFromQueryIf) {
    // TODO: Your test code here
    QueryEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertIfStmt(2, 0, unordered_set<string>());

    PqlQuery* q = new PqlQuery();
    q->SetVarName("if");
    q->AddDeclaration(keyword_if, "if");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "2";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestGetResultFromQueryWhile) {
    // TODO: Your test code here
    QueryEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertWhileStmt(3, 0, unordered_set<string>());

    PqlQuery* q = new PqlQuery();
    q->SetVarName("while");
    q->AddDeclaration(keyword_while, "while");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "3";

    Assert::AreEqual(expected_result, first_result);
  } */
  TEST_METHOD(TestGetResultFromQueryRead) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertReadStmt(5, 0, "x");

    PqlQuery* q = new PqlQuery();
    q->SetVarName("read");
    q->AddDeclaration(keyword_read, "read");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "5";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestGetResultFromQueryPrint) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertPrintStmt(6, 0, "x");

    PqlQuery* q = new PqlQuery();
    q->SetVarName("print");
    q->AddDeclaration(keyword_print, "print");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "6";

    Assert::AreEqual(expected_result, first_result);
  }
};
}  // namespace PQLTests