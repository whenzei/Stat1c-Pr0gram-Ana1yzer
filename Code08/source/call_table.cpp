#pragma once

#include "call_table.h"

bool CallTable::InsertIndirectCallRelationship(ProcIndex caller_proc,
                                               ProcIndex callee_proc) {
  if (IsCallT(caller_proc, callee_proc)) {
    return false;
  }
  call_table_[caller_proc].insert(callee_proc);
  callee_table_[callee_proc].insert(caller_proc);
  if (caller_set_.insert(caller_proc).second) {
    caller_list_.push_back(caller_proc);
  }
  if (callee_set_.insert(callee_proc).second) {
    callee_list_.push_back(callee_proc);
  }
  return true;
}

bool CallTable::InsertDirectCallRelationship(ProcIndex caller_proc,
                                             ProcIndex callee_proc) {
  if (IsCallT(caller_proc, callee_proc)) {
    return false;
  }
  direct_call_table_[caller_proc].insert(callee_proc);
  direct_callee_table_[callee_proc].insert(caller_proc);
  call_table_[caller_proc].insert(callee_proc);
  callee_table_[callee_proc].insert(caller_proc);
  if (caller_set_.insert(caller_proc).second) {
    caller_list_.push_back(caller_proc);
  }
  if (callee_set_.insert(callee_proc).second) {
    callee_list_.push_back(callee_proc);
    callee_twin_set_.insert(
        make_pair(callee_proc, callee_proc));
  }
  return true;
}

void CallTable::InsertCalls(StmtNum stmt_num, ProcIndex callee_proc) {
  stmt_num_proc_table_[callee_proc].insert(stmt_num);
  stmt_to_call_table_[stmt_num] = callee_proc;
}

ProcIndex CallTable::GetCalledProcedure(StmtNum stmt_num) {
  if (stmt_to_call_table_.count(stmt_num)) {
    return stmt_to_call_table_[stmt_num];
  }
  return -1;
}

StmtNumSet CallTable::GetCallingStmts(ProcIndex callee_proc) {
  StmtNumSet calling_stmts;
  if (stmt_num_proc_table_.find(callee_proc) != stmt_num_proc_table_.end()) {
    calling_stmts = stmt_num_proc_table_[callee_proc];
  }
  return calling_stmts;
}

StmtNumProcPairSet CallTable::GetAllCallingStmtPairs() {
  StmtNumProcPairSet stmt_proc_pair_set;
  for (auto entry : stmt_num_proc_table_) {
    for (StmtNum stmt_num : entry.second) {
      stmt_proc_pair_set.insert(make_pair(stmt_num, entry.first));
    }
  }
  return stmt_proc_pair_set;
}

ProcIndexSet CallTable::GetCallee(ProcIndex caller_proc) {
  ProcIndexSet callee_set;
  CallMap::iterator iter = direct_call_table_.find(caller_proc);
  if (iter != direct_call_table_.end()) {
    callee_set = (*iter).second;
  }
  return callee_set;
}

ProcIndexSet CallTable::GetCalleeT(ProcIndex caller_proc) {
  ProcIndexSet callee_set;
  if (call_table_.find(caller_proc) != call_table_.end()) {
    callee_set = call_table_[caller_proc];
  }
  return callee_set;
}

ProcIndexSet CallTable::GetCaller(ProcIndex callee_proc) {
  ProcIndexSet caller_set;
  if (direct_callee_table_.find(callee_proc) != direct_callee_table_.end()) {
    caller_set = direct_callee_table_[callee_proc];
  }
  return caller_set;
}

ProcIndexSet CallTable::GetCallerT(ProcIndex callee_proc) {
  ProcIndexSet caller_set;
  if (callee_table_.find(callee_proc) != callee_table_.end()) {
    caller_set = callee_table_[callee_proc];
  }
  return caller_set;
}

ProcIndexSet CallTable::GetAllCaller() { return caller_set_; }

ProcIndexSet CallTable::GetAllCallee() { return callee_set_; }

ProcIndexPairSet CallTable::GetAllCalleeTwin() { return callee_twin_set_; }

ProcIndexPairSet CallTable::GetAllCallPairs() {
  ProcIndexPairSet call_pair_set;
  for (auto entry : direct_call_table_) {
    if (direct_call_table_.find(entry.first) != direct_call_table_.end()) {
      for (auto callee : direct_call_table_[entry.first]) {
        call_pair_set.insert(make_pair(entry.first, callee));
      }
    }
  }
  return call_pair_set;
}

ProcIndexPairSet CallTable::GetAllCallTPairs() {
  ProcIndexPairSet call_pair_set;
  for (auto entry : call_table_) {
    if (call_table_.find(entry.first) != call_table_.end()) {
      for (ProcIndex called_proc : call_table_[entry.first]) {
        call_pair_set.insert(make_pair(entry.first, called_proc));
      }
    }
  }
  return call_pair_set;
}

bool CallTable::IsCall(ProcIndex caller_proc, ProcIndex callee_proc) {
  if (direct_call_table_.find(caller_proc) != direct_call_table_.end()) {
    ProcIndexSet direct_callee_set = direct_call_table_[caller_proc];
    return (find(direct_callee_set.begin(), direct_callee_set.end(), callee_proc)
        != direct_callee_set.end());
  }
  return false;
}

bool CallTable::IsCallT(ProcIndex caller_proc, ProcIndex callee_proc) {
  if (call_table_.find(caller_proc) != call_table_.end()) {
    ProcIndexSet callee_set = call_table_[caller_proc];
    return (find(callee_set.begin(), callee_set.end(), callee_proc) !=
        callee_set.end());
  }
  return false;
}

bool CallTable::IsCalledProc(ProcIndex callee_proc) {
  return callee_set_.find(callee_proc) != callee_set_.end();
}

bool CallTable::HasCallsRelationship() { return !direct_call_table_.empty(); }