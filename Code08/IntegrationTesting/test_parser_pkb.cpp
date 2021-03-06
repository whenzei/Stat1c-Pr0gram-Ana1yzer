#include "stdafx.h"
#include "CppUnitTest.h"
#include "design_extractor.h"
#include "parser.h"
#include "pkb.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using tt = Tokenizer::TokenType;

namespace ParserPKBTests {
TEST_CLASS(TestParserPkb) {
  const string kProcName1 = "one";

 public:
  // Include both Follows and Follows* tests
  TEST_METHOD(TestFollowsOfStatementsOnly) {
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    string program = "procedure one {\n a = b;\n c = d;\n e = f;\n }";
    TokenList tokenized_program = Tokenizer::Tokenize(program);

    parser.Parse(tokenized_program);

    PKB true_pkb = PKB();
    true_pkb.InsertFollows(1, 2);
    true_pkb.InsertFollows(1, 3);
    true_pkb.InsertFollows(2, 3);

    //*********************************

    StmtNumList stmt_nums_test_get_all_followed_by =
        test_pkb.GetAllFollowedBy();
    StmtNumList stmt_nums_true_get_all_followed_by =
        true_pkb.GetAllFollowedBy();

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

    StmtNumList stmt_num_test_get_follows = test_pkb.GetFollows(2);
    StmtNumList stmt_num_true_get_follows = true_pkb.GetFollows(2);

    iter_1 = stmt_num_test_get_follows.begin();
    iter_2 = stmt_num_true_get_follows.begin();

    // Should only be statement number 3
    Assert::AreEqual(*iter_1, *iter_2);

    //*************************************

    StmtNumList stmt_num_test_get_follows_t = test_pkb.GetFollowsT(1);
    StmtNumList stmt_num_true_get_follows_t = true_pkb.GetFollowsT(1);

    iter_1 = stmt_num_test_get_follows_t.begin();
    iter_2 = stmt_num_true_get_follows_t.begin();

    // Should have 2 statements
    Assert::AreEqual(*iter_1++, *iter_2++);
    Assert::AreEqual(*iter_1, *iter_2);

  }  // namespace ParserPKBTests

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
    true_pkb.InsertFollows(3, 4);
    true_pkb.InsertFollows(1, 2);
    true_pkb.InsertFollows(1, 6);
    true_pkb.InsertFollows(2, 6);

    //*************************************

    StmtNumList stmt_nums_test_get_all_followed_by =
        test_pkb.GetAllFollowedBy();
    StmtNumList stmt_nums_true_get_all_followed_by =
        true_pkb.GetAllFollowedBy();

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

    StmtNumList stmt_num_test_get_follows = test_pkb.GetFollows(1);
    StmtNumList stmt_num_true_get_follows = true_pkb.GetFollows(1);

    iter_1 = stmt_num_test_get_follows.begin();
    iter_2 = stmt_num_true_get_follows.begin();

    // Should have 1 follows stmt_num
    Assert::AreEqual(*iter_1, *iter_2);

    //*************************************

    StmtNumList stmt_num_test_get_follows_t = test_pkb.GetFollowsT(1);
    StmtNumList stmt_num_true_get_follows_t = true_pkb.GetFollowsT(1);

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
    true_pkb.InsertParent(2, 3);
    true_pkb.InsertParent(2, 4);
    true_pkb.InsertParent(2, 5);
    true_pkb.InsertParent(6, 7);

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

    StmtNumList test_child_of_stmt = test_pkb.GetChild(2);
    StmtNumList true_child_of_stmt = true_pkb.GetChild(2);

    iter_1 = test_child_of_stmt.begin();
    iter_2 = true_child_of_stmt.begin();

    // Should have three children for stmt_num 2
    Assert::AreEqual(*iter_1++, *iter_2++);
    Assert::AreEqual(*iter_1++, *iter_2++);
    Assert::AreEqual(*iter_1, *iter_2);

    //**********************************
    StmtNumList test_parent_of_stmt = test_pkb.GetParent(7);
    StmtNumList true_parent_of_stmt = true_pkb.GetParent(7);

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
    true_pkb.InsertParent(5, 6);
    true_pkb.InsertParent(5, 7);
    true_pkb.InsertParent(2, 3);
    true_pkb.InsertParent(2, 4);
    true_pkb.InsertParent(2, 5);
    true_pkb.InsertParent(2, 8);

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

    StmtNumList test_child_of_stmt = test_pkb.GetChild(5);
    StmtNumList true_child_of_stmt = true_pkb.GetChild(5);

    iter_1 = test_child_of_stmt.begin();
    iter_2 = true_child_of_stmt.begin();

    // Should have two children for stmt_num 2
    Assert::AreEqual(*iter_1++, *iter_2++);
    Assert::AreEqual(*iter_1, *iter_2);

    //**********************************

    StmtNumList test_parent_of_stmt = test_pkb.GetParent(5);
    StmtNumList true_parent_of_stmt = true_pkb.GetParent(5);

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
        "f = f + 1;"           // 9
        "}"
        "}\n"
        "} else {\n"
        "k = 3;\n"  // 10
        "}\n"
        "}\n";

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    PKB true_pkb = PKB();
    true_pkb.InsertParent(8, 9);
    true_pkb.InsertParent(5, 6);
    true_pkb.InsertParent(5, 7);
    true_pkb.InsertParent(5, 8);
    true_pkb.InsertParent(2, 3);
    true_pkb.InsertParent(2, 4);
    true_pkb.InsertParent(2, 5);
    true_pkb.InsertParent(2, 10);

    true_pkb.InsertParentT(5, 9);
    true_pkb.InsertParentT(2, 5);
    true_pkb.InsertParentT(2, 6);
    true_pkb.InsertParentT(2, 7);
    true_pkb.InsertParentT(2, 8);
    true_pkb.InsertParentT(2, 9);
    //*******************************

    StmtNumList test_parent_t = test_pkb.GetParentT(9);
    StmtNumList true_parent_t = true_pkb.GetParentT(9);

    StmtNumList::iterator iter_1 = test_parent_t.begin();
    StmtNumList::iterator iter_2 = true_parent_t.begin();

    // Should only have three parents*
    Assert::AreEqual(*iter_1++, *iter_2++);
    Assert::AreEqual(*iter_1++, *iter_2++);
    Assert::AreEqual(*iter_1, *iter_2);

    //*******************************

    StmtNumList test_child_t = test_pkb.GetChildT(2);
    StmtNumList true_child_t = true_pkb.GetChildT(2);

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
    true_pkb.InsertModifiesS(1, "a");
    true_pkb.InsertUsesS(1, "b");
    true_pkb.InsertModifiesS(2, "c");
    true_pkb.InsertUsesS(2, "d");
    true_pkb.InsertUsesS(2, "e");
    true_pkb.InsertUsesS(2, "f");
    true_pkb.InsertModifiesS(3, "j");
    true_pkb.InsertUsesS(3, "k");
    true_pkb.InsertUsesS(3, "l");

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

    VarIndexList test_used_var_of_stmt = test_pkb.GetUsedVarS(3);
    VarIndexList true_used_var_of_stmt = true_pkb.GetUsedVarS(3);

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
        "k = 3;\n"      // 8
        "print print;"  // 9
        "}\n"
        "}\n";

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    // since recursive descent, do parent last
    PKB true_pkb = PKB();
    true_pkb.InsertModifiesS(1, "a");
    true_pkb.InsertUsesS(1, "b");

    true_pkb.InsertModifiesS(3, "e");
    true_pkb.InsertUsesS(3, "f");

    true_pkb.InsertModifiesS(4, "asd");
    true_pkb.InsertUsesS(4, "ddd");

    true_pkb.InsertModifiesS(6, "a");
    true_pkb.InsertUsesS(6, "a");

    true_pkb.InsertModifiesS(7, "k");
    true_pkb.InsertUsesS(7, "k");

    true_pkb.InsertUsesS(5, "a");
    true_pkb.InsertUsesS(5, "a");
    true_pkb.InsertUsesS(5, "k");
    true_pkb.InsertModifiesS(5, "a");
    true_pkb.InsertModifiesS(5, "k");

    true_pkb.InsertModifiesS(8, "k");

    true_pkb.InsertUsesS(9, "print");

    true_pkb.InsertUsesS(2, "a");
    true_pkb.InsertUsesS(2, "b");
    true_pkb.InsertUsesS(2, "f");
    true_pkb.InsertUsesS(2, "ddd");
    true_pkb.InsertUsesS(2, "a");
    true_pkb.InsertUsesS(2, "k");
    true_pkb.InsertUsesS(2, "print");
    true_pkb.InsertModifiesS(2, "e");
    true_pkb.InsertModifiesS(2, "asd");
    true_pkb.InsertModifiesS(2, "a");
    true_pkb.InsertModifiesS(2, "k");
    true_pkb.InsertModifiesS(2, "k");

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
    StmtNumList test_used_var_of_stmt = test_pkb.GetUsedVarS(2);
    StmtNumList true_used_var_of_stmt = true_pkb.GetUsedVarS(2);

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
    true_pkb.InsertModifiesS(1, "a");
    true_pkb.InsertUsesS(1, "b");
    true_pkb.InsertModifiesS(2, "while");
    true_pkb.InsertUsesS(2, "k");
    true_pkb.InsertModifiesS(3, "if");
    true_pkb.InsertUsesS(3, "x");
    true_pkb.InsertUsesS(3, "y");
    true_pkb.InsertUsesS(3, "z");
    true_pkb.InsertModifiesS(4, "if");
    true_pkb.InsertModifiesS(5, "if");

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

    StmtNumList test_modified_stmt_get_var = test_pkb.GetModifiedVarS(2);
    StmtNumList true_modified_stmt_get_var = true_pkb.GetModifiedVarS(2);

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
    true_pkb.InsertModifiesS(1, "a");
    true_pkb.InsertUsesS(1, "b");

    true_pkb.InsertModifiesS(4, "if");

    true_pkb.InsertModifiesS(5, "z");
    true_pkb.InsertUsesS(5, "a");

    true_pkb.InsertModifiesS(6, "t");
    true_pkb.InsertUsesS(3, "abc");

    true_pkb.InsertModifiesS(8, "w");

    true_pkb.InsertModifiesS(9, "a");
    true_pkb.InsertUsesS(9, "b");

    true_pkb.InsertModifiesS(10, "x");

    true_pkb.InsertModifiesS(7, "w");
    true_pkb.InsertModifiesS(7, "a");
    true_pkb.InsertModifiesS(7, "x");
    true_pkb.InsertUsesS(7, "a");
    true_pkb.InsertUsesS(7, "b");

    true_pkb.InsertModifiesS(3, "if");
    true_pkb.InsertModifiesS(3, "z");
    true_pkb.InsertModifiesS(3, "t");
    true_pkb.InsertModifiesS(3, "w");
    true_pkb.InsertModifiesS(3, "a");
    true_pkb.InsertModifiesS(3, "x");
    true_pkb.InsertUsesS(3, "a");
    true_pkb.InsertUsesS(3, "abc");
    true_pkb.InsertUsesS(3, "b");

    true_pkb.InsertModifiesS(2, "if");
    true_pkb.InsertModifiesS(2, "w");
    true_pkb.InsertModifiesS(2, "z");
    true_pkb.InsertModifiesS(2, "t");
    true_pkb.InsertModifiesS(2, "a");
    true_pkb.InsertModifiesS(2, "x");
    true_pkb.InsertUsesS(2, "x");
    true_pkb.InsertUsesS(2, "y");
    true_pkb.InsertUsesS(2, "a");
    true_pkb.InsertUsesS(2, "abc");
    true_pkb.InsertUsesS(2, "b");

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

    StmtNumList test_modified_stmt_get_var = test_pkb.GetModifiedVarS(2);
    StmtNumList true_modified_stmt_get_var = true_pkb.GetModifiedVarS(2);

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
        "a = b;\n"             // 1
        "if (x < y) then {\n"  // 2
        "print x;"             // 3
        "read t;"              // 4
        "while (a == 1) {\n"   // 5
        "if = 2;\n"            // 6
        "z = a +1;\n"          // 7
        "}"
        "} else {"
        "print z;"  // 8
        "} "
        "while (j > f) {"      // 9
        "s = s * l;"           // 10
        "asd = asd / 2;"       // 11
        "if (a < 100) then {"  // 12
        "x = 100;"             // 13
        "} else {"
        "s = 500;"  // 14
        "}"
        "}"
        "a = 1;"  // 15
        "}";

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    vector<StmtNumList> list_of_adj_list;
    list_of_adj_list.push_back(StmtNumList());

    list_of_adj_list.push_back(StmtNumList({2}));       // stmt1 adj_list
    list_of_adj_list.push_back(StmtNumList({3, 8}));    // stmt2 adj_list
    list_of_adj_list.push_back(StmtNumList({4}));       // stmt3 adj_list
    list_of_adj_list.push_back(StmtNumList({5}));       // stmt4 adj_list
    list_of_adj_list.push_back(StmtNumList({6, 9}));    // stmt5 adj_list
    list_of_adj_list.push_back(StmtNumList({7}));       // stmt6 adj_list
    list_of_adj_list.push_back(StmtNumList({5}));       // stmt7 adj_list
    list_of_adj_list.push_back(StmtNumList({9}));       // stmt8 adj_list
    list_of_adj_list.push_back(StmtNumList({10, 15}));  // stmt9 adj_list
    list_of_adj_list.push_back(StmtNumList({11}));      // stmt10 adj_list
    list_of_adj_list.push_back(StmtNumList({12}));      // stmt11 adj_list
    list_of_adj_list.push_back(StmtNumList({13, 14}));  // stmt12 adj_list
    list_of_adj_list.push_back(StmtNumList({9}));       // stmt13 adj_list
    list_of_adj_list.push_back(StmtNumList({9}));       // stmt14 adj_list

    CFG* test_cfg = test_pkb.GetCFG("one");

    for (unsigned i = 1; i < list_of_adj_list.size(); i++) {
      StmtNumList test_adj_list = test_cfg->GetNeighboursList(i);
      StmtNumList true_adj_list = list_of_adj_list.at(i);

      Assert::AreEqual(test_adj_list.size(), true_adj_list.size());
      for (size_t j = 0; j < test_adj_list.size(); j++) {
        Assert::AreEqual(test_adj_list.at(j), true_adj_list.at(j));
      }
    }
  }

  TEST_METHOD(TestCfgIfInIfStatements) {
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    string program =
        "procedure one {\n"
        "a = b;\n"              // 1
        "if (x < y) then {\n"   // 2
        "if (a == 1) then {\n"  // 3
        "if = 2;\n"             // 4
        "z = a +1;\n"           // 5
        "} else {"
        "a = a + 1;"  // 6
        "}"
        "x = x+ 1;"  // 7
        "} else {\n"
        "t = abc;\n"            // 8
        "if (a == b) then {\n"  // 9
        "w = 1 + 2;\n"          // 10
        "a = b + 1;\n"          // 11
        "} else {\n"
        "a = 1;"  // 12
        "}\n"
        "}\n"
        "x = 1 + y;\n"  // 13
        "}";

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    vector<StmtNumList> list_of_adj_list;
    list_of_adj_list.push_back(StmtNumList());

    list_of_adj_list.push_back(StmtNumList({2}));       // stmt1 adj_list
    list_of_adj_list.push_back(StmtNumList({3, 8}));    // stmt2 adj_list
    list_of_adj_list.push_back(StmtNumList({4, 6}));    // stmt3 adj_list
    list_of_adj_list.push_back(StmtNumList({5}));       // stmt4 adj_list
    list_of_adj_list.push_back(StmtNumList({7}));       // stmt5 adj_list
    list_of_adj_list.push_back(StmtNumList({7}));       // stmt6 adj_list
    list_of_adj_list.push_back(StmtNumList({13}));      // stmt7 adj_list
    list_of_adj_list.push_back(StmtNumList({9}));       // stmt8 adj_list
    list_of_adj_list.push_back(StmtNumList({10, 12}));  // stmt9 adj_list
    list_of_adj_list.push_back(StmtNumList({11}));      // stmt10 adj_list
    list_of_adj_list.push_back(StmtNumList({13}));      // stmt11 adj_list
    list_of_adj_list.push_back(StmtNumList({13}));      // stmt12 adj_list

    CFG* test_cfg = test_pkb.GetCFG("one");

    for (unsigned i = 1; i < list_of_adj_list.size(); i++) {
      StmtNumList test_adj_list = test_cfg->GetNeighboursList(i);
      StmtNumList true_adj_list = list_of_adj_list.at(i);

      Assert::AreEqual(test_adj_list.size(), true_adj_list.size());
      for (size_t j = 0; j < test_adj_list.size(); j++) {
        Assert::AreEqual(test_adj_list.at(j), true_adj_list.at(j));
      }
    }
  }

  TEST_METHOD(TestCfgIfInIfInIfStatements) {
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    string program =
        "procedure one {\n"
        "a = b;\n"             // 1
        "if (x < y) then {\n"  // 2
        "a = 1;"               // 3
        "} else {\n"
        "if (a == 1) then {\n"  // 4
        "if = 2;\n"             // 5
        "z = a +1;\n"           // 6
        "} else {"
        "a = a + 1;"          // 7
        "if (t == 1) then {"  // 8
        "print x;"            // 9
        "} else {"
        "a = z + x;"  // 10
        "}"
        "}"
        "}\n"
        "x = 1 + y;\n"  // 11
        "}";

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    vector<StmtNumList> list_of_adj_list;
    list_of_adj_list.push_back(StmtNumList());

    list_of_adj_list.push_back(StmtNumList({2}));      // stmt1 adj_list
    list_of_adj_list.push_back(StmtNumList({3, 4}));   // stmt2 adj_list
    list_of_adj_list.push_back(StmtNumList({11}));     // stmt3 adj_list
    list_of_adj_list.push_back(StmtNumList({5, 7}));   // stmt4 adj_list
    list_of_adj_list.push_back(StmtNumList({6}));      // stmt5 adj_list
    list_of_adj_list.push_back(StmtNumList({11}));     // stmt6 adj_list
    list_of_adj_list.push_back(StmtNumList({8}));      // stmt7 adj_list
    list_of_adj_list.push_back(StmtNumList({9, 10}));  // stmt8 adj_list
    list_of_adj_list.push_back(StmtNumList({11}));     // stmt9 adj_list
    list_of_adj_list.push_back(StmtNumList({11}));     // stmt10 adj_list

    CFG* test_cfg = test_pkb.GetCFG("one");

    for (unsigned i = 1; i < list_of_adj_list.size(); i++) {
      StmtNumList test_adj_list = test_cfg->GetNeighboursList(i);
      StmtNumList true_adj_list = list_of_adj_list.at(i);

      Assert::AreEqual(test_adj_list.size(), true_adj_list.size());
      for (size_t j = 0; j < test_adj_list.size(); j++) {
        Assert::AreEqual(test_adj_list.at(j), true_adj_list.at(j));
      }
    }
  }

  TEST_METHOD(TestProcessCalls) {
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    string program =
        "procedure one {\n"
        "call two;}\n"  // 1
        "procedure two {\n"
        "call three;"    // 2
        "call four;}\n"  // 3
        "procedure three {\n"
        "call five;}\n"  // 4
        "procedure four {\n"
        "call three;}\n"  // 5
        "procedure five {\n"
        "thing = what;}";  // 6

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    PKB true_pkb = PKB();
    StmtNumList expected_stmts = StmtNumList{1, 2, 3, 4, 5};
    StmtNumList call_stmts = test_pkb.GetAllCallStmt();
    Assert::IsTrue(call_stmts.size() == 5);
    Assert::IsTrue(call_stmts == expected_stmts);
    ProcNameList callers = test_pkb.GetAllCallerName();
    ProcNameList expected_callers = ProcNameList{"one", "two", "three", "four"};
    Assert::IsTrue(callers.size() == 4);
    Assert::IsTrue(callers == expected_callers);

    ProcNameList callees = test_pkb.GetAllCalleeName();
    ProcNameList expected_callees =
        ProcNameList{"two", "three", "four", "five"};
    Assert::IsTrue(callees.size() == 4);
    Assert::IsTrue(callees == expected_callees);

    ProcIndexList proc_id_order = test_pkb.GetToposortedCalls();
    ProcNameList proc_order;
    for (auto id : proc_id_order) {
      proc_order.push_back(test_pkb.GetProcName(id));
    }
    ProcNameList expected_order =
        ProcNameList{"five", "three", "four", "two", "one"};
    Assert::IsTrue(proc_order == expected_order);
  }
  TEST_METHOD(TestCallsTWithOneCallPerProc) {
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    string program =
        "procedure one {\n"
        "call two;}\n"  // 1
        "procedure two {\n"
        "call four;}\n"  // 2
        "procedure three {\n"
        "call five;}\n"  // 3
        "procedure four {\n"
        "call three;}\n"  // 4
        "procedure five {\n"
        "thing = what;}";  // 5

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    // Test direct and indirect callees of procedure "one"
    ProcIndexList callees_1 = test_pkb.GetCalleeT("one");
    ProcIndexSet actual_set_1(callees_1.begin(), callees_1.end());
    ProcIndexSet expected_callees_1{1, 2, 3, 4};
    Assert::IsTrue(callees_1.size() == 4);
    Assert::IsTrue(actual_set_1 == expected_callees_1);

    // Test direct and indirect callers of procedure "five"
    ProcIndexList callees_2 = test_pkb.GetCallerT("five");
    ProcIndexSet actual_set_2(callees_2.begin(), callees_2.end());
    ProcIndexSet expected_callees_2{0, 1, 2, 3};
    Assert::IsTrue(callees_2.size() == 4);
    Assert::IsTrue(actual_set_2 == expected_callees_2);
  }
  TEST_METHOD(TestCallsTWithMultipleCallPerProc) {
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    string program =
        "procedure one {\n"
        "call two;\n"
        "call three;}\n"
        "procedure two {\n"
        "call four;}\n"
        "procedure three {\n"
        "call five;}\n"
        "procedure four {\n"
        "call three;}\n"
        "procedure five {\n"
        "call me;\n"
        "call you;}\n"
        "procedure me {\n"
        "print j;}\n"
        "procedure you {\n"
        "print x;}\n"
        "procedure them {"
        "call you;\n}";

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);

    // Test direct and indirect callees of procedure "one"
    // If this part fails, it's because the indices don't match.
    ProcIndexList callees_1 = test_pkb.GetCalleeT("one");
    // zero based indexes so all the indexes - 1
    ProcIndexList expected_callees_1 = ProcIndexList{1, 2, 3, 4, 5, 6};
    Assert::IsTrue(callees_1.size() == 6);
    Assert::IsTrue(callees_1 == expected_callees_1);

    // Test direct and indirect callers of procedure "you"
    ProcIndexList callees_2 = test_pkb.GetCallerT("you");
    // zero based indexes so all the indexes - 1
    ProcIndexList expected_callees_2 = ProcIndexList{4, 7, 0, 1, 2, 3};
    Assert::IsTrue(callees_2.size() == 6);
    Assert::IsTrue(callees_2 == expected_callees_2);
  }

  TEST_METHOD(TestPopulateAllNextPairs) {
    PKB test_pkb = PKB();
    test_pkb.InsertProcName(kProcName1);
    test_pkb.InsertNext(kProcName1, 1, 2);
    test_pkb.InsertNext(kProcName1, 2, 3);
    test_pkb.InsertNext(kProcName1, 2, 4);

    DesignExtractor de = DesignExtractor(&test_pkb);

    de.PopulateAllNextPairs();

    StmtNumPairList result1 = test_pkb.GetAllNextPairs();
    Assert::IsTrue(result1.size() == 3);
    StmtNumPairList::iterator iter = result1.begin();
    Assert::AreEqual(1, (*iter).first);
    Assert::AreEqual(2, (*iter).second);
    iter++;
    Assert::AreEqual(2, (*iter).first);
    Assert::AreEqual(3, (*iter).second);
    iter++;
    Assert::AreEqual(2, (*iter).first);
    Assert::AreEqual(4, (*iter).second);
  }

  TEST_METHOD(TestPopulateProgramCFG) {
    PKB test_pkb = PKB();
    Parser parser = Parser(&test_pkb);
    string program =
        " procedure one {"
        "while(a == 1) {"      // 1
        " a = 1;"              // 2
        " if (x == b) then {"  // 3
        "t = t + 1;"           // 4
        "print x;"             // 5
        "} else {"
        "t = t * 2;"  // 6
        "}"
        "}"
        "call two;"                 // 7
        "g = a;"                    // 8
        "f = t + a + c;"            // 9
        "read f;"                   // 10
        "w = f;"                    // 11
        "affects = g + f + a + t;"  // 12
        "}"
        "procedure two {"
        "read a;"  // 13
        "a = b;"   // 14
        "}";

    TokenList tokenized_program = Tokenizer::Tokenize(program);
    parser.Parse(tokenized_program);
    DesignExtractor de = DesignExtractor(&test_pkb);

    de.PopulateProgramCFG();

    CFG* actual_cfg = test_pkb.GetProgramCFG();

    //stmt#7 should be linked to root of proc "two" and not stmt#8
    Assert::IsTrue(actual_cfg->GetNeighboursSet(7) == VertexSet{13});
    // expect terminal node (stmt#14) of procedure two be to linked to stmt#8
    Assert::IsTrue(actual_cfg->GetNeighboursSet(14) == VertexSet{8});
  }
};
}  // namespace ParserPKBTests
