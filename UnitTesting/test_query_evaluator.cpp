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
	DeclarationKeyword keyword_assign = DeclarationKeyword::kAssign;
	DeclarationKeyword keyword_variable = DeclarationKeyword::kVariable;
	DeclarationKeyword keyword_procedure = DeclarationKeyword::kProcedure;
	Declaration declr_assign = Declaration::Declaration(keyword_assign, "a");
	Declaration declr_variable = Declaration::Declaration(keyword_variable, "v");
	Declaration declr_procedure = Declaration::Declaration(keyword_procedure, "p");
	TEST_METHOD(TestGetResultFromQuery){
	// TODO: Your test code here
	QueryEvaluator qe;
	PKB pkb;

	pkb.PKB::InsertAssignStmt(1, "x = y");
	
	Query q;
	q.Query::SetVarName("a");
    q.Query::AddDeclaration(declr_variable);
    q.Query::AddDeclaration(declr_assign);
    q.Query::AddDeclaration(declr_procedure);

	std::list<std::string> resultlist = qe.QueryEvaluator::GetResultFromQuery(q, pkb);

	std::string first_result = resultlist.front();

	std::string expected_result = "1";

	Assert::AreEqual(expected_result, first_result);
        Assert::AreEqual(expected_result, first_result);

	}

};
}