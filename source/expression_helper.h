#pragma once

#ifndef EXPRESSION_HELPER_H
#define EXPRESSION_HELPER_H

#include "tokenizer.h"

class ExpressionHelper {
 public:
  static TokenList ToPostfix(TokenList tokens_to_convert);
};
#endif	