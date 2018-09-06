#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_parser.h"
#include "pql_query.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace UnitTesting {
/*
This test the parsing logic for PQL.
*/
TEST_CLASS(TestQueryParser) {
public:
  TEST_METHOD(TestEmptyQuery) {
	  string content = "";
	  PqlQuery* query = new PqlQuery();
    PqlParser::Parse(content, query);
	  Assert::IsTrue(query->GetDeclarations().empty());
	  Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestAssign) {
	  string content = "assign a;";
    PqlQuery* query = new PqlQuery();
    PqlParser::Parse(content, query);
	  Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
	  Assert::IsTrue(query->GetVarName() == "");
  }

  TEST_METHOD(TestAssignSelect) {
	  string content = "assign a; Select a;";
    PqlQuery* query = new PqlQuery();
    PqlParser::Parse(content, query);
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
	  Assert::IsTrue(query->GetVarName() == "a");
  }
  };
}