#include "stdafx.h"
#include "CppUnitTest.h"
#include "parser.h"
#include "pkb.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using tt = Tokenizer::TokenType;

namespace ParserPKBTests {
TEST_CLASS(TestParserPkb){

public :

// Include both Follows and Follows* tests
TEST_METHOD(TestFollowsOfStatementsOnly){

  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program = "procedure one {\n a = b;\n c = d;\n e = f;\n }";
  TokenList tokenized_program = Tokenizer::Tokenize(program);

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

  // Should only be statement number 3
  Assert::AreEqual(*iter_1, *iter_2);

  //*************************************

  StmtNumList stmt_num_test_get_follows_t = test_pkb.GetFollowsT("1");
  StmtNumList stmt_num_true_get_follows_t = true_pkb.GetFollowsT("1");

  iter_1 = stmt_num_test_get_follows_t.begin();
  iter_2 = stmt_num_true_get_follows_t.begin();

  // Should have 2 statements
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

}  // namespace PKBPQLTests

// Include both Follows and Follows* tests
TEST_METHOD(TestFollowsOfOneNestedStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"
      "if (a > b) then {\n"
      "e = f;\n"
      "asd = ddd;\n"
      "}\n"
      "else {\n"
      "k = 3;\n"
      "}\n"
      "while (a < 10) {\n"
      "a = a + 1;\n"
      "}\n"
      "}\n";

  TokenList tokenized_program = Tokenizer::Tokenize(program);

  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertFollows("3", "4");
  true_pkb.InsertFollows("1", "2");
  true_pkb.InsertFollows("1", "6");
  true_pkb.InsertFollows("2", "6");

  //*************************************

  StmtNumList stmt_nums_test_get_all_followed_by = test_pkb.GetAllFollowedBy();
  StmtNumList stmt_nums_true_get_all_followed_by = true_pkb.GetAllFollowedBy();

  StmtNumList::iterator iter_1 = stmt_nums_test_get_all_followed_by.begin();
  StmtNumList::iterator iter_2 = stmt_nums_true_get_all_followed_by.begin();

  // Should have 3 followed by stmt_num
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //**************************************

  StmtNumList stmt_nums_test_get_all_follows = test_pkb.GetAllFollows();
  StmtNumList stmt_nums_true_get_all_follows = true_pkb.GetAllFollows();

  iter_1 = stmt_nums_test_get_all_follows.begin();
  iter_2 = stmt_nums_true_get_all_follows.begin();

  // Should have 3 follows stmt_num
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //**************************************

  StmtNumList stmt_num_test_get_follows = test_pkb.GetFollows("1");
  StmtNumList stmt_num_true_get_follows = true_pkb.GetFollows("1");

  iter_1 = stmt_num_test_get_follows.begin();
  iter_2 = stmt_num_true_get_follows.begin();

  // Should have 1 follows stmt_num
  Assert::AreEqual(*iter_1, *iter_2);

  //*************************************

  StmtNumList stmt_num_test_get_follows_t = test_pkb.GetFollowsT("1");
  StmtNumList stmt_num_true_get_follows_t = true_pkb.GetFollowsT("1");

  iter_1 = stmt_num_test_get_follows_t.begin();
  iter_2 = stmt_num_true_get_follows_t.begin();

  // Should have 2 follows stmt_num
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);
}
TEST_METHOD(TestParentOfOneNestedStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"             // 1
      "if (a > b) then {\n"  // 2
      "e = f;\n"             // 3
      "asd = ddd;\n"         // 4
      "}\n"
      "else {\n"
      "k = 3;\n"  // 5
      "}\n"
      "while (a < 10) {\n"  // 6
      "a = a + 1;\n"        // 7
      "}\n"
      "}\n";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertParent("2", "3");
  true_pkb.InsertParent("2", "4");
  true_pkb.InsertParent("2", "5");
  true_pkb.InsertParent("6", "7");

  //*******************************

  StmtNumList test_all_parents = test_pkb.GetAllParent();
  StmtNumList true_all_parents = true_pkb.GetAllParent();

  StmtNumList::iterator iter_1 = test_all_parents.begin();
  StmtNumList::iterator iter_2 = true_all_parents.begin();

  // Should only have two parents
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_all_children = test_pkb.GetAllChild();
  StmtNumList true_all_children = true_pkb.GetAllChild();

  iter_1 = test_all_children.begin();
  iter_2 = true_all_children.begin();

  // Should have four children stmt_num
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*********************************

  StmtNumList test_child_of_stmt = test_pkb.GetChild("2");
  StmtNumList true_child_of_stmt = true_pkb.GetChild("2");

  iter_1 = test_child_of_stmt.begin();
  iter_2 = true_child_of_stmt.begin();

  // Should have three children for stmt_num 2
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //**********************************
  StmtNumList test_parent_of_stmt = test_pkb.GetParent("7");
  StmtNumList true_parent_of_stmt = true_pkb.GetParent("7");

  iter_1 = test_parent_of_stmt.begin();
  iter_2 = true_parent_of_stmt.begin();

  // Should have one parent of stmt_num 7
  Assert::AreEqual(*iter_1, *iter_2);
}
TEST_METHOD(TestParentOfNestedStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"             // 1
      "if (a > b) then {\n"  // 2
      "e = f;\n"             // 3
      "asd = ddd;\n"         // 4
      "while (a < 10) {\n"   // 5
      "a = a + 1;\n"         // 6
      "k = k + 1;\n"         // 7
      "}\n"
      "}\n"
      "else {\n"
      "k = 3;\n"  // 8
      "}\n"
      "}\n";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertParent("5", "6");
  true_pkb.InsertParent("5", "7");
  true_pkb.InsertParent("2", "3");
  true_pkb.InsertParent("2", "4");
  true_pkb.InsertParent("2", "5");
  true_pkb.InsertParent("2", "8");

  //*******************************

  StmtNumList test_all_parents = test_pkb.GetAllParent();
  StmtNumList true_all_parents = true_pkb.GetAllParent();

  StmtNumList::iterator iter_1 = test_all_parents.begin();
  StmtNumList::iterator iter_2 = true_all_parents.begin();

  // Should only have two parents
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_all_children = test_pkb.GetAllChild();
  StmtNumList true_all_children = true_pkb.GetAllChild();

  iter_1 = test_all_children.begin();
  iter_2 = true_all_children.begin();

  // Should have six children stmt_num
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*********************************

  StmtNumList test_child_of_stmt = test_pkb.GetChild("5");
  StmtNumList true_child_of_stmt = true_pkb.GetChild("5");

  iter_1 = test_child_of_stmt.begin();
  iter_2 = true_child_of_stmt.begin();

  // Should have two children for stmt_num 2
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //**********************************

  StmtNumList test_parent_of_stmt = test_pkb.GetParent("5");
  StmtNumList true_parent_of_stmt = true_pkb.GetParent("5");

  iter_1 = test_parent_of_stmt.begin();
  iter_2 = true_parent_of_stmt.begin();

  // Should have one parent of stmt_num 5
  Assert::AreEqual(*iter_1, *iter_2);
}
TEST_METHOD(TestParentStarOfNestedStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"             // 1
      "if (a > b) then {\n"  // 2
       "e = f;\n"             // 3
       "asd = ddd;\n"         // 4
       "while (a < 10) {\n"   // 5
        "a = a + 1;\n"         // 6
        "t = t + 1;\n"         // 7
        "while (t == 10) {"    // 8
           "f = f + 1;"         // 9
        "}"
       "}\n"
      "} else {\n"
        "k = 3;\n"             // 10
      "}\n"
      "}\n";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertParent("8", "9");
  true_pkb.InsertParent("5", "6");
  true_pkb.InsertParent("5", "7");
  true_pkb.InsertParent("5", "8");
  true_pkb.InsertParent("2", "3");
  true_pkb.InsertParent("2", "4");
  true_pkb.InsertParent("2", "5");
  true_pkb.InsertParent("2", "10");

  true_pkb.InsertParentT("5", "9");
  true_pkb.InsertParentT("2", "5");
  true_pkb.InsertParentT("2", "6");
  true_pkb.InsertParentT("2", "7");
  true_pkb.InsertParentT("2", "8");
  true_pkb.InsertParentT("2", "9");
  //*******************************

  StmtNumList test_parent_t = test_pkb.GetParentT("9");
  StmtNumList true_parent_t = true_pkb.GetParentT("9");

  StmtNumList::iterator iter_1 = test_parent_t.begin();
  StmtNumList::iterator iter_2 = true_parent_t.begin();

  // Should only have three parents*
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_child_t = test_pkb.GetChildT("2");
  StmtNumList true_child_t = true_pkb.GetChildT("2");

  iter_1 = test_child_t.begin();
  iter_2 = true_child_t.begin();

  // Should only have eight child direct/indirect
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************
}
TEST_METHOD(TestUsesOfOnlyAssignmentStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"           // 1
      "c =  d + e + f;\n"  // 2
      "j = k + l + 1;\n"   // 3
      "}\n";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertUsesS("1", "b");
  true_pkb.InsertUsesS("2", "d");
  true_pkb.InsertUsesS("2", "e");
  true_pkb.InsertUsesS("2", "f");
  true_pkb.InsertUsesS("3", "k");
  true_pkb.InsertUsesS("3", "l");

  //*******************************

  StmtNumList test_all_using_stmts = test_pkb.GetAllUsingStmt();
  StmtNumList true_all_using_stmts = true_pkb.GetAllUsingStmt();

  StmtNumList::iterator iter_1 = test_all_using_stmts.begin();
  StmtNumList::iterator iter_2 = true_all_using_stmts.begin();

  // Should only have three using statements
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_used_var_of_stmt = test_pkb.GetUsedVarS("3");
  StmtNumList true_used_var_of_stmt = true_pkb.GetUsedVarS("3");

  iter_1 = test_used_var_of_stmt.begin();
  iter_2 = true_used_var_of_stmt.begin();

  // Should have two used vars
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************
}

TEST_METHOD(TestUsesOfNestedStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"             // 1
      "if (a > b) then {\n"  // 2
      "e = f;\n"             // 3
      "asd = ddd;\n"         // 4
      "while (a < 10) {\n"   // 5
      "a = a + 1;\n"         // 6
      "k = k + 1;\n"         // 7
      "}\n"
      "}\n"
      "else {\n"
      "k = 3;\n"             // 8
      "print print;"         // 9
      "}\n"
      "}\n";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertUsesS("1", "b");
  true_pkb.InsertUsesS("3", "f");
  true_pkb.InsertUsesS("4", "ddd");
  true_pkb.InsertUsesS("6", "a");
  true_pkb.InsertUsesS("7", "k");
  true_pkb.InsertUsesS("5", "a");
  true_pkb.InsertUsesS("5", "a");
  true_pkb.InsertUsesS("5", "k");
  true_pkb.InsertUsesS("9", "print");
  true_pkb.InsertUsesS("2", "a");
  true_pkb.InsertUsesS("2", "b");
  true_pkb.InsertUsesS("2", "f");
  true_pkb.InsertUsesS("2", "ddd");
  true_pkb.InsertUsesS("2", "a");
  true_pkb.InsertUsesS("2", "a");
  true_pkb.InsertUsesS("2", "k");
  true_pkb.InsertUsesS("2", "print");

  true_pkb.InsertUsesP("one", "b");
  true_pkb.InsertUsesP("one", "a");
  true_pkb.InsertUsesP("one", "f");
  true_pkb.InsertUsesP("one", "ddd");
  true_pkb.InsertUsesP("one", "k");
  true_pkb.InsertUsesP("one", "print");

  //*******************************

  StmtNumList test_all_using_stmts = test_pkb.GetAllUsingStmt();
  StmtNumList true_all_using_stmts = true_pkb.GetAllUsingStmt();

  StmtNumList::iterator iter_1 = test_all_using_stmts.begin();
  StmtNumList::iterator iter_2 = true_all_using_stmts.begin();

  // Should only have eight using statements
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************
  StmtNumList test_used_var_of_stmt = test_pkb.GetUsedVarS("2");
  StmtNumList true_used_var_of_stmt = true_pkb.GetUsedVarS("2");

  iter_1 = test_used_var_of_stmt.begin();
  iter_2 = true_used_var_of_stmt.begin();

  // Should have five used vars
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************
  StmtNumList test_used_vars_proc = test_pkb.GetUsedVarP("one");
  StmtNumList true_used_vars_proc = true_pkb.GetUsedVarP("one");

  iter_1 = test_used_vars_proc.begin();
  iter_2 = true_used_vars_proc.begin();

  // Should have six used vars in procedure "one"
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************
}
TEST_METHOD(TestModifiesOfOnlyAssignmentStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"          // 1
      "while = k + 1;\n"  // 2
      "if = x+ y + z;\n"  // 3
      "if = 2;\n"         // 4
      "if = 1;\n"         // 5
      "}";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertModifiesS("1", "a");
  true_pkb.InsertModifiesS("2", "while");
  true_pkb.InsertModifiesS("3", "if");
  true_pkb.InsertModifiesS("4", "if");
  true_pkb.InsertModifiesS("5", "if");

  //*******************************

  StmtNumList test_all_modified_stmts = test_pkb.GetAllModifyingS();
  StmtNumList true_all_modified_stmts = true_pkb.GetAllModifyingS();

  StmtNumList::iterator iter_1 = test_all_modified_stmts.begin();
  StmtNumList::iterator iter_2 = true_all_modified_stmts.begin();

  // Should only have five modified statements
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_modified_var_at_stmts = test_pkb.GetModifyingS("if");
  StmtNumList true_modified_var_at_stmts = true_pkb.GetModifyingS("if");

  iter_1 = test_modified_var_at_stmts.begin();
  iter_2 = true_modified_var_at_stmts.begin();

  // Should only have three stmt_num that modifies "if" variable
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_modified_stmt_get_var = test_pkb.GetModifiedVarS("2");
  StmtNumList true_modified_stmt_get_var = true_pkb.GetModifiedVarS("2");

  iter_1 = test_modified_stmt_get_var.begin();
  iter_2 = true_modified_stmt_get_var.begin();

  // Should only have one modified variable at stmt_num 2
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************
}
TEST_METHOD(TestModifiesOfNestedStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "	procedure one {\n"
      "a = b;\n"              // 1
      "while (x < y) {\n"     // 2
      "if (a == 1) then {\n"  // 3
      "if = 2;\n"             // 4
      "z = a +1;\n"           // 5
      "} else {\n"
      "t = abc;\n"          // 6
      "while (a == b) {\n"  // 7
      "w = 1 + 2;\n"        // 8
      "a = b + 1;\n"        // 9
      "read x;"             // 10
      "}\n"
      "}\n"
      "}"
      "}";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  PKB true_pkb = PKB();
  true_pkb.InsertModifiesS("1", "a");
  true_pkb.InsertModifiesS("4", "if");
  true_pkb.InsertModifiesS("5", "z");
  true_pkb.InsertModifiesS("6", "t");
  true_pkb.InsertModifiesS("8", "w");
  true_pkb.InsertModifiesS("9", "a");
  true_pkb.InsertModifiesS("10", "x");
  true_pkb.InsertModifiesS("7", "w");
  true_pkb.InsertModifiesS("7", "a");
  true_pkb.InsertModifiesS("7", "x");
  true_pkb.InsertModifiesS("3", "if");
  true_pkb.InsertModifiesS("3", "z");
  true_pkb.InsertModifiesS("3", "t");
  true_pkb.InsertModifiesS("3", "w");
  true_pkb.InsertModifiesS("3", "a");
  true_pkb.InsertModifiesS("3", "x");
  true_pkb.InsertModifiesS("2", "if");
  true_pkb.InsertModifiesS("2", "w");
  true_pkb.InsertModifiesS("2", "z");
  true_pkb.InsertModifiesS("2", "t");
  true_pkb.InsertModifiesS("2", "a");
  true_pkb.InsertModifiesS("2", "x");

  true_pkb.InsertModifiesP("one", "a");
  true_pkb.InsertModifiesP("one", "w");
  true_pkb.InsertModifiesP("one", "if");
  true_pkb.InsertModifiesP("one", "z");
  true_pkb.InsertModifiesP("one", "t");
  true_pkb.InsertModifiesP("one", "x");
  //*****************************

  StmtNumList test_all_modified_stmts = test_pkb.GetAllModifyingS();
  StmtNumList true_all_modified_stmts = true_pkb.GetAllModifyingS();

  StmtNumList::iterator iter_1 = test_all_modified_stmts.begin();
  StmtNumList::iterator iter_2 = true_all_modified_stmts.begin();

  // Should only have ten modified statements
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_modified_var_at_stmts = test_pkb.GetModifyingS("a");
  StmtNumList true_modified_var_at_stmts = true_pkb.GetModifyingS("a");

  iter_1 = test_modified_var_at_stmts.begin();
  iter_2 = true_modified_var_at_stmts.begin();

  // Should only have five stmt_num that modifies variable "a"
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  StmtNumList test_modified_stmt_get_var = test_pkb.GetModifiedVarS("2");
  StmtNumList true_modified_stmt_get_var = true_pkb.GetModifiedVarS("2");

  iter_1 = test_modified_stmt_get_var.begin();
  iter_2 = true_modified_stmt_get_var.begin();

  // Should only have six modified variable at stmt_num 2
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************

  
  StmtNumList test_modified_vars_proc = test_pkb.GetModifiedVarP("one");
  StmtNumList true_modified_vars_proc = true_pkb.GetModifiedVarP("one");

  iter_1 = test_modified_vars_proc.begin();
  iter_2 = true_modified_vars_proc.begin();

  // Should only have six modified variable at procedure "one"
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1++, *iter_2++);
  Assert::AreEqual(*iter_1, *iter_2);

  //*******************************
}
TEST_METHOD(TestCfgIfWhileStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "procedure one {\n"
      "a = b;\n"               // 1
      "if (x < y) then {\n"    // 2
        "print x;"             // 3
        "read t;"              // 4
        "while (a == 1) {\n"   // 5
          "if = 2;\n"          // 6
          "z = a +1;\n"        // 7
        "}"
      "} else {"
        "print z;"             // 8
      "} "
      "while (j > f) {"        // 9
         "s = s * l;"          // 10
         "asd = asd / 2;"      // 11
         "if (a < 100) then {" // 12
            "x = 100;"         // 13
         "} else {"     
            "s = 500;"         // 14
         "}"     
      "}"
      "a = 1;"                // 15
      "}";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  vector<StmtNumIntList> list_of_adj_list;
  list_of_adj_list.push_back(StmtNumIntList());

  list_of_adj_list.push_back(StmtNumIntList({2}));       // stmt1 adj_list
  list_of_adj_list.push_back(StmtNumIntList({3, 8}));    // stmt2 adj_list
  list_of_adj_list.push_back(StmtNumIntList({4}));        // stmt3 adj_list
  list_of_adj_list.push_back(StmtNumIntList({5}));       // stmt4 adj_list
  list_of_adj_list.push_back(StmtNumIntList({6, 9}));    // stmt5 adj_list
  list_of_adj_list.push_back(StmtNumIntList({7}));       // stmt6 adj_list
  list_of_adj_list.push_back(StmtNumIntList({5}));       // stmt7 adj_list
  list_of_adj_list.push_back(StmtNumIntList({9}));       // stmt8 adj_list
  list_of_adj_list.push_back(StmtNumIntList({10, 15}));  // stmt9 adj_list
  list_of_adj_list.push_back(StmtNumIntList({11}));      // stmt10 adj_list
  list_of_adj_list.push_back(StmtNumIntList({12}));      // stmt11 adj_list
  list_of_adj_list.push_back(StmtNumIntList({13, 14}));  // stmt12 adj_list
  list_of_adj_list.push_back(StmtNumIntList({9}));      // stmt13 adj_list
  list_of_adj_list.push_back(StmtNumIntList({9}));      // stmt14 adj_list

  CFG* test_cfg = test_pkb.GetCFG("one");

  for (size_t i = 1; i < list_of_adj_list.size(); i++) {
    StmtNumList test_adj_list = (*test_cfg).at(std::to_string(i));
    StmtNumIntList true_adj_list = list_of_adj_list.at(i);

    Assert::AreEqual(test_adj_list.size(), true_adj_list.size());
    for (size_t j = 0; j < test_adj_list.size(); j++) {
      Assert::AreEqual(test_adj_list.at(j), std::to_string(true_adj_list.at(j)));
    }
  }
}
TEST_METHOD(TestCfgIfInIfStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "procedure one {\n"
        "a = b;\n"                // 1
        "if (x < y) then {\n"          // 2
          "if (a == 1) then {\n"  // 3
            "if = 2;\n"           // 4
            "z = a +1;\n"         // 5
          "} else {"              
              "a = a + 1;"        // 6
            "}"
            "x = x+ 1;"           // 7
         "} else {\n"
            "t = abc;\n"          // 8
            "if (a == b) then {\n"     // 9
              "w = 1 + 2;\n"      // 10
              "a = b + 1;\n"      // 11
             "} else {\n"         
                "a = 1;"          //12
             "}\n"
        "}\n"
        "x = 1 + y;\n"            //13
      "}";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  vector<StmtNumIntList> list_of_adj_list;
  list_of_adj_list.push_back(StmtNumIntList());

  list_of_adj_list.push_back(StmtNumIntList({2})); //stmt1 adj_list
  list_of_adj_list.push_back(StmtNumIntList({3, 8})); //stmt2 adj_list
  list_of_adj_list.push_back(StmtNumIntList({4, 6})); //stmt3 adj_list
  list_of_adj_list.push_back(StmtNumIntList({5})); //stmt4 adj_list
  list_of_adj_list.push_back(StmtNumIntList({7})); //stmt5 adj_list
  list_of_adj_list.push_back(StmtNumIntList({7})); //stmt6 adj_list
  list_of_adj_list.push_back(StmtNumIntList({13})); //stmt7 adj_list
  list_of_adj_list.push_back(StmtNumIntList({9})); //stmt8 adj_list
  list_of_adj_list.push_back(StmtNumIntList({10, 12})); //stmt9 adj_list
  list_of_adj_list.push_back(StmtNumIntList({11})); //stmt10 adj_list
  list_of_adj_list.push_back(StmtNumIntList({13})); //stmt11 adj_list
  list_of_adj_list.push_back(StmtNumIntList({13})); //stmt12 adj_list


  CFG* test_cfg = test_pkb.GetCFG("one"); 

  for (int i = 1; i < list_of_adj_list.size(); i++) {
    StmtNumList test_adj_list = (*test_cfg).at(std::to_string(i));
    StmtNumIntList true_adj_list = list_of_adj_list.at(i);

    Assert::AreEqual(test_adj_list.size(), true_adj_list.size());
    for (size_t j = 0; j < test_adj_list.size(); j++) {
      Assert::AreEqual(test_adj_list.at(j), std::to_string(true_adj_list.at(j)));
    }
  }
 }
TEST_METHOD(TestCfgIfInIfInIfStatements) {
  PKB test_pkb = PKB();
  Parser parser = Parser(&test_pkb);
  string program =
      "procedure one {\n"
      "a = b;\n"                  // 1
      "if (x < y) then {\n"       // 2
          "a = 1;"                // 3
      "} else {\n"
         "if (a == 1) then {\n"   // 4
            "if = 2;\n"           // 5
            "z = a +1;\n"         // 6
         "} else {"
            "a = a + 1;"          // 7
            "if (t == 1) then {"  // 8
                "print x;"        // 9
            "} else {"
                "a = z + x;"      // 10
            "}"
         "}"
      "}\n"
      "x = 1 + y;\n"              // 11
      "}";

  TokenList tokenized_program = Tokenizer::Tokenize(program);
  parser.Parse(tokenized_program);

  vector<StmtNumIntList> list_of_adj_list;
  list_of_adj_list.push_back(StmtNumIntList());

  list_of_adj_list.push_back(StmtNumIntList({2}));       // stmt1 adj_list
  list_of_adj_list.push_back(StmtNumIntList({3, 4}));    // stmt2 adj_list
  list_of_adj_list.push_back(StmtNumIntList({11}));      // stmt3 adj_list
  list_of_adj_list.push_back(StmtNumIntList({5, 7}));    // stmt4 adj_list
  list_of_adj_list.push_back(StmtNumIntList({6}));       // stmt5 adj_list
  list_of_adj_list.push_back(StmtNumIntList({11}));      // stmt6 adj_list
  list_of_adj_list.push_back(StmtNumIntList({8}));       // stmt7 adj_list
  list_of_adj_list.push_back(StmtNumIntList({9, 10}));   // stmt8 adj_list
  list_of_adj_list.push_back(StmtNumIntList({11}));      // stmt9 adj_list
  list_of_adj_list.push_back(StmtNumIntList({11}));      // stmt10 adj_list

  CFG* test_cfg = test_pkb.GetCFG("one");

  for (int i = 1; i < list_of_adj_list.size(); i++) {
    StmtNumList test_adj_list = (*test_cfg).at(std::to_string(i));
    StmtNumIntList true_adj_list = list_of_adj_list.at(i);

    Assert::AreEqual(test_adj_list.size(), true_adj_list.size());
    for (size_t j = 0; j < test_adj_list.size(); j++) {
      Assert::AreEqual(test_adj_list.at(j), std::to_string(true_adj_list.at(j)));
    }
  }
}
}
;
}