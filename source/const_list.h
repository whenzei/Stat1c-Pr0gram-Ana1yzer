#pragma once

#ifndef SPA_CONST_LIST_H
#define SPA_CONST_LIST_H

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

using std::list;
using std::string;
using std::unordered_set;
using std::vector;

using ConstValue = int;
using ConstValueList = vector<string>;
using ConstValueSet = unordered_set<int>;

// The constant list class for the PKB component
// Used to store constant values that are passed into PKB from the parser
class ConstList {
  ConstValueList const_value_list_;
  ConstValueSet const_value_set_;

 public:
  // Returns a list of all constant values in the ConstList.
  ConstValueList GetAllConstValue();

  // Takes in the constant value in integer format.
  // Checks if the same constant value already exists in the ConstList, then adds
  // the constant value to the ConstList if not. Returns false if the same value 
  // already exists in the ConstTable. Returns true if value is successfully added 
  // to the ConstTable.
  bool InsertConstValue(ConstValue const_value);
};

#endif !SPA_CONST_LIST_H