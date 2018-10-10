#pragma once

#include "next_table.h"

void NextTable::InsertCFG(ProcName proc_name) {
  cfg_table_.emplace(proc_name, make_pair(CFG(), CFG()));
}

void NextTable::InsertNext(ProcName proc_name, StmtNumInt previous_stmt_int,
                           StmtNumInt next_stmt_int) {
  StmtNum previous_stmt = to_string(previous_stmt_int);
  StmtNum next_stmt = to_string(next_stmt_int);
  combined_cfg_[previous_stmt].push_back(next_stmt);
  combined_reverse_cfg_[next_stmt].push_back(previous_stmt);
  CFG* proc_cfg_ = &cfg_table_[proc_name].first;
  CFG* proc_reverse_cfg_ = &cfg_table_[proc_name].second;
  (*proc_cfg_)[previous_stmt].push_back(next_stmt);
  (*proc_reverse_cfg_)[next_stmt].push_back(previous_stmt);

  if (previous_set_.insert(previous_stmt).second) {
    previous_list_.push_back(previous_stmt);
  }
  if (next_set_.insert(next_stmt).second) {
    next_list_.push_back(next_stmt);
  }
}

CFG* NextTable::GetCFG(ProcName proc_name) {
  return &(cfg_table_[proc_name].first);
}

bool NextTable::IsNext(StmtNum previous_stmt, StmtNum next_stmt) {
  CFG::iterator iter = combined_cfg_.find(previous_stmt);
  if (iter != combined_cfg_.end()) {
    StmtNumList next_stmts = (*iter).second;
    return find(next_stmts.begin(), next_stmts.end(), next_stmt) !=
           next_stmts.end();
  } else {
    return false;
  }
}

bool NextTable::IsNext(StmtNum stmt_num) { return next_set_.find(stmt_num) != next_set_.end(); }

bool NextTable::IsPrevious(StmtNum stmt_num) {
  return previous_set_.find(stmt_num) != previous_set_.end();
}

StmtNumList NextTable::GetNext(StmtNum stmt_num) {
  CFG::iterator iter = combined_cfg_.find(stmt_num);
  if (iter != combined_cfg_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  };
}

StmtNumList NextTable::GetPrevious(StmtNum stmt_num) {
  CFG::iterator iter = combined_reverse_cfg_.find(stmt_num);
  if (iter != combined_reverse_cfg_.end()) {
    return (*iter).second;
  } else {
    return StmtNumList();
  };
}

StmtNumList NextTable::GetAllNext() { return next_list_; }

StmtNumList NextTable::GetAllPrevious() { return previous_list_; }

StmtNumPairList NextTable::GetAllNextPairs() {
  StmtNumPairList next_pair_list;
  for (auto entry : combined_cfg_) {
    for (StmtNum& next_stmt : entry.second) {
      next_pair_list.push_back(make_pair(entry.first, next_stmt));
    }
  }
  return next_pair_list;
}

bool NextTable::HasNextRelationship() { return !combined_cfg_.empty(); }

// TODO: add getter methods, check with pql whether to return string or int
