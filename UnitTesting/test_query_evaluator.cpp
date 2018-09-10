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
  TEST_METHOD(TestGetResultFromQueryAssign) {
    // TODO: Your test code here
    QueryEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertAssignStmt(1, 0, "x", list<string>());

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

    pkb.PKB::InsertProc("testprocedure");

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
};
}  // namespace UnitTesting