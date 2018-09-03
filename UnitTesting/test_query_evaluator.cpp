#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_declaration.h"
#include "pql_query.h"
#include "pkb.h"
#include "query_evaluator.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestQueryEvaluator){

	public :
  PqlDeclarationKeyword keyword_assign = PqlDeclarationKeyword::kAssign;
         PqlDeclarationKeyword keyword_variable =
             PqlDeclarationKeyword::kVariable;
  PqlDeclarationKeyword keyword_procedure = PqlDeclarationKeyword::kProcedure;
         PqlDeclaration declr_assign =
      PqlDeclaration::PqlDeclaration(keyword_assign, "a");
  PqlDeclaration declr_variable =
             PqlDeclaration::PqlDeclaration(keyword_variable, "v");
         PqlDeclaration declr_procedure =
      PqlDeclaration::PqlDeclaration(keyword_procedure, "p");
	TEST_METHOD(TestGetResultFromQuery){
	// TODO: Your test code here
	QueryEvaluator qe;
	PKB pkb;

	pkb.PKB::InsertAssignStmt(1, "x = y");
	
	PqlQuery q;
        q.PqlQuery::SetVarName("a");
        q.PqlQuery::AddDeclaration(declr_variable);
        q.PqlQuery::AddDeclaration(declr_assign);
        q.PqlQuery::AddDeclaration(declr_procedure);

	std::list<std::string> resultlist = qe.QueryEvaluator::GetResultFromQuery(q, pkb);

	std::string first_result = resultlist.front();

	std::string expected_result = "1";

	Assert::AreEqual(expected_result, first_result);
        Assert::AreEqual(expected_result, first_result);

	}

};
}