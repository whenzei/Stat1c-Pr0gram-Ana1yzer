#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql_parser.h"
#include "pql_query.h"
#include "pkb.h"
#include "pql_evaluator.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PKBPQLTests {
TEST_CLASS(TestPKBPQL){
  public :

      TEST_METHOD(TestSelectAssignQuery){
          // TODO: Your test code here
			PKB pkb;
			pkb.InsertAssignStmt(1, 0, "x", unordered_set<string>());
			pkb.InsertAssignStmt(8, 0, "i", unordered_set<string>());

			PqlEvaluator qe;
			string user_query = "assign a; Select a";
      PqlQuery* query = new PqlQuery();
      PqlParser parser(user_query, query);
      parser.Parse();

			list<string> resultlist = qe.GetResultFromQuery(query, pkb);

			string output = resultlist.front();

			string expected_output = "1";

			Assert::AreEqual(expected_output, output);
      }

};
}