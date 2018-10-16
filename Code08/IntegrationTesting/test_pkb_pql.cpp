#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_parser.h"
#include "pql_query.h"
#include "pkb.h"
#include "pql_evaluator.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBPQLTests {
TEST_CLASS(TestPKBPQL){
public:
  TEST_METHOD(TestSelectAllAssign){
    PKB pkb;
	PqlEvaluator qe;
	AssignStmtData stmt1(1, 0, "x", unordered_set<string>(), unordered_set<int>(), TokenList());
	AssignStmtData stmt2(8, 0, "i", unordered_set<string>(), unordered_set<int>(), TokenList());

	pkb.InsertAssignStmt(&stmt1);
	pkb.InsertAssignStmt(&stmt2);

	string user_query = "assign a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

	FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

	string output = resultlist.front();

	string expected_output = "1";

	Assert::AreEqual(expected_output, output);
  }

  TEST_METHOD(TestSelectAllProc) {
    PqlEvaluator qe;
    PKB pkb;

    pkb.InsertProcName("testprocedure");

    string user_query = "procedure p; Select p";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

    FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

    string first_result = resultlist.front();

    string expected_result = "testprocedure";

    Assert::AreEqual(expected_result, first_result);
  }

  TEST_METHOD(TestSelectAllConst) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.InsertAssignStmt(
        &AssignStmtData(1, 0, "x", VarNameSet(), {8}, TokenList()));

    string user_query = "constant c; Select c";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

    FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

    string first_result = resultlist.front();

    string expected_result = "8";

    Assert::AreEqual(expected_result, first_result);
  }

  TEST_METHOD(TestSelectAllStmt) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.InsertAssignStmt(
        &AssignStmtData(1, 0, "x", VarNameSet(), ConstValueSet(), TokenList()));

    string user_query = "stmt s; Select s";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

    FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

    string first_result = resultlist.front();

    string expected_result = "1";

    Assert::AreEqual(expected_result, first_result);
  }

  TEST_METHOD(TestSelectAllIf) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.InsertIfStmt(&IfStmtData(2, 0, VarNameSet(), ConstValueSet()));

    string user_query = "if ifs; Select ifs";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

    FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

    string first_result = resultlist.front();

    string expected_result = "2";

    Assert::AreEqual(expected_result, first_result);
  }

  TEST_METHOD(TestSelectAllWhile) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.InsertWhileStmt(&WhileStmtData(3, 0, VarNameSet(), ConstValueSet()));

    string user_query = "while w; Select w";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

    FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

    string first_result = resultlist.front();

    string expected_result = "3";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestSelectAllRead) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.InsertReadStmt(&ReadStmtData(5, 0, "x"));

    string user_query = "read r; Select r";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

    FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

    string first_result = resultlist.front();

    string expected_result = "5";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestSelectAllPrint) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.InsertPrintStmt(&PrintStmtData(6, 0, "x"));

    string user_query = "print p; Select p";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(user_query, query);
    parser.Parse();

    FinalResult resultlist = qe.GetResultFromQuery(query, pkb);

    string first_result = resultlist.front();

    string expected_result = "6";

    Assert::AreEqual(expected_result, first_result);
  }
  };
}