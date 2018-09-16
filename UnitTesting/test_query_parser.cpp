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

  TEST_METHOD(TestSuchthatFollows) {
    string content = "assign a; while b; Select a such that Follows(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kFollows);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kWhile);
  };

  TEST_METHOD(TestInvalidSuchthatFollows) {
    string content = "variable a; while b; Select a such that Follows(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(parser.GetErrorMessage() == "First parameter in such that clause is invalid for the type.");
  };

  TEST_METHOD(TestSuchthatFollowsT) {
    string content = "assign a; while b; Select a such that Follows*(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kFollowsT);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kWhile);
  };

  TEST_METHOD(TestInvalidSuchthatFollowsT) {
    string content = "variable a; while b; Select a such that Follows*(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(parser.GetErrorMessage() == "First parameter in such that clause is invalid for the type.");
  };

  TEST_METHOD(TestSuchthatParent) {
    string content = "assign a; while b; Select a such that Parent(b,a)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kParent);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kAssign);
  };

  TEST_METHOD(TestInvalidSuchthatParent) {
    string content = "variable a; while b; Select a such that Parent(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(parser.GetErrorMessage() == "First parameter in such that clause is invalid for the type.");
  };

  TEST_METHOD(TestSuchthatParentT) {
    string content = "assign a; while b; Select a such that Parent*(b,a)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kParentT);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kAssign);
  };

  TEST_METHOD(TestInvalidSuchthatParentT) {
    string content = "variable a; while b; Select a such that Parent*(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(parser.GetErrorMessage() == "First parameter in such that clause is invalid for the type.");
  };

  TEST_METHOD(TestSuchthatUsesS) {
    string content = "assign a; variable b; Select a such that Uses(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kUsesS);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kVariable);
  };

  TEST_METHOD(TestSuchthatUsesP) {
    string content = "procedure a; variable b; Select a such that Uses(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kUsesP);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kVariable);
  };

  TEST_METHOD(TestInvalidSuchthatUses) {
    string content = "variable a; Select a such that Uses(_,a)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(parser.GetErrorMessage() == "First parameter in such that clause is invalid for the type.");
  };

  TEST_METHOD(TestSuchthatModifiesS) {
    string content = "assign a; variable b; Select a such that Modifies(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kModifiesS);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kVariable);
  };

  TEST_METHOD(TestSuchthatModifiesP) {
    string content = "procedure a; variable b; Select a such that Modifies(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetVarName() == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kModifiesP);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kVariable);
  };

  TEST_METHOD(TestInvalidSuchthatModifies) {
    string content = "variable a; Select a such that Modifies(_,a)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(parser.GetErrorMessage() == "First parameter in such that clause is invalid for the type.");
  };

  TEST_METHOD(TestSuchthatAnd) {
    string content = "procedure a; variable b; Select a such that Modifies(a,b) and Uses(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetVarName() == "a");

    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kModifiesP);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kVariable);

    Assert::IsTrue(query->GetSuchThats()[1].GetType() == PqlSuchthatType::kUsesP);
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().first.second == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().second.second == PqlDeclarationEntity::kVariable);
  };

  TEST_METHOD(TestSuchthatSuchthat) {
    string content = "procedure a; variable b; Select a such that Modifies(a,b) such that Uses(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    parser.Parse();
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetVarName() == "a");

    Assert::IsTrue(query->GetSuchThats()[0].GetType() == PqlSuchthatType::kModifiesP);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().first.second == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[0].GetParameters().second.second == PqlDeclarationEntity::kVariable);

    Assert::IsTrue(query->GetSuchThats()[1].GetType() == PqlSuchthatType::kUsesP);
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().first.first == "a");
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().first.second == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().second.first == "b");
    Assert::IsTrue(query->GetSuchThats()[1].GetParameters().second.second == PqlDeclarationEntity::kVariable);
  };
  };
}