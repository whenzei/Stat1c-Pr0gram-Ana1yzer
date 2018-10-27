#pragma once

#include "const_list.h"

using std::to_string;

void ConstList::InsertConstValue(ConstValue const_value_int) {
  if (const_value_set_.insert(const_value_int).second) {
    const_value_list_.push_back(const_value_int);
    const_value_twin_list_.push_back(
        std::make_pair(const_value_int, const_value_int));
  }
}

ConstValueList ConstList::GetAllConstValue() { return const_value_list_; }

bool ConstList::IsConstValue(ConstValue const_value) {
  return (const_value_set_.find(const_value) != const_value_set_.end());
}

ConstValuePairList ConstList::GetAllConstValueTwin() {
  return const_value_twin_list_;
}
