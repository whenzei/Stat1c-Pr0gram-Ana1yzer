#pragma once

#include "call_table.h"
  
  bool CallTable::InsertIndirectCallRelationship(ProcIndex caller_proc, ProcIndex callee_proc) {
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

  bool CallTable::InsertDirectCallRelationship(ProcIndex caller_proc, ProcIndex callee_proc) {
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
  }

  StmtNumList CallTable::GetCallingStmts(ProcIndex callee_proc) {
    StmtNumList calling_stmts;
    if (stmt_num_proc_table_.find(callee_proc) != stmt_num_proc_table_.end()) {
      calling_stmts = stmt_num_proc_table_[callee_proc];
    }
    return calling_stmts;
  }

  StmtNumProcPairList CallTable::GetAllCallingStmtPairs(){
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
    if (direct_call_table_.find(caller_proc) != direct_call_table_.end()) {
      callee_list = direct_call_table_[caller_proc];
    }
    return callee_list;
  }

  ProcNameList CallTable::GetCallee(ProcName caller_proc) {
    ProcIndexList callee_list = GetCallee(proc_list_.GetProcIndex(caller_proc));
    ProcNameList callee_name_list;
    for (auto& proc_index : callee_list) {
      callee_name_list.push_back(proc_list_.GetProcName(proc_index));
    }
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

  ProcIndexList CallTable::GetAllCaller() {
    return caller_list_;
  }

  ProcIndexList CallTable::GetAllCallee() {
    return callee_list_; }

  ProcNamePairList CallTable::GetAllCalleeTwin() { return callee_twin_list_; }

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
        for (ProcIndex called_proc : call_table_[entry.first]) {
          call_pair_list.push_back(make_pair(entry.first, called_proc));
        }
      }
    }
    return call_pair_list;
  }

  bool CallTable::IsCall(ProcIndex caller_proc, ProcIndex callee_proc) {
    if (direct_call_table_.find(caller_proc) != direct_call_table_.end()) {
      return (direct_call_table_[caller_proc].front() == callee_proc);
    }
    return false;
  }

  bool CallTable::IsCallT(ProcIndex caller_proc, ProcIndex callee_proc) {
    if (call_table_.find(caller_proc) != call_table_.end()) {
      ProcNameList callee_list = call_table_[caller_proc];
      return (find(callee_list.begin(), callee_list.end(), callee_proc) != callee_list.end());
    }
    return false; 
  }

  bool CallTable::IsCalledProc(ProcIndex callee_proc) {
    return callee_set_.find(callee_proc) != callee_set_.end();
  }

  bool CallTable::HasCallsRelationship() {
    return !direct_call_table_.empty();
  }