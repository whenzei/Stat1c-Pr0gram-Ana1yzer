#pragma once

#include "call_table.h"

bool CallTable::InsertIndirectCallRelationship(ProcIndex caller_proc,
                                               ProcIndex callee_proc) {
  if (IsCallT(caller_proc, callee_proc)) {
    return false;
  }
  call_table_[caller_proc].push_back(callee_proc);
  callee_table_[callee_proc].push_back(caller_proc);
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
  direct_call_table_[caller_proc].push_back(callee_proc);
  direct_callee_table_[callee_proc].push_back(caller_proc);
  call_table_[caller_proc].push_back(callee_proc);
  callee_table_[callee_proc].push_back(caller_proc);
  if (caller_set_.insert(caller_proc).second) {
    caller_list_.push_back(caller_proc);
  }
  if (callee_set_.insert(callee_proc).second) {
    callee_list_.push_back(callee_proc);
    callee_twin_list_.push_back(make_pair(callee_proc, callee_proc));
  }
  return true;
}

void CallTable::InsertCalls(StmtNum stmt_num, ProcIndex callee_proc) {
  stmt_num_proc_table_[callee_proc].push_back(stmt_num);
  stmt_to_call_table_[stmt_num] = callee_proc;
  for (StmtNum stmt : stmt_num_proc_table_[callee_proc]) {
    stmt_pair_list_.push_back(make_pair(stmt_num, stmt));
    if (stmt_num != stmt) {
      stmt_pair_list_.push_back(make_pair(stmt, stmt_num));
    }
  }
}

ProcIndex CallTable::GetCalledProcedure(StmtNum stmt_num) {
  if (stmt_to_call_table_.count(stmt_num)) {
    return stmt_to_call_table_[stmt_num];
  }
  return -1;
}

StmtNumList CallTable::GetCallingStmts(ProcIndex callee_proc) {
  StmtNumList calling_stmts;
  if (stmt_num_proc_table_.find(callee_proc) != stmt_num_proc_table_.end()) {
    calling_stmts = stmt_num_proc_table_[callee_proc];
  }
  return calling_stmts;
}

StmtNumProcPairList CallTable::GetAllCallingStmtPairs() {
  StmtNumProcPairList stmt_proc_pair_list;
  for (auto entry : stmt_num_proc_table_) {
    for (StmtNum stmt_num : entry.second) {
      stmt_proc_pair_list.push_back(make_pair(stmt_num, entry.first));
    }
  }
  return stmt_proc_pair_list;
}

ProcIndexList CallTable::GetCallee(ProcIndex caller_proc) {
  ProcIndexList callee_list;
  CallMap::iterator iter = direct_call_table_.find(caller_proc);
  if (iter != direct_call_table_.end()) {
    callee_list = (*iter).second;
  }
  return callee_list;
}

ProcIndexList CallTable::GetCalleeT(ProcIndex caller_proc) {
  ProcIndexList callee_list;
  if (call_table_.find(caller_proc) != call_table_.end()) {
    callee_list = call_table_[caller_proc];
  }
  return callee_list;
}

ProcIndexList CallTable::GetCaller(ProcIndex callee_proc) {
  ProcIndexList caller_list;
  if (direct_callee_table_.find(callee_proc) != direct_callee_table_.end()) {
    caller_list = direct_callee_table_[callee_proc];
  }
  return caller_list;
}

ProcIndexList CallTable::GetCallerT(ProcIndex callee_proc) {
  ProcIndexList caller_list;
  if (callee_table_.find(callee_proc) != callee_table_.end()) {
    caller_list = callee_table_[callee_proc];
  }
  return caller_list;
}

ProcIndexList CallTable::GetAllCaller() { return caller_list_; }

ProcIndexList CallTable::GetAllCallee() { return callee_list_; }

ProcIndexPairList CallTable::GetAllCalleeTwin() { return callee_twin_list_; }

ProcIndexPairList CallTable::GetAllCallPairs() {
  ProcIndexPairList call_pair_list;
  for (auto entry : direct_call_table_) {
    if (direct_call_table_.find(entry.first) != direct_call_table_.end()) {
      for (auto callee : direct_call_table_[entry.first]) {
        call_pair_list.push_back(make_pair(entry.first, callee));
      }
    }
  }
  return call_pair_list;
}

ProcIndexPairList CallTable::GetAllCallTPairs() {
  ProcIndexPairList call_pair_list;
  for (auto entry : call_table_) {
    if (call_table_.find(entry.first) != call_table_.end()) {
      for (ProcIndex called_proc : call_table_[entry.first]) {
        call_pair_list.push_back(make_pair(entry.first, called_proc));
      }
    }
  }
  return call_pair_list;
}

bool CallTable::IsCall(ProcIndex caller_proc, ProcIndex callee_proc) {
  if (direct_call_table_.find(caller_proc) != direct_call_table_.end()) {
    ProcIndexList direct_callee_list = direct_call_table_[caller_proc];
    return (find(direct_callee_list.begin(), direct_callee_list.end(),
                 callee_proc) != direct_callee_list.end());
  }
  return false;
}

bool CallTable::IsCallT(ProcIndex caller_proc, ProcIndex callee_proc) {
  if (call_table_.find(caller_proc) != call_table_.end()) {
    ProcIndexList callee_list = call_table_[caller_proc];
    return (find(callee_list.begin(), callee_list.end(), callee_proc) !=
            callee_list.end());
  }
  return false;
}

bool CallTable::IsCalledProc(ProcIndex callee_proc) {
  return callee_set_.find(callee_proc) != callee_set_.end();
}

bool CallTable::HasCallsRelationship() { return !direct_call_table_.empty(); }

StmtNumPairList CallTable::GetAllCallPairWithSameProc() {
  return stmt_pair_list_;
}
