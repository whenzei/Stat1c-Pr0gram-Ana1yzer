#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_query.h"
#include "pkb.h"
#include "query_evaluator.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPKBPQL){
  public :

      TEST_METHOD(TestSelectAssignQuery){
          // TODO: Your test code here
			PKB pkb;
			pkb.InsertAssignStmt(1, "x = y");
			pkb.InsertAssignStmt(8, "i = j");

			QueryEvaluator qe;
			string user_query = "assign a; Select a;";
			PqlQuery query(user_query);

			list<string> resultlist = qe.GetResultFromQuery(query, pkb);
			string output = qe.FormatResultString(resultlist);

			string expected_output = "statement #1, #8";

			Assert::AreEqual(expected_output, output);
      }

};
}