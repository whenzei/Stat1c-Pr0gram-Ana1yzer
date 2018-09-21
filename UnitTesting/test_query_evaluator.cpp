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

  TEST_METHOD(TestCheckSuchthatParamTypeUnderscoreBoth) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "_", PqlDeclarationEntity::kUnderscore, "_",
        PqlDeclarationEntity::kUnderscore);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonymUnderscoreBoth;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeSynonymUnderscoreLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "_", PqlDeclarationEntity::kUnderscore, "2",
        PqlDeclarationEntity::kInteger);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonymUnderscoreLeft;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeNoSynonymUnderscoreRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kModifiesP, "One", PqlDeclarationEntity::kIdent, "_",
        PqlDeclarationEntity::kUnderscore);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonymUnderscoreRight;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }
  TEST_METHOD(TestCheckSuchthatParamTypeNoSynonym) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "1", PqlDeclarationEntity::kInteger, "2",
        PqlDeclarationEntity::kInteger);
    SuchthatParamType result;
    SuchthatParamType expected_result = kNoSynonym;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeOneSynonymLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "w", PqlDeclarationEntity::kWhile, "3",
        PqlDeclarationEntity::kInteger);
    SuchthatParamType result;
    SuchthatParamType expected_result = kOneSynonymLeft;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeOneSynonymSelectLeftUnderscoreRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "a", PqlDeclarationEntity::kAssign, "_",
        PqlDeclarationEntity::kUnderscore);
    SuchthatParamType result;
    SuchthatParamType expected_result = kOneSynonymSelectLeftUnderscoreRight;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeOneSynonymSelectLeft) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause = PqlSuchthat(
        PqlSuchthatType::kFollows, "a", PqlDeclarationEntity::kAssign, "2",
        PqlDeclarationEntity::kInteger);
    SuchthatParamType result;
    SuchthatParamType expected_result = kOneSynonymSelectLeft;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }

  TEST_METHOD(TestCheckSuchthatParamTypeTwoSynonymSelectRight) {
    PqlEvaluator qe;
    PqlSuchthat such_that_clause =
        PqlSuchthat(PqlSuchthatType::kFollows, "if", PqlDeclarationEntity::kIf,
                    "a", PqlDeclarationEntity::kAssign);
    SuchthatParamType result;
    SuchthatParamType expected_result = kTwoSynonymSelectRight;

    result = qe.CheckSuchthatParamType("a", such_that_clause.GetParameters());

    Assert::IsTrue(expected_result == result);
  }
  TEST_METHOD(TestSelectAllAssign) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertAssignStmt(&AssignStmtData(1, 0, "x", VarNameSet(), ConstValueSet(), TokenList()));

    PqlQuery* q = new PqlQuery();
    q->SetVarName("a");
    q->AddDeclaration(keyword_assign, "a");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "1";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestSelectAllProc) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertProcName("testprocedure");

    PqlQuery* q = new PqlQuery();
    q->SetVarName("p");
    q->AddDeclaration(keyword_assign, "a");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "testprocedure";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestSelectAllConst) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertConstValue(8);

    PqlQuery* q = new PqlQuery();
    q->SetVarName("c");
    q->AddDeclaration(keyword_constant, "c");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "8";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestSelectAllStmt) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertAssignStmt(&AssignStmtData(1, 0, "x", VarNameSet(), ConstValueSet(), TokenList()));

    PqlQuery* q = new PqlQuery();
    q->SetVarName("s");
    q->AddDeclaration(keyword_stmt, "s");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "1";

    Assert::AreEqual(expected_result, first_result);
  } 

  TEST_METHOD(TestSelectAllIf) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertIfStmt(&IfStmtData(2, 0, 1, 1, VarNameSet(), ConstValueSet()));

    PqlQuery* q = new PqlQuery();
    q->SetVarName("if");
    q->AddDeclaration(keyword_if, "if");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "2";

    Assert::AreEqual(expected_result, first_result);
  }

  TEST_METHOD(TestSelectAllWhile) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertWhileStmt(&WhileStmtData(3, 0, 1, VarNameSet(), ConstValueSet()));

    PqlQuery* q = new PqlQuery();
    q->SetVarName("while");
    q->AddDeclaration(keyword_while, "while");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "3";

    Assert::AreEqual(expected_result, first_result);
  } 
  TEST_METHOD(TestSelectAllRead) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertReadStmt(&ReadStmtData(5, 0, "x"));

    PqlQuery* q = new PqlQuery();
    q->SetVarName("read");
    q->AddDeclaration(keyword_read, "read");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "5";

    Assert::AreEqual(expected_result, first_result);
  }
  TEST_METHOD(TestSelectAllPrint) {
    // TODO: Your test code here
    PqlEvaluator qe;
    PKB pkb;

    pkb.PKB::InsertPrintStmt(&PrintStmtData(6, 0, "x"));

    PqlQuery* q = new PqlQuery();
    q->SetVarName("print");
    q->AddDeclaration(keyword_print, "print");
    q->AddDeclaration(keyword_variable, "v");
    q->AddDeclaration(keyword_procedure, "p");

    vector<string> resultlist = qe.GetResultFromQuery(q, pkb);

    string first_result = resultlist.front();

    string expected_result = "6";

    Assert::AreEqual(expected_result, first_result);
  }
};
}  // namespace PQLTests