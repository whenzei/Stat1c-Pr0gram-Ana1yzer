#pragma once

#include "const_list.h"
#include <sstream>

using std::stringstream;

bool ConstList::InsertConstValue(ConstValue const_value) {
  if (const_value_set_.find(const_value) != const_value_set_.end()) {
    return false;
  } else {
    stringstream const_value_ss;
    const_value_ss << const_value;
    const_value_list_.push_back(const_value_ss.str());
    const_value_set_.insert(const_value);
    return true;
  }
}

ConstValueList ConstList::GetAllConstValue() { return const_value_list_; }