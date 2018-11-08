#pragma once

#include "const_list.h"

void ConstList::InsertConstValue(ConstValue const_value_int) {
  const_value_set_.insert(const_value_int);
  const_value_twin_set_.insert(make_pair(const_value_int, const_value_int));
}

ConstValueSet ConstList::GetAllConstValue() { return const_value_set_; }

bool ConstList::IsConstValue(ConstValue const_value) {
  return (const_value_set_.find(const_value) != const_value_set_.end());
}

ConstValuePairSet ConstList::GetAllConstValueTwin() {
  return const_value_twin_set_;
}
