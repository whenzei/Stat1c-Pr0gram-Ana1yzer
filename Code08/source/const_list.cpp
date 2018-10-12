#pragma once

#include "const_list.h"

using std::to_string;

void ConstList::InsertConstValue(ConstValue const_value) {
  if (const_value_set_.insert(const_value).second) {
    const_value_list_.push_back(to_string(const_value));
  } 
}

ConstValueList ConstList::GetAllConstValue() { return const_value_list_; }

bool ConstList::IsConstValue(ConstValue const_value) {
  return (const_value_set_.find(const_value)!=const_value_set_.end());
}