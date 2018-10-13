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
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestNoSelect) {
    string content = "assign a;";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestUndeclaredSelect) {
    string content = "assign a; Select c";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestSelectOnly) {
    string content = "Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestSelectBoolean) {
    string content = "assign a; Select BOOLEAN";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections().empty());
  }

  TEST_METHOD(TestSelectTuple) {
    string content = "assign a,b,c; Select <a,b,c>";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["c"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[1].first == "b");
    Assert::IsTrue(query->GetSelections()[1].second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[2].first == "c");
    Assert::IsTrue(query->GetSelections()[2].second == PqlDeclarationEntity::kAssign);
  }

  TEST_METHOD(TestSelectTupleWithClause) {
    string content = "assign a,b; Select <a,b> such that Follows(a,_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[1].first == "b");
    Assert::IsTrue(query->GetSelections()[1].second == PqlDeclarationEntity::kAssign);

    PqlSuchthat* suchthat = (PqlSuchthat*)query->GetClauses()[0];
    Assert::IsTrue(suchthat->GetType() == PqlSuchthatType::kFollows);
    Assert::IsTrue(suchthat->GetParameters().first.first == "a");
    Assert::IsTrue(suchthat->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(suchthat->GetParameters().second.first == "_");
    Assert::IsTrue(suchthat->GetParameters().second.second == PqlDeclarationEntity::kUnderscore);
  }

  TEST_METHOD(TestSelectWithAttr) {
    string content = "assign a; Select a.stmt#";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
  }

  TEST_METHOD(TestInvalidSynonymFormat) {
    string content = "assign 123; Select 123";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestAssign) {
	  string content = "assign a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
  }

  TEST_METHOD(TestMultiAssign) {
    string content = "assign a, b, c; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["c"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
  }

  TEST_METHOD(TestStmt) {
    string content = "stmt a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kStmt);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kStmt);
  }

  TEST_METHOD(TestRead) {
    string content = "read a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kRead);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kRead);
  }

  TEST_METHOD(TestPrint) {
    string content = "print a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kPrint);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kPrint);
  }

  TEST_METHOD(TestCall) {
    string content = "call a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kCall);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kCall);
  }

  TEST_METHOD(TestWhile) {
    string content = "while a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kWhile);
  }

  TEST_METHOD(TestIf) {
    string content = "if a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kIf);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kIf);
  }

  TEST_METHOD(TestVariable) {
    string content = "variable a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kVariable);
  }

  TEST_METHOD(TestConstant) {
    string content = "constant a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kConstant);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kConstant);
  }

  TEST_METHOD(TestProgline) {
    string content = "prog_line a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProgline);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kProgline);
  }

  TEST_METHOD(TestProcedure) {
    string content = "procedure a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kProcedure);
  }

  TEST_METHOD(TestInvalidEntity) {
    string content = "qwerty a; Select a";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestInvalidSuchthatType) {
    string content = "assign a; Select a such that Qwerty(a,a)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestSuchthatFollows) {
    string content = "assign a; while b; Select a such that Follows(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
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
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestSuchthatFollowsT) {
    string content = "assign a; while b; Select a such that Follows*(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
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
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestSuchthatParent) {
    string content = "assign a; while b; Select a such that Parent(b,a)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
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
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestSuchthatParentT) {
    string content = "assign a; while b; Select a such that Parent*(b,a)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
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
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestSuchthatUsesS) {
    string content = "assign a; variable b; Select a such that Uses(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
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
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kProcedure);
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
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestSuchthatModifiesS) {
    string content = "assign a; variable b; Select a such that Modifies(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);
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
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kProcedure);
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
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestSuchthatAnd) {
    string content = "procedure a; variable b; Select a such that Modifies(a,b) and Uses(a,b)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kProcedure);

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
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kProcedure);
    Assert::IsTrue(query->GetDeclarations()["b"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kProcedure);

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

  TEST_METHOD(TestInvalidPatternType) {
    string content = "assign a; prog_line p; Select a pattern p(a,_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }

  TEST_METHOD(TestPatternAssign) {
    string content = "variable v; assign a; Select v pattern a(v,_\"a*2\"_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["v"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "v");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kVariable);

    Assert::IsTrue(query->GetPatterns()[0].GetType().first == "a");
    Assert::IsTrue(query->GetPatterns()[0].GetType().second == PqlPatternType::kAssign);
    Assert::IsTrue(query->GetPatterns()[0].GetFirstParameter().first == "v");
    Assert::IsTrue(query->GetPatterns()[0].GetFirstParameter().second == PqlDeclarationEntity::kVariable);
    Expression expression = query->GetPatterns()[0].GetAssignExpression();
    Assert::IsTrue(expression.first == PqlPatternExpressionType::kUnderscoreExpressionUnderscore);
  };

  TEST_METHOD(TestInvalidPatternAssign) {
    string content = "variable v; assign a; Select v pattern a(v, _\"2*a\")";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  };
  TEST_METHOD(TestPatternWhile) {
    string content = "variable v; while w; Select v pattern w(v,_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["v"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetDeclarations()["w"] == PqlDeclarationEntity::kWhile);
    Assert::IsTrue(query->GetSelections()[0].first == "v");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kVariable);

    Assert::IsTrue(query->GetPatterns()[0].GetType().first == "w");
    Assert::IsTrue(query->GetPatterns()[0].GetType().second == PqlPatternType::kWhile);
    Assert::IsTrue(query->GetPatterns()[0].GetFirstParameter().first == "v");
    Assert::IsTrue(query->GetPatterns()[0].GetFirstParameter().second == PqlDeclarationEntity::kVariable);
  };

  TEST_METHOD(TestInvalidPatternWhile) {
    string content = "assign a; while w; Select a pattern w(a,_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  }; 

  TEST_METHOD(TestPatternIf) {
    string content = "variable v; if if; Select v pattern if(v,_,_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["v"] == PqlDeclarationEntity::kVariable);
    Assert::IsTrue(query->GetDeclarations()["if"] == PqlDeclarationEntity::kIf);
    Assert::IsTrue(query->GetSelections()[0].first == "v");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kVariable);

    Assert::IsTrue(query->GetPatterns()[0].GetType().first == "if");
    Assert::IsTrue(query->GetPatterns()[0].GetType().second == PqlPatternType::kIf);
    Assert::IsTrue(query->GetPatterns()[0].GetFirstParameter().first == "v");
    Assert::IsTrue(query->GetPatterns()[0].GetFirstParameter().second == PqlDeclarationEntity::kVariable);
  };

  TEST_METHOD(TestInvalidPatternIf) {
    string content = "assign a; if if; Select a pattern if(a,_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestWith) {
    string content = "assign a; Select a with a.stmt# = 2";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);

    PqlWith* with = (PqlWith*)query->GetClauses()[0];
    Assert::IsTrue(with->GetParameters().first.first == "a");
    Assert::IsTrue(with->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(with->GetParameters().second.first == "2");
    Assert::IsTrue(with->GetParameters().second.second == PqlDeclarationEntity::kInteger);
  };

  TEST_METHOD(TestWithProgline) {
    string content = "prog_line p; Select p with p = 2";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["p"] == PqlDeclarationEntity::kProgline);
    Assert::IsTrue(query->GetSelections()[0].first == "p");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kProgline);

    PqlWith* with = (PqlWith*)query->GetClauses()[0];
    Assert::IsTrue(with->GetParameters().first.first == "p");
    Assert::IsTrue(with->GetParameters().first.second == PqlDeclarationEntity::kProgline);
    Assert::IsTrue(with->GetParameters().second.first == "2");
    Assert::IsTrue(with->GetParameters().second.second == PqlDeclarationEntity::kInteger);
  };

  TEST_METHOD(TestWithCall) {
    string content = "call c; Select c with c.stmt# = 2";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["c"] == PqlDeclarationEntity::kCall);
    Assert::IsTrue(query->GetSelections()[0].first == "c");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kCall);

    PqlWith* with = (PqlWith*)query->GetClauses()[0];
    Assert::IsTrue(with->GetParameters().first.first == "c");
    Assert::IsTrue(with->GetParameters().first.second == PqlDeclarationEntity::kCall);
    Assert::IsTrue(with->GetParameters().second.first == "2");
    Assert::IsTrue(with->GetParameters().second.second == PqlDeclarationEntity::kInteger);
  };

  TEST_METHOD(TestWithCallProcName) {
    string content = "call c; Select c.procName with c.procName = \"abc\"";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["c"] == PqlDeclarationEntity::kCall);
    Assert::IsTrue(query->GetSelections()[0].first == "cp");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kCallName);

    PqlWith* with = (PqlWith*)query->GetClauses()[0];
    Assert::IsTrue(with->GetParameters().first.first == "cp");
    Assert::IsTrue(with->GetParameters().first.second == PqlDeclarationEntity::kCallName);
    Assert::IsTrue(with->GetParameters().second.first == "abc");
    Assert::IsTrue(with->GetParameters().second.second == PqlDeclarationEntity::kIdent);
  };

  TEST_METHOD(TestWithWith) {
    string content = "assign a1,a2; Select a1 with a1.stmt# = 2 with a1.stmt# = a2.stmt#";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a1"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["a2"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a1");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);

    PqlWith* with1 = (PqlWith*)query->GetClauses()[0];
    Assert::IsTrue(with1->GetParameters().first.first == "a1");
    Assert::IsTrue(with1->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(with1->GetParameters().second.first == "2");
    Assert::IsTrue(with1->GetParameters().second.second == PqlDeclarationEntity::kInteger);

    PqlWith* with2 = (PqlWith*)query->GetClauses()[1];
    Assert::IsTrue(with2->GetParameters().first.first == "a1");
    Assert::IsTrue(with2->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(with2->GetParameters().second.first == "a2");
    Assert::IsTrue(with2->GetParameters().second.second == PqlDeclarationEntity::kAssign);
  };

  TEST_METHOD(TestWithAnd) {
    string content = "assign a1, a2; Select a1 with a1.stmt# = 2 and a1.stmt# = a2.stmt#";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a1"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetDeclarations()["a2"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a1");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);

    PqlWith* with1 = (PqlWith*)query->GetClauses()[0];
    Assert::IsTrue(with1->GetParameters().first.first == "a1");
    Assert::IsTrue(with1->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(with1->GetParameters().second.first == "2");
    Assert::IsTrue(with1->GetParameters().second.second == PqlDeclarationEntity::kInteger);

    PqlWith* with2 = (PqlWith*)query->GetClauses()[1];
    Assert::IsTrue(with2->GetParameters().first.first == "a1");
    Assert::IsTrue(with2->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(with2->GetParameters().second.first == "a2");
    Assert::IsTrue(with2->GetParameters().second.second == PqlDeclarationEntity::kAssign);
  };

  TEST_METHOD(TestInvalidWith) {
    string content = "assign a; Select a with a.stmt# = 2.stmt#";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsFalse(parser.Parse());
  };

  TEST_METHOD(TestSuchthatPattern) {
    string content = "assign a; Select a such that Modifies(a,_) pattern a(\"abc\",_)";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);

    PqlSuchthat* suchthat = (PqlSuchthat*)query->GetClauses()[0];
    Assert::IsTrue(suchthat->GetType() == PqlSuchthatType::kModifiesS);
    Assert::IsTrue(suchthat->GetParameters().first.first == "a");
    Assert::IsTrue(suchthat->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(suchthat->GetParameters().second.first == "_");
    Assert::IsTrue(suchthat->GetParameters().second.second == PqlDeclarationEntity::kUnderscore);

    PqlPattern* pattern = (PqlPattern*)query->GetClauses()[1];
    Assert::IsTrue(pattern->GetType().first == "a");
    Assert::IsTrue(pattern->GetType().second == PqlPatternType::kAssign);
    Assert::IsTrue(pattern->GetFirstParameter().first == "abc");
    Assert::IsTrue(pattern->GetFirstParameter().second == PqlDeclarationEntity::kIdent);
    Assert::IsTrue(pattern->GetAssignExpression().first == PqlPatternExpressionType::kUnderscore);
    Assert::IsTrue(pattern->GetAssignExpression().second.empty());
  };

  TEST_METHOD(TestSuchthatWith) {
    string content = "assign a; Select a such that Modifies(a,_) with a.stmt# = 1";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);

    PqlSuchthat* suchthat = (PqlSuchthat*)query->GetClauses()[0];
    Assert::IsTrue(suchthat->GetType() == PqlSuchthatType::kModifiesS);
    Assert::IsTrue(suchthat->GetParameters().first.first == "a");
    Assert::IsTrue(suchthat->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(suchthat->GetParameters().second.first == "_");
    Assert::IsTrue(suchthat->GetParameters().second.second == PqlDeclarationEntity::kUnderscore);

    PqlWith* with = (PqlWith*)query->GetClauses()[1];
    Assert::IsTrue(with->GetParameters().first.first == "a");
    Assert::IsTrue(with->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(with->GetParameters().second.first == "1");
    Assert::IsTrue(with->GetParameters().second.second == PqlDeclarationEntity::kInteger);
  };

  TEST_METHOD(TestPatternWith) {
    string content = "assign a; Select a pattern a(\"abc\",_) with a.stmt# = 1";
    PqlQuery* query = new PqlQuery();
    PqlParser parser(content, query);
    Assert::IsTrue(parser.Parse());
    Assert::IsTrue(query->GetDeclarations()["a"] == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(query->GetSelections()[0].first == "a");
    Assert::IsTrue(query->GetSelections()[0].second == PqlDeclarationEntity::kAssign);

    PqlPattern* pattern = (PqlPattern*)query->GetClauses()[0];
    Assert::IsTrue(pattern->GetType().first == "a");
    Assert::IsTrue(pattern->GetType().second == PqlPatternType::kAssign);
    Assert::IsTrue(pattern->GetFirstParameter().first == "abc");
    Assert::IsTrue(pattern->GetFirstParameter().second == PqlDeclarationEntity::kIdent);
    Assert::IsTrue(pattern->GetAssignExpression().first == PqlPatternExpressionType::kUnderscore);
    Assert::IsTrue(pattern->GetAssignExpression().second.empty());

    PqlWith* with = (PqlWith*)query->GetClauses()[1];
    Assert::IsTrue(with->GetParameters().first.first == "a");
    Assert::IsTrue(with->GetParameters().first.second == PqlDeclarationEntity::kAssign);
    Assert::IsTrue(with->GetParameters().second.first == "1");
    Assert::IsTrue(with->GetParameters().second.second == PqlDeclarationEntity::kInteger);
  };
  };
}