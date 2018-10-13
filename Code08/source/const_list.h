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
using ConstValueList = vector<string>;
using ConstValuePairList = vector<pair<string, string>>;
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

  // Returns a list of all constant values in pairs (in each pair, the const value is repeated)
  ConstValuePairList GetAllConstValueTwin();

  // Convert const_value from int to string (because output needs to be in the
  // format vector<string>) and insert into the ConstList
  void InsertConstValue(ConstValue const_value);
};

#endif !SPA_CONST_LIST_H