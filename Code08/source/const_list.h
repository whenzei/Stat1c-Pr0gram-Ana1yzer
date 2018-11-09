#pragma once

#ifndef SPA_CONST_LIST_H
#define SPA_CONST_LIST_H

#include <unordered_set>
#include "result_hasher.h"

using std::make_pair;
using std::pair;
using std::unordered_set;

using ConstValue = int;
using ConstValuePairSet = unordered_set<pair<ConstValue, ConstValue>, ResultHasher>;
using ConstValueSet = unordered_set<ConstValue>;

// The constant list class for the PKB component
// Used to store constant values that are passed into PKB from the parser
class ConstList {
  ConstValuePairSet const_value_twin_set_;
  ConstValueSet const_value_set_;

 public:
  // Returns a set of all constant values in the ConstList.
  ConstValueSet GetAllConstValue();

  // Returns a set of all constant values in pairs (in each pair, the const
  // value is repeated)
  ConstValuePairSet GetAllConstValueTwin();

  // Inserts constant value into the ConstList.
  // Duplicate values will be ignored.
  void InsertConstValue(ConstValue const_value);

  // Checks and returns whether the given value is a value in the const table.
  // @return true if const_value is in the const list
  bool IsConstValue(ConstValue const_value);
};

#endif !SPA_CONST_LIST_H