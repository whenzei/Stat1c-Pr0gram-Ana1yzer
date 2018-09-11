#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_parser.h"
#include "pql_query.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace PQLTests {
/*
This test the parsing logic for PQL.
*/
TEST_CLASS(TestQueryParser) {
public:
  TEST_METHOD(TestEmptyQuery) {
	  string content = "";
	  PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
	  Assert::IsTrue(query->GetDeclarations().empty());
	  Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestAssign) {
	  string content = "assign a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
	  Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
	  Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestAssignSelect) {
	  string content = "assign a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
	  Assert::IsTrue(query->GetVarName() == "a");
  }

  TEST_METHOD(TestMultiAssignSelect) {
    string content = "assign a, b, c; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["c"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetVarName() == "a");
  }

  TEST_METHOD(TestStmt) {
    string content = "stmt a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kStmt);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestRead) {
    string content = "read a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kRead);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestPrint) {
    string content = "print a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kPrint);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestCall) {
    string content = "call a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kCall);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestWhile) {
    string content = "while a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestIf) {
    string content = "if a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kIf);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestVariable) {
    string content = "variable a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestConstant) {
    string content = "constant a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kConstant);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestProgline) {
    string content = "prog_line a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProgline);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestProcedure) {
    string content = "procedure a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestInvalidEntity) {
    string content = "qwerty a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(parser.GetErrorMessage() == "Invalid declaration entity.");
  }
  };
}