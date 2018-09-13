#include "stdafx.h"
#include "CppUnitTest.h"
#include "expression_helper.h"
#include "tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using tt = Tokenizer::TokenType;

namespace FrontendTests {
TEST_CLASS(TestExpressionHelper){
 public :

	TEST_METHOD(TestToPostfix){
			// sum of 2 variable
			TokenList sum_two_var = {{tt::kName, "a"},
																												{tt::kOperator, "+"},
																												{tt::kName, "b"}};
			TokenList result_1 = ExpressionHelper::ToPostfix(sum_two_var);
			TokenList expected_result_1 = {{tt::kName, "a"},
																																{tt::kName, "b"},
																																{tt::kOperator, "+"}};
			Assert::IsTrue(result_1 == expected_result_1);
			
			// product of 2 variable
			TokenList product_two_var = {{tt::kName, "a"},
																																{tt::kOperator, "*"},
																																{tt::kName, "b"}};
			TokenList result_2 = ExpressionHelper::ToPostfix(product_two_var);
			TokenList expected_result_2 = {{tt::kName, "a"},
																																{tt::kName, "b"},
																																{tt::kOperator, "*"}};
			Assert::IsTrue(result_2 == expected_result_2);

			// subtraction of 2 variable
   TokenList subtraction_two_var = {{tt::kName, "a"},
                                {tt::kOperator, "-"},
                                {tt::kName, "b"}};
   TokenList result_4 =
       ExpressionHelper::ToPostfix(subtraction_two_var);
   TokenList expected_result_4 = {{tt::kName, "a"},
                                {tt::kName, "b"},
                                {tt::kOperator, "-"}};
   Assert::IsTrue(result_4 == expected_result_4);


			// a / b + c
   TokenList divide_then_plus = {{tt::kName, "a"},
                                {tt::kOperator, "/"},
                                {tt::kName, "b"},
                                {tt::kOperator, "+"},
                                {tt::kName, "c"}};
   TokenList result_5 =
       ExpressionHelper::ToPostfix(divide_then_plus);
   TokenList expected_result_5 = {{tt::kName, "a"},
                                {tt::kName, "b"},
                                {tt::kOperator, "/"},
                                {tt::kName, "c"},
                                {tt::kOperator, "+"}};
   Assert::IsTrue(result_5 == expected_result_5);

			// a - b * c
   TokenList minus_then_multiply = {{tt::kName, "a"},
                                {tt::kOperator, "-"},
                                {tt::kName, "b"},
                                {tt::kOperator, "*"},
                                {tt::kName, "c"}};
   TokenList result_6 =
       ExpressionHelper::ToPostfix(minus_then_multiply);
   TokenList expected_result_6 = {{tt::kName, "a"},
                                {tt::kName, "b"},
                                {tt::kName, "c"},
                                {tt::kOperator, "*"},
                                {tt::kOperator, "-"}};
   Assert::IsTrue(result_6 == expected_result_6);

			// (a - b) * c
   TokenList paranthesize_var_test_1 = {{tt::kOpenParen, "("},
																															 {tt::kName, "a"},
                                {tt::kOperator, "-"},
                                {tt::kName, "b"},
                                {tt::kCloseParen, ")"},
                                {tt::kOperator, "*"},
                                {tt::kName, "c"}};
   TokenList result_7 =
       ExpressionHelper::ToPostfix(paranthesize_var_test_1);
   TokenList expected_result_7 = {{tt::kName, "a"},
                                {tt::kName, "b"},
                                {tt::kOperator, "-"},
                                {tt::kName, "c"},
                                {tt::kOperator, "*"}};
   Assert::IsTrue(result_7 == expected_result_7);

			// a * (b - c)
   TokenList paranthesize_var_test_2 = {{tt::kName, "a"},
                                {tt::kOperator, "-"},
																																{tt::kOpenParen, "("},
                                {tt::kName, "b"},
                                {tt::kOperator, "*"},
                                {tt::kName, "c"},
																																{tt::kCloseParen, ")"}};
   TokenList result_8 =
       ExpressionHelper::ToPostfix(paranthesize_var_test_2);
   TokenList expected_result_8 = {{tt::kName, "a"},
                                {tt::kName, "b"},
                                {tt::kName, "c"},
                                {tt::kOperator, "*"},
																																{tt::kOperator, "-"}};
   Assert::IsTrue(result_8 == expected_result_8);

			// (a - (3 + 4) / d) * (b - c)
   TokenList paranthesize_var_test_3 = {{tt::kOpenParen, "("}, 
																																{tt::kName, "a"},
                                {tt::kOperator, "-"},
																																{tt::kOpenParen, "("},
                                {tt::kDigit, "3"},
                                {tt::kOperator, "+"},
                                {tt::kDigit, "4"},
																																{tt::kCloseParen, ")"},
																																{tt::kOperator, "/"},
																																{tt::kName, "d"},
																																{tt::kCloseParen, ")"},
																																{tt::kOperator, "*"},
																																{tt::kOpenParen, "("},
																																{tt::kName, "b"},
																																{tt::kOperator, "-"},
																																{tt::kName, "c"},
																																{tt::kCloseParen, ")"}};
   TokenList result_9 =
       ExpressionHelper::ToPostfix(paranthesize_var_test_3);
   TokenList expected_result_9 = {{tt::kName, "a"},
                                {tt::kDigit, "3"},
                                {tt::kDigit, "4"},
                                {tt::kOperator, "+"},
                                {tt::kName, "d"},
                                {tt::kOperator, "/"},
																																{tt::kOperator, "-"},
																																{tt::kName, "b"},
																																{tt::kName, "c"},
																																{tt::kOperator, "-"},
																																{tt::kOperator, "*"}};

   // (a + b * 4 + (c - d % (f + g / (h - k ) ) ) )
   TokenList paranthesize_var_test_4 = {{tt::kOpenParen, "("}, 
																																{tt::kName, "a"},
                                {tt::kOperator, "+"},
																																{tt::kName, "b"},
                                {tt::kOperator, "*"},
                                {tt::kDigit, "4"},
                                {tt::kOperator, "+"},
																																{tt::kOpenParen, "("},
																																{tt::kName, "c"},
																																{tt::kOperator, "-"},
																																{tt::kName, "d"},
																																{tt::kOperator, "%"},
																																{tt::kOpenParen, "("},
																																{tt::kName, "f"},
																																{tt::kOperator, "+"},
																																{tt::kName, "g"},
																																{tt::kOperator, "/"},
																																{tt::kOpenParen, "("},
																																{tt::kName, "h"},
																																{tt::kOperator, "-"},
																																{tt::kName, "k"},
																																{tt::kCloseParen, ")"},
																																{tt::kCloseParen, ")"},
																																{tt::kCloseParen, ")"},
																																{tt::kCloseParen, ")"}};
   TokenList result_10 =
       ExpressionHelper::ToPostfix(paranthesize_var_test_4);
   TokenList expected_result_10 = {{tt::kName, "a"},
                                {tt::kName, "b"},
                                {tt::kDigit, "4"},
                                {tt::kOperator, "*"},
                                {tt::kOperator, "+"},
                                {tt::kName, "c"},
																																{tt::kName, "d"},
																																{tt::kName, "f"},
																																{tt::kName, "g"},
																																{tt::kName, "h"},
																																{tt::kName, "k"},
																																{tt::kOperator, "-"},
																																{tt::kOperator, "/"},
																																{tt::kOperator, "+"},
																																{tt::kOperator, "%"},
																																{tt::kOperator, "-"},
																																{tt::kOperator, "+"}};
   Assert::IsTrue(result_10 == expected_result_10);		
	}

};
}