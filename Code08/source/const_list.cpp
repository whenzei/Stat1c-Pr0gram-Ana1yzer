#pragma once

#include "const_list.h"

using std::to_string;

void ConstList::InsertConstValue(ConstValue const_value_int) {
  if (const_value_set_.insert(const_value_int).second) {
    string const_value = to_string(const_value_int);
    const_value_list_.push_back(const_value);
    const_value_twin_list_.push_back(make_pair(const_value, const_value));
  } 
}

ConstValueList ConstList::GetAllConstValue() { return const_value_list_; }

ConstValuePairList ConstList::GetAllConstValueTwin() {
  return const_value_twin_list_;
}
