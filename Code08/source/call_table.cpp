#pragma once

#include "call_table.h"
  
  bool CallTable::InsertIndirectCallRelationship(ProcName caller_proc, ProcName callee_proc) {
    if (proc_list_.GetProcIndex(caller_proc) == -1) {
      proc_list_.InsertProcName(caller_proc);
    }
    if (proc_list_.GetProcIndex(callee_proc) == -1) {
      proc_list_.InsertProcName(callee_proc);
    }
    int caller_proc_index = proc_list_.GetProcIndex(caller_proc);
    int callee_proc_index = proc_list_.GetProcIndex(callee_proc);
    if (IsCallT(caller_proc_index, callee_proc_index)) {
      return false;
    }
    call_table_[caller_proc_index].push_back(callee_proc_index);
    callee_table_[callee_proc_index].push_back(caller_proc_index);
    if (caller_set_.insert(caller_proc_index).second) {
      caller_list_.push_back(caller_proc_index);
    }
    if (callee_set_.insert(callee_proc_index).second) {
      callee_list_.push_back(callee_proc_index);
    }
    return true;
  }

  bool CallTable::InsertDirectCallRelationship(ProcName caller_proc, ProcName callee_proc) {
    if (proc_list_.GetProcIndex(caller_proc) == -1) {
      proc_list_.InsertProcName(caller_proc);
    }
    if (proc_list_.GetProcIndex(callee_proc) == -1) {
      proc_list_.InsertProcName(callee_proc);
    }
    int caller_proc_index = proc_list_.GetProcIndex(caller_proc);
    int callee_proc_index = proc_list_.GetProcIndex(callee_proc);
    if (IsCallT(caller_proc_index, callee_proc_index)) {
      return false;
    }
    direct_call_table_[caller_proc_index].push_back(callee_proc_index);
    direct_callee_table_[callee_proc_index].push_back(caller_proc_index);
    call_table_[caller_proc_index].push_back(callee_proc_index);
    callee_table_[callee_proc_index].push_back(caller_proc_index);
    if (caller_set_.insert(caller_proc_index).second) {
      caller_list_.push_back(caller_proc_index);
    }
    if (callee_set_.insert(callee_proc_index).second) {
      callee_list_.push_back(callee_proc_index);
      callee_twin_list_.push_back(make_pair(callee_proc_index, callee_proc_index));
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
    CallMap::iterator iter = direct_call_table_.find(caller_proc);
    if (iter != direct_call_table_.end()) {
      callee_list = (*iter).second;
    }
    return callee_list;
  }

  ProcNameList CallTable::GetCallee(ProcName caller_proc) {
    ProcIndexList callee_list = GetCallee(proc_list_.GetProcIndex(caller_proc));
    ProcNameList callee_name_list;
    for (int proc_index : callee_list) {
      callee_name_list.push_back(proc_list_.GetProcName(proc_index));
    }
    return callee_name_list;
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
      ProcIndexList callee_list = call_table_[caller_proc];
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