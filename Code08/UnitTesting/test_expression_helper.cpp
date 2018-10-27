#include "stdafx.h"
#include "CppUnitTest.h"
#include "expression_helper.h"
#include "tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using tt = Tokenizer::TokenType;

namespace FrontendTests {
TEST_CLASS(TestExpressionHelper) {
 public:
  TEST_METHOD(TestToPostfix) {
    // sum of 2 variable
    TokenList sum_two_var = {
        {tt::kName, "a"}, {tt::kOperator, "+"}, {tt::kName, "b"}};
    TokenList result_1 = ExpressionHelper::ToPostfix(sum_two_var);
    TokenList expected_result_1 = {
        {tt::kName, "a"}, {tt::kName, "b"}, {tt::kOperator, "+"}};
    Assert::IsTrue(result_1 == expected_result_1);

    // product of 2 variable
    TokenList product_two_var = {
        {tt::kName, "a"}, {tt::kOperator, "*"}, {tt::kName, "b"}};
    TokenList result_2 = ExpressionHelper::ToPostfix(product_two_var);
    TokenList expected_result_2 = {
        {tt::kName, "a"}, {tt::kName, "b"}, {tt::kOperator, "*"}};
    Assert::IsTrue(result_2 == expected_result_2);

    // subtraction of 2 variable
    TokenList subtraction_two_var = {
        {tt::kName, "a"}, {tt::kOperator, "-"}, {tt::kName, "b"}};
    TokenList result_4 = ExpressionHelper::ToPostfix(subtraction_two_var);
    TokenList expected_result_4 = {
        {tt::kName, "a"}, {tt::kName, "b"}, {tt::kOperator, "-"}};
    Assert::IsTrue(result_4 == expected_result_4);

    // a / b + c
    TokenList divide_then_plus = {{tt::kName, "a"},
                                  {tt::kOperator, "/"},
                                  {tt::kName, "b"},
                                  {tt::kOperator, "+"},
                                  {tt::kName, "c"}};
    TokenList result_5 = ExpressionHelper::ToPostfix(divide_then_plus);
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
    TokenList result_6 = ExpressionHelper::ToPostfix(minus_then_multiply);
    TokenList expected_result_6 = {{tt::kName, "a"},
                                   {tt::kName, "b"},
                                   {tt::kName, "c"},
                                   {tt::kOperator, "*"},
                                   {tt::kOperator, "-"}};
    Assert::IsTrue(result_6 == expected_result_6);

    // (a - b) * c
    TokenList paranthesize_var_test_1 = {
        {tt::kOpenParen, "("}, {tt::kName, "a"},       {tt::kOperator, "-"},
        {tt::kName, "b"},      {tt::kCloseParen, ")"}, {tt::kOperator, "*"},
        {tt::kName, "c"}};
    TokenList result_7 = ExpressionHelper::ToPostfix(paranthesize_var_test_1);
    TokenList expected_result_7 = {{tt::kName, "a"},
                                   {tt::kName, "b"},
                                   {tt::kOperator, "-"},
                                   {tt::kName, "c"},
                                   {tt::kOperator, "*"}};
    Assert::IsTrue(result_7 == expected_result_7);

    // a * (b - c)
    TokenList paranthesize_var_test_2 = {
        {tt::kName, "a"},      {tt::kOperator, "-"}, {tt::kOpenParen, "("},
        {tt::kName, "b"},      {tt::kOperator, "*"}, {tt::kName, "c"},
        {tt::kCloseParen, ")"}};
    TokenList result_8 = ExpressionHelper::ToPostfix(paranthesize_var_test_2);
    TokenList expected_result_8 = {{tt::kName, "a"},
                                   {tt::kName, "b"},
                                   {tt::kName, "c"},
                                   {tt::kOperator, "*"},
                                   {tt::kOperator, "-"}};
    Assert::IsTrue(result_8 == expected_result_8);

    // (a - (3 + 4) / d) * (b - c)
    TokenList paranthesize_var_test_3 = {
        {tt::kOpenParen, "("}, {tt::kName, "a"},       {tt::kOperator, "-"},
        {tt::kOpenParen, "("}, {tt::kDigit, "3"},      {tt::kOperator, "+"},
        {tt::kDigit, "4"},     {tt::kCloseParen, ")"}, {tt::kOperator, "/"},
        {tt::kName, "d"},      {tt::kCloseParen, ")"}, {tt::kOperator, "*"},
        {tt::kOpenParen, "("}, {tt::kName, "b"},       {tt::kOperator, "-"},
        {tt::kName, "c"},      {tt::kCloseParen, ")"}};
    TokenList result_9 = ExpressionHelper::ToPostfix(paranthesize_var_test_3);
    TokenList expected_result_9 = {
        {tt::kName, "a"},     {tt::kDigit, "3"},   {tt::kDigit, "4"},
        {tt::kOperator, "+"}, {tt::kName, "d"},    {tt::kOperator, "/"},
        {tt::kOperator, "-"}, {tt::kName, "b"},    {tt::kName, "c"},
        {tt::kOperator, "-"}, {tt::kOperator, "*"}};

    // (a + b * 4 + (c - d % (f + g / (h - k ) ) ) )
    TokenList paranthesize_var_test_4 = {
        {tt::kOpenParen, "("},  {tt::kName, "a"},       {tt::kOperator, "+"},
        {tt::kName, "b"},       {tt::kOperator, "*"},   {tt::kDigit, "4"},
        {tt::kOperator, "+"},   {tt::kOpenParen, "("},  {tt::kName, "c"},
        {tt::kOperator, "-"},   {tt::kName, "d"},       {tt::kOperator, "%"},
        {tt::kOpenParen, "("},  {tt::kName, "f"},       {tt::kOperator, "+"},
        {tt::kName, "g"},       {tt::kOperator, "/"},   {tt::kOpenParen, "("},
        {tt::kName, "h"},       {tt::kOperator, "-"},   {tt::kName, "k"},
        {tt::kCloseParen, ")"}, {tt::kCloseParen, ")"}, {tt::kCloseParen, ")"},
        {tt::kCloseParen, ")"}};
    TokenList result_10 = ExpressionHelper::ToPostfix(paranthesize_var_test_4);
    TokenList expected_result_10 = {
        {tt::kName, "a"},     {tt::kName, "b"},     {tt::kDigit, "4"},
        {tt::kOperator, "*"}, {tt::kOperator, "+"}, {tt::kName, "c"},
        {tt::kName, "d"},     {tt::kName, "f"},     {tt::kName, "g"},
        {tt::kName, "h"},     {tt::kName, "k"},     {tt::kOperator, "-"},
        {tt::kOperator, "/"}, {tt::kOperator, "+"}, {tt::kOperator, "%"},
        {tt::kOperator, "-"}, {tt::kOperator, "+"}};
    Assert::IsTrue(result_10 == expected_result_10);
  }  // namespace FrontendTests

  TEST_METHOD(TestIsPatternExactSuccess) {
    TokenList pattern_1 = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};
    TokenList pattern_2 = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsTrue(ExpressionHelper::IsPatternExact(pattern_1, pattern_2));
  }
  TEST_METHOD(TestIsPatternExactFail) {
    // Test pattern_1_pri length < pattern_1_sec length
    TokenList pattern_1_pri = {{tt::kName, "x"},     {tt::kName, "z"},
                               {tt::kName, "a"},     {tt::kName, "b"},
                               {tt::kOperator, "*"}, {tt::kOperator, "*"},
                               {tt::kName, "12"}};

    TokenList pattern_1_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternExact(pattern_1_pri, pattern_1_sec));

    // Test pattern_2_pri length > pattern_2_sec length
    TokenList pattern_2_pri = {{tt::kName, "x"},     {tt::kName, "z"},
                               {tt::kName, "a"},     {tt::kName, "b"},
                               {tt::kOperator, "*"}, {tt::kOperator, "*"},
                               {tt::kOperator, "*"}, {tt::kName, "12"},
                               {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_2_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternExact(pattern_2_pri, pattern_2_sec));

    // Test pattern_3_pri != pattern_3_sec
    TokenList pattern_3_pri = {{tt::kName, "x"},     {tt::kName, "z"},
                               {tt::kName, "a"},     {tt::kName, "b"},
                               {tt::kOperator, "*"}, {tt::kOperator, "*"},
                               {tt::kOperator, "*"}, {tt::kName, "12"},
                               {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_3_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternExact(pattern_3_pri, pattern_3_sec));
  }
  TEST_METHOD(TestIsPatternStartWithSuccess) {
    // Test pattern_1_pri matches pattern_1_sec exactly
    TokenList pattern_1_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};
    TokenList pattern_1_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsTrue(
        ExpressionHelper::IsPatternStartWith(pattern_1_pri, pattern_1_sec));

    // Test pattern_2_pri begins with the sequence of tokens in pattern_2_sec
    TokenList pattern_2_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};
    TokenList pattern_2_sec = {{tt::kName, "x"},     {tt::kName, "z"},
                               {tt::kName, "a"},     {tt::kName, "b"},
                               {tt::kOperator, "*"}, {tt::kOperator, "*"}};

    Assert::IsTrue(
        ExpressionHelper::IsPatternStartWith(pattern_2_pri, pattern_2_sec));
  }
  TEST_METHOD(TestIsPatternStartWithFail) {
    // Test pattern_1_pri length < pattern_1_sec length
    TokenList pattern_1_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_1_sec = {
        {tt::kName, "x"},     {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},     {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kOperator, "*"}, {tt::kOperator, "*"}, {tt::kName, "12"},
        {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternStartWith(pattern_1_pri, pattern_1_sec));

    // Test pattern_2_pri does not match last token in pattern_2_sec
    // pattern_2_pri length == pattern_2_sec length
    TokenList pattern_2_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_2_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternStartWith(pattern_2_pri, pattern_2_sec));

    // Test pattern_3_pri does not begin with tokens in pattern_3_sec
    TokenList pattern_3_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_3_sec = {
        {tt::kName, "x"}, {tt::kName, "z"}, {tt::kName, "a"}, {tt::kName, "d"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternStartWith(pattern_3_pri, pattern_3_sec));
  }

  TEST_METHOD(TestIsPatternEndWithSuccess) {
    // Test pattern_1_pri matches pattern_1_sec exactly
    TokenList pattern_1_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};
    TokenList pattern_1_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsTrue(
        ExpressionHelper::IsPatternEndWith(pattern_1_pri, pattern_1_sec));

    // Test pattern_2_pri ends with the sequence of tokens in pattern_2_sec
    TokenList pattern_2_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_2_sec = {
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsTrue(
        ExpressionHelper::IsPatternEndWith(pattern_2_pri, pattern_2_sec));
  }
  TEST_METHOD(TestIsPatternEndWithFail) {
    // Test pattern_1_pri length < pattern_1_sec length
    TokenList pattern_1_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_1_sec = {
        {tt::kName, "x"},     {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},     {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kOperator, "*"}, {tt::kOperator, "*"}, {tt::kName, "12"},
        {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternEndWith(pattern_1_pri, pattern_1_sec));

    // Test pattern_2_pri does not match last token in pattern_2_sec
    // pattern_2_pri length == pattern_2_sec length
    TokenList pattern_2_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_2_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternEndWith(pattern_2_pri, pattern_2_sec));

    // Test pattern_3_pri does not end with tokens in pattern_3_sec
    TokenList pattern_3_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_3_sec = {{tt::kName, "*"},
                               {tt::kName, "12"},
                               {tt::kName, "/"},
                               {tt::kName, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternEndWith(pattern_3_pri, pattern_3_sec));
  }

  TEST_METHOD(TestIsPatternSubsetSuccess) {
    // Test pattern_1_pri matches pattern_1_sec exactly
    TokenList pattern_1_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_1_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsTrue(
        ExpressionHelper::IsPatternSubset(pattern_1_pri, pattern_1_sec));

    // Test pattern_2_pri ends with the sequence of tokens in pattern_2_sec
    TokenList pattern_2_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_2_sec = {
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsTrue(
        ExpressionHelper::IsPatternSubset(pattern_2_pri, pattern_2_sec));

    // Test pattern_3_pri begins with the sequence of tokens in pattern_3_sec
    TokenList pattern_3_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_3_sec = {{tt::kName, "x"},     {tt::kName, "z"},
                               {tt::kName, "a"},     {tt::kName, "b"},
                               {tt::kOperator, "*"}, {tt::kOperator, "*"}};
    Assert::IsTrue(
        ExpressionHelper::IsPatternSubset(pattern_3_pri, pattern_3_sec));

    // Test pattern_3_sec has token sequence matching somewhere in between the
    // token sequence in pattern_3_pri
    TokenList pattern_4_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_4_sec = {{tt::kName, "a"},
                               {tt::kName, "b"},
                               {tt::kOperator, "*"},
                               {tt::kOperator, "*"}};
    Assert::IsTrue(
        ExpressionHelper::IsPatternSubset(pattern_4_pri, pattern_4_sec));
  }

  TEST_METHOD(TestIsPatternSubsetFail) {
    // Test pattern_1_pri length < pattern_1_sec length
    TokenList pattern_1_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    TokenList pattern_1_sec = {
        {tt::kName, "x"},     {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},     {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kOperator, "*"}, {tt::kOperator, "*"}, {tt::kName, "12"},
        {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternSubset(pattern_1_pri, pattern_1_sec));

    // Test pattern_2_pri does not match last token in pattern_2_sec
    // pattern_2_pri length == pattern_2_sec length
    TokenList pattern_2_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_2_sec = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternSubset(pattern_2_pri, pattern_2_sec));

    // Test pattern_3_pri does not end with tokens in pattern_3_sec
    TokenList pattern_3_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_3_sec = {{tt::kName, "*"},
                               {tt::kName, "12"},
                               {tt::kName, "/"},
                               {tt::kName, "+"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternSubset(pattern_3_pri, pattern_3_sec));

    // Test pattern_4_pri does not begin with tokens in pattern_4_sec
    TokenList pattern_4_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_4_sec = {
        {tt::kName, "x"}, {tt::kName, "z"}, {tt::kName, "a"}, {tt::kName, "d"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternSubset(pattern_4_pri, pattern_4_sec));

    // Test pattern_5_sec does not have token sequence matching somewhere in
    // between token sequence in pattern_5_pri
    TokenList pattern_5_pri = {
        {tt::kName, "x"},  {tt::kName, "z"},     {tt::kName, "a"},
        {tt::kName, "b"},  {tt::kOperator, "*"}, {tt::kOperator, "*"},
        {tt::kName, "12"}, {tt::kOperator, "/"}, {tt::kOperator, "-"}};

    TokenList pattern_5_sec = {{tt::kName, "b"},
                               {tt::kName, "*"},
                               {tt::kName, "*"},
                               {tt::kName, "11"}};

    Assert::IsFalse(
        ExpressionHelper::IsPatternSubset(pattern_5_pri, pattern_5_sec));
  }
};
}  // namespace FrontendTests