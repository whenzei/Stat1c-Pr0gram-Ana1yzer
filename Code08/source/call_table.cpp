#pragma once

#include "call_table.h"
  
  bool CallTable::InsertCalls(ProcName caller_proc, ProcName callee_proc) {
    if (IsCallT(caller_proc, callee_proc)) {
      return false;
    }
    if (IsCall(caller_proc, callee_proc)) {
      direct_call_table_[caller_proc].push_back(callee_proc);
    }
    call_table_[caller_proc].push_back(callee_proc);
    return true;
  }

  ProcNameList CallTable::GetCallee(ProcName caller_proc) {
    ProcNameList callee_list;
    if (direct_call_table_.find(caller_proc) != direct_call_table_.end()) {
      callee_list.push_back(direct_call_table_[caller_proc].front());
    }
    return callee_list;
  }

  ProcNameList CallTable::GetCalleeT(ProcName caller_proc) {
    ProcNameList callee_list;
    if (call_table_.find(caller_proc) != call_table_.end()) {
      callee_list = call_table_[caller_proc];
    }
    return callee_list;
  }

  ProcNameList CallTable::GetCaller(ProcName callee_proc) {
    ProcNameList caller_list;
    if (direct_callee_table_.find(callee_proc) != direct_callee_table_.end()) {
      caller_list.push_back(direct_callee_table_[callee_proc].front());
    }
    return caller_list;
  }

  ProcNameList CallTable::GetCallerT(ProcName callee_proc) {
    ProcNameList caller_list;
    if (callee_table_.find(callee_proc) != callee_table_.end()) {
      caller_list = callee_table_[callee_proc];
    }
    return caller_list;
  }

  ProcNameList CallTable::GetAllCaller() {
    return caller_list_;
  }

  ProcNameList CallTable::GetAllCallee() {
    return callee_list_;
  }

  ProcNamePairList CallTable::GetAllCallPairs() {
    ProcNamePairList call_pair_list;
    for (auto entry : direct_call_table_) {
      if (direct_call_table_.find(entry.first) != direct_call_table_.end()) {
        call_pair_list.push_back(make_pair(entry.first, direct_call_table_[entry.first].front()));
      }
    }
    return call_pair_list;
  }

  ProcNamePairList CallTable::GetAllCallTPairs() {
    ProcNamePairList call_pair_list;
    for (auto entry : call_table_) {
      if (call_table_.find(entry.first) != call_table_.end()) {
        for (ProcName called_proc : call_table_[entry.first]) {
          call_pair_list.push_back(make_pair(entry.first, called_proc));
        }
      }
    }
    return call_pair_list;
  }

  bool CallTable::IsCall(ProcName caller_proc, ProcName callee_proc) {
    if (direct_call_table_.find(caller_proc) != direct_call_table_.end()) {
      return (direct_call_table_[caller_proc].front() == callee_proc);
    }
  }

  bool CallTable::IsCallT(ProcName caller_proc, ProcName callee_proc) {
    if (call_table_.find(caller_proc) != call_table_.end()) {
      ProcNameList callee_list = call_table_[caller_proc];
      return (find(callee_list.begin(), callee_list.end(), callee_proc) != callee_list.end());
    }
  }

  bool CallTable::HasCallsRelationship() {
    return direct_call_table_.empty();
  }