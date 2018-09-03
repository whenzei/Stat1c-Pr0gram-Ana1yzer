#include "CppUnitTest.h"
#include "stdafx.h"
#include "query.h"

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
	    Query query(content);
	    Assert::IsTrue(query.GetDeclarations().empty());
	    Assert::IsTrue(query.GetVarName() == "");
    }

    TEST_METHOD(TestAssign) {
	    string content = "assign a;";
	    Query query(content);
	    Assert::IsTrue(query.GetDeclarations()[0].GetKeyword() == DeclarationKeyword::kAssign);
	    Assert::IsTrue(query.GetDeclarations()[0].GetVarName() == "a");
	    Assert::IsTrue(query.GetVarName() == "");
    }

    TEST_METHOD(TestAssignSelect) {
	    string content = "assign a; Select a;";
	    Query query(content);
	    Assert::IsTrue(query.GetDeclarations()[0].GetKeyword() == DeclarationKeyword::kAssign);
	    Assert::IsTrue(query.GetDeclarations()[0].GetVarName() == "a");
	    Assert::IsTrue(query.GetVarName() == "a");
    }
    };
}