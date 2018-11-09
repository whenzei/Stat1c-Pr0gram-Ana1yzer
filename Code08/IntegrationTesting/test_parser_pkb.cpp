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

    StmtNumSet stmt_nums_test_get_all_followed_by =
        test_pkb.GetAllFollowedBy();
    StmtNumSet stmt_nums_true_get_all_followed_by =
        true_pkb.GetAllFollowedBy();

    Assert::IsTrue(stmt_nums_test_get_all_followed_by ==
                   stmt_nums_true_get_all_followed_by);

    //***********************************

    StmtNumSet stmt_nums_test_get_all_follows = test_pkb.GetAllFollows();
    StmtNumSet stmt_nums_true_get_all_follows = true_pkb.GetAllFollows();

    Assert::IsTrue(stmt_nums_test_get_all_follows == stmt_nums_true_get_all_follows);

    //*************************************

    StmtNumSet stmt_num_test_get_follows = test_pkb.GetFollows(2);
    StmtNumSet stmt_num_true_get_follows = true_pkb.GetFollows(2);

    Assert::IsTrue(stmt_num_test_get_follows == stmt_num_true_get_follows);

    //*************************************

    StmtNumSet stmt_num_test_get_follows_t = test_pkb.GetFollowsT(1);
    StmtNumSet stmt_num_true_get_follows_t = true_pkb.GetFollowsT(1);

    Assert::IsTrue(stmt_num_test_get_follows_t == stmt_num_true_get_follows_t);

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

    StmtNumSet stmt_nums_test_get_all_followed_by =
        test_pkb.GetAllFollowedBy();
    StmtNumSet stmt_nums_true_get_all_followed_by =
        true_pkb.GetAllFollowedBy();

    Assert::IsTrue(stmt_nums_test_get_all_followed_by ==
                   stmt_nums_true_get_all_followed_by);

    //**************************************

    StmtNumSet stmt_nums_test_get_all_follows = test_pkb.GetAllFollows();
    StmtNumSet stmt_nums_true_get_all_follows = true_pkb.GetAllFollows();

    Assert::IsTrue(stmt_nums_test_get_all_follows ==
                   stmt_nums_true_get_all_follows);

    //**************************************

    StmtNumSet stmt_num_test_get_follows = test_pkb.GetFollows(1);
    StmtNumSet stmt_num_true_get_follows = true_pkb.GetFollows(1);

    Assert::IsTrue(stmt_num_test_get_follows == stmt_num_test_get_follows);

    //*************************************

    StmtNumSet stmt_num_test_get_follows_t = test_pkb.GetFollowsT(1);
    StmtNumSet stmt_num_true_get_follows_t = true_pkb.GetFollowsT(1);

    Assert::IsTrue(stmt_num_test_get_follows_t == stmt_num_true_get_follows_t);
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

    StmtNumSet test_all_parents = test_pkb.GetAllParent();
    StmtNumSet true_all_parents = true_pkb.GetAllParent();

    Assert::IsTrue(test_all_parents == true_all_parents);

    //*******************************

    StmtNumSet test_all_children = test_pkb.GetAllChild();
    StmtNumSet true_all_children = true_pkb.GetAllChild();

    Assert::IsTrue(test_all_children == true_all_children);

    //*********************************

    StmtNumSet test_child_of_stmt = test_pkb.GetChild(2);
    StmtNumSet true_child_of_stmt = true_pkb.GetChild(2);

    Assert::IsTrue(test_child_of_stmt == true_child_of_stmt);

    //**********************************
    StmtNumSet test_parent_of_stmt = test_pkb.GetParent(7);
    StmtNumSet true_parent_of_stmt = true_pkb.GetParent(7);

    Assert::IsTrue(test_parent_of_stmt == true_parent_of_stmt);
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

    StmtNumSet test_all_parents = test_pkb.GetAllParent();
    StmtNumSet true_all_parents = true_pkb.GetAllParent();

    Assert::IsTrue(test_all_parents == true_all_parents);

    //*******************************

    StmtNumSet test_all_children = test_pkb.GetAllChild();
    StmtNumSet true_all_children = true_pkb.GetAllChild();

    Assert::IsTrue(test_all_children == true_all_children);

    //*********************************

    StmtNumSet test_child_of_stmt = test_pkb.GetChild(5);
    StmtNumSet true_child_of_stmt = true_pkb.GetChild(5);

    Assert::IsTrue(test_child_of_stmt == true_child_of_stmt);

    //**********************************

    StmtNumSet test_parent_of_stmt = test_pkb.GetParent(5);
    StmtNumSet true_parent_of_stmt = true_pkb.GetParent(5);

    Assert::IsTrue(test_parent_of_stmt == true_parent_of_stmt);
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

    StmtNumSet test_parent_t = test_pkb.GetParentT(9);
    StmtNumSet true_parent_t = true_pkb.GetParentT(9);

    Assert::IsTrue(test_parent_t == true_parent_t);

    //*******************************

    StmtNumSet test_child_t = test_pkb.GetChildT(2);
    StmtNumSet true_child_t = true_pkb.GetChildT(2);

    Assert::IsTrue(test_child_t == true_child_t);

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

    StmtNumSet test_all_using_stmts = test_pkb.GetAllUsingStmt();
    StmtNumSet true_all_using_stmts = true_pkb.GetAllUsingStmt();

    Assert::IsTrue(test_all_using_stmts == true_all_using_stmts);

    //*******************************

    VarIndexSet test_used_var_of_stmt = test_pkb.GetUsedVarS(3);
    VarIndexSet true_used_var_of_stmt = true_pkb.GetUsedVarS(3);

    Assert::IsTrue(test_used_var_of_stmt == true_used_var_of_stmt);

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

    StmtNumSet test_all_using_stmts = test_pkb.GetAllUsingStmt();
    StmtNumSet true_all_using_stmts = true_pkb.GetAllUsingStmt();

    Assert::IsTrue(test_all_using_stmts == true_all_using_stmts);

    //*******************************
    StmtNumSet test_used_var_of_stmt = test_pkb.GetUsedVarS(2);
    StmtNumSet true_used_var_of_stmt = true_pkb.GetUsedVarS(2);

    Assert::IsTrue(test_used_var_of_stmt == true_used_var_of_stmt);

    //*******************************
    StmtNumSet test_used_vars_proc = test_pkb.GetUsedVarP("one");
    StmtNumSet true_used_vars_proc = true_pkb.GetUsedVarP("one");

    Assert::IsTrue(test_used_vars_proc == true_used_vars_proc);

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

    StmtNumSet test_all_modified_stmts = test_pkb.GetAllModifyingS();
    StmtNumSet true_all_modified_stmts = true_pkb.GetAllModifyingS();

    Assert::IsTrue(test_all_modified_stmts == true_all_modified_stmts);

    //*******************************

    StmtNumSet test_modified_var_at_stmts = test_pkb.GetModifyingS("if");
    StmtNumSet true_modified_var_at_stmts = true_pkb.GetModifyingS("if");

    Assert::IsTrue(test_modified_var_at_stmts == true_modified_var_at_stmts);

    //*******************************

    StmtNumSet test_modified_stmt_get_var = test_pkb.GetModifiedVarS(2);
    StmtNumSet true_modified_stmt_get_var = true_pkb.GetModifiedVarS(2);

    Assert::IsTrue(test_modified_stmt_get_var == true_modified_stmt_get_var);

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

    StmtNumSet test_all_modified_stmts = test_pkb.GetAllModifyingS();
    StmtNumSet true_all_modified_stmts = true_pkb.GetAllModifyingS();

    Assert::IsTrue(test_all_modified_stmts == true_all_modified_stmts);

    //*******************************

    StmtNumSet test_modified_var_at_stmts = test_pkb.GetModifyingS("a");
    StmtNumSet true_modified_var_at_stmts = true_pkb.GetModifyingS("a");

    Assert::IsTrue(test_modified_var_at_stmts == true_modified_var_at_stmts);

    //*******************************

    StmtNumSet test_modified_stmt_get_var = test_pkb.GetModifiedVarS(2);
    StmtNumSet true_modified_stmt_get_var = true_pkb.GetModifiedVarS(2);

    Assert::IsTrue(test_modified_stmt_get_var == true_modified_stmt_get_var);

    //*******************************

    StmtNumSet test_modified_vars_proc = test_pkb.GetModifiedVarP("one");
    StmtNumSet true_modified_vars_proc = true_pkb.GetModifiedVarP("one");

    Assert::IsTrue(test_modified_vars_proc == true_modified_vars_proc);

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
    StmtNumSet expected_stmts = StmtNumSet{1, 2, 3, 4, 5};
    StmtNumSet call_stmts = test_pkb.GetAllCallStmt();
    Assert::IsTrue(call_stmts.size() == 5);
    Assert::IsTrue(expected_stmts == call_stmts);

    ProcNameSet callers = test_pkb.GetAllCallerName();
    ProcNameSet expected_callers = ProcNameSet{"one", "two", "three", "four"};
    Assert::IsTrue(callers.size() == 4);
    Assert::IsTrue(expected_callers == callers);

    ProcNameSet callees = test_pkb.GetAllCalleeName();
    ProcNameSet expected_callees =
      ProcNameSet{"two", "three", "four", "five"};
    Assert::IsTrue(callees.size() == 4);
    Assert::IsTrue(expected_callees == callees);

    ProcIndexList proc_id_order = test_pkb.GetToposortedCalls();
    ProcNameSet proc_order;
    for (auto id : proc_id_order) {
      proc_order.insert(test_pkb.GetProcName(id));
    }
    ProcNameSet expected_order =
      ProcNameSet{"five", "three", "four", "two", "one"};
    Assert::IsTrue(expected_order == proc_order);
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
    ProcIndexSet callees_1 = test_pkb.GetCalleeT("one");
    ProcIndexSet expected_callees_1{1, 2, 3, 4};
    Assert::IsTrue(callees_1.size() == 4);
    Assert::IsTrue(callees_1 == expected_callees_1);

    // Test direct and indirect callers of procedure "five"
    ProcIndexSet callees_2 = test_pkb.GetCallerT("five");
    ProcIndexSet expected_callees_2{0, 1, 2, 3};
    Assert::IsTrue(callees_2.size() == 4);
    Assert::IsTrue(callees_2 == expected_callees_2);
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
    ProcIndexSet callees_1 = test_pkb.GetCalleeT("one");
    // zero based indexes so all the indexes - 1
    ProcIndexSet expected_callees_1 = ProcIndexSet{1, 2, 3, 4, 5, 6};
    Assert::IsTrue(callees_1.size() == 6);
    Assert::IsTrue(callees_1 == expected_callees_1);

    // Test direct and indirect callers of procedure "you"
    ProcIndexSet callees_2 = test_pkb.GetCallerT("you");
    // zero based indexes so all the indexes - 1
    ProcIndexSet expected_callees_2 = ProcIndexSet{4, 7, 0, 1, 2, 3};
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

    StmtNumPairSet result = test_pkb.GetAllNextPairs();
    Assert::IsTrue(result.size() == 3);
    StmtNumPairSet expected_result = StmtNumPairSet{ {1,2}, {2,3}, {2,4} };
    Assert::IsTrue(expected_result == result);
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

    // Since program_cfg has no call statements,
    // expect the previous node of stmt#7 (stmt#1) to be linked to root of procedure two (stmt#8)
    // since #1 is a while statement, should have two neighbours
    Assert::IsTrue(actual_cfg->GetNeighboursSet(1) == VertexSet{2, 13});
    // expect terminal node (stmt#14) of procedure two be to linked to stmt#9
    Assert::IsTrue(actual_cfg->GetNeighboursSet(14) == VertexSet{8});

    // expect call statement 7 is be removed, 13 nodes left
    Assert::IsTrue(actual_cfg->GetNeighboursSet(7) == VertexSet());
    Assert::AreEqual(13, actual_cfg->GetSize());
  }
};
}  // namespace ParserPKBTests
