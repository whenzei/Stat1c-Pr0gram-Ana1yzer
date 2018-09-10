#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_query.h"
#include "query_evaluator.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
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

  TEST_METHOD(TestGetResultFromQueryAssign) {
    // TODO: Your test code here
    QueryEvaluator qe;
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
    QueryEvaluator qe;
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
    QueryEvaluator qe;
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
    QueryEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertAssignStmt(1, 0, "x", list<string>());

    PqlQuery* q = new PqlQuery();
    q->SetVarName("s");
    q->AddDeclaration(keyword_stmt, "s");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "1";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestGetResultFromQueryIf) {
    // TODO: Your test code here
    QueryEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertIfStmt(2, 0, list<string>());

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

    pkb.PKB::InsertWhileStmt(3, 0, list<string>());

    PqlQuery* q = new PqlQuery();
    q->SetVarName("while");
    q->AddDeclaration(keyword_while, "while");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    list<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "3";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestGetResultFromQueryRead) {
    // TODO: Your test code here
    QueryEvaluator qe;
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
    QueryEvaluator qe;
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
}  // namespace UnitTesting