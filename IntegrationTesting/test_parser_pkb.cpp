#include "stdafx.h"
#include "CppUnitTest.h"
#include "parser.h"
#include "pkb.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using tt = Tokenizer::TokenType;

namespace PKBPQLTests {
	TEST_CLASS(TestParserPkb) {

		public: 
			TEST_METHOD(TestFollowOfStatementsOnly){
				
				PKB test_pkb = PKB();
				Parser parser = Parser(&test_pkb);
				TokenList tokenized_program = {{tt::kName, "procedure", 1},
																													{tt::kName, "one"},
																													{tt::kOpenBrace, "{"},
																													{tt::kName, "a"},
																													{tt::kAssignment, "="},
																													{tt::kName, "b"},
																													{tt::kSemicolon, ";"},
																													{tt::kName, "c"},
																													{tt::kAssignment, "="},
																													{tt::kName, "d"},
																													{tt::kSemicolon, ";"},
																													{tt::kName, "e"},
																													{tt::kAssignment, "="},
																													{tt::kName, "f"},
																													{tt::kSemicolon, ";"},
																													{tt::kCloseBrace, "}"},
																													{tt::kEOF, string()}};

				parser.Parse(tokenized_program);

				PKB true_pkb = PKB();
				true_pkb.InsertFollows("1", "2");
				true_pkb.InsertFollows("1", "3");
				true_pkb.InsertFollows("2", "3");

				//*********************************
				StmtNumList stmt_nums_test_get_all_followed_by = test_pkb.GetAllFollowedBy();
				StmtNumList stmt_nums_true_get_all_followed_by = true_pkb.GetAllFollowedBy();

				StmtNumList::iterator iter_1 = stmt_nums_test_get_all_followed_by.begin(); 
				StmtNumList::iterator iter_2 = stmt_nums_true_get_all_followed_by.begin(); 
				
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);
				//***********************************
				StmtNumList stmt_nums_test_get_all_follows = test_pkb.GetAllFollows();
				StmtNumList stmt_nums_true_get_all_follows = true_pkb.GetAllFollows();

				iter_1 = stmt_nums_test_get_all_follows.begin(); 
				iter_2 = stmt_nums_true_get_all_follows.begin(); 
				
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);
				//*************************************
				StmtNumList stmt_num_test_get_follows = test_pkb.GetFollows("2");
				StmtNumList stmt_num_true_get_follows = true_pkb.GetFollows("2");

				iter_1 = stmt_num_test_get_follows.begin();
				iter_2 = stmt_num_true_get_follows.begin();

				//Should only be statement number 3
				Assert::AreEqual(*iter_1, *iter_2);
		}

		TEST_METHOD(TestFollowsOfOneNestedStatements) {

					PKB test_pkb = PKB();
					Parser parser = Parser(&test_pkb);
     /*
							procedure one {
									a = b;																							//1
									if (a > b) then {												//2
											e = f;																					//3
											asd = ddd;																	//4
									} else {
											k = 3;																					//5
									}
									while (a < 10) {													//6
											a = a + 1;																	//7
									}


					*/
					TokenList tokenized_program = {{tt::kName, "procedure", 1},
																											{tt::kName, "one"},
																											{tt::kOpenBrace, "{"},
																											{tt::kName, "a"},
																											{tt::kAssignment, "="},
																											{tt::kName, "b"},
																											{tt::kSemicolon, ";"},
																											{tt::kName, "if", 2},
																											{tt::kOpenParen, "("},
																											{tt::kName, "a"},
																											{tt::kRelational, ">"},
																											{tt::kName, "b"},
																											{tt::kCloseParen, ")"},
																											{tt::kName, "then", 3},
																											{tt::kOpenBrace, "{"},
																											{tt::kName, "e"},
																											{tt::kAssignment, "="},
																											{tt::kName, "f"},
																											{tt::kSemicolon, ";"},
																											{tt::kName, "asd"},
																											{tt::kAssignment, "="},
																											{tt::kName, "ddd"},
																											{tt::kSemicolon, ";"},
																											{tt::kCloseBrace, "}"},
																											{tt::kName, "else", 4},
																											{tt::kOpenBrace, "{"},
																											{tt::kName, "k"},
																											{tt::kAssignment, "="},
																											{tt::kDigit, "3"},
																											{tt::kSemicolon, ";"},
																											{tt::kCloseBrace, "}"},
																											{tt::kName, "while", 5},
																											{tt::kOpenParen, "("},
																											{tt::kName, "a"},
																											{tt::kRelational, "<"},
																											{tt::kDigit, "10"},
																											{tt::kCloseParen, ")"},
																											{tt::kOpenBrace, "{"},
																											{tt::kName, "a"},
																											{tt::kAssignment, "="},
																											{tt::kName, "a"},
																											{tt::kOperator, "+"},
																											{tt::kDigit, "1"},
																											{tt::kSemicolon, ";"},
																											{tt::kCloseBrace, "}"},
																											{tt::kCloseBrace, "}"},
																											{tt::kEOF, string()}};

					parser.Parse(tokenized_program);

					PKB true_pkb = PKB();
					true_pkb.InsertFollows("3", "4");
					true_pkb.InsertFollows("1", "2");
					true_pkb.InsertFollows("1", "6");
					true_pkb.InsertFollows("2", "6");

				//***************************************
				StmtNumList stmt_nums_test_get_all_followed_by = test_pkb.GetAllFollowedBy();
				StmtNumList stmt_nums_true_get_all_followed_by = true_pkb.GetAllFollowedBy();

				StmtNumList::iterator iter_1 = stmt_nums_test_get_all_followed_by.begin(); 
				StmtNumList::iterator iter_2 = stmt_nums_true_get_all_followed_by.begin(); 
				
				// Should have 3 followed by stmt_num
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);
				//*****************************************
				StmtNumList stmt_nums_test_get_all_follows = test_pkb.GetAllFollows();
				StmtNumList stmt_nums_true_get_all_follows = true_pkb.GetAllFollows();

				iter_1 = stmt_nums_test_get_all_follows.begin(); 
				iter_2 = stmt_nums_true_get_all_follows.begin(); 
				
				// Should have 3 follows stmt_num
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);
				//****************************************
				StmtNumList stmt_num_test_get_follows = test_pkb.GetFollows("1");
				StmtNumList stmt_num_true_get_follows = true_pkb.GetFollows("1");

				iter_1 = stmt_num_test_get_follows.begin();
				iter_2 = stmt_num_true_get_follows.begin();

				// Should have 1 follows stmt_num
				Assert::AreEqual(*iter_1, *iter_2);
		}
};
}