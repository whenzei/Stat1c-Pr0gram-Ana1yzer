#pragma once

#ifndef SPA_CONST_LIST_H
#define SPA_CONST_LIST_H

#include <string>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_set;
using std::vector;

using ConstValue = int;
using ConstValueList = vector<int>;
using ConstValuePairList = vector<pair<int, int>>;
using ConstValueSet = unordered_set<int>;

// The constant list class for the PKB component
// Used to store constant values that are passed into PKB from the parser
class ConstList {
  ConstValueList const_value_list_;
  ConstValuePairList const_value_twin_list_;
  ConstValueSet const_value_set_;

 public:
  // Returns a list of all constant values in the ConstList.
  ConstValueList GetAllConstValue();

  // Returns a list of all constant values in pairs (in each pair, the const
  // value is repeated)
  ConstValuePairList GetAllConstValueTwin();

  // Inserts constant value into the ConstList.
  // Duplicate values will be ignored.
  void InsertConstValue(ConstValue const_value);

  // Checks and returns whether the given value is a value in the const table.
  // @return true if const_value is in the const list
  bool IsConstValue(ConstValue const_value);
};

#endif !SPA_CONST_LIST_H