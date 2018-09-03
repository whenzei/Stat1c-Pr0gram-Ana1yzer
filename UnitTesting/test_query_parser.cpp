#include "stdafx.h"
#include "CppUnitTest.h"
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
	  PqlQuery query(content);
	  Assert::IsTrue(query.GetDeclarations().empty());
	  Assert::IsTrue(query.GetVarName() == "");
  }

  TEST_METHOD(TestAssign) {
	  string content = "assign a;";
    PqlQuery query(content);
	  Assert::IsTrue(query.GetDeclarations()[0].GetKeyword() == PqlDeclarationKeyword::kAssign);
	  Assert::IsTrue(query.GetDeclarations()[0].GetVarName() == "a");
	  Assert::IsTrue(query.GetVarName() == "");
  }

  TEST_METHOD(TestAssignSelect) {
	  string content = "assign a; Select a;";
    PqlQuery query(content);
	  Assert::IsTrue(query.GetDeclarations()[0].GetKeyword() == PqlDeclarationKeyword::kAssign);
	  Assert::IsTrue(query.GetDeclarations()[0].GetVarName() == "a");
	  Assert::IsTrue(query.GetVarName() == "a");
  }
  };
}