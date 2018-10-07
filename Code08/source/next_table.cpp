#pragma once

#include "next_table.h"

void NextTable::InsertCFG(ProcName proc_name) {
  cfg_table_.emplace(proc_name, make_pair(CFG(), CFG()));
}

void NextTable::InsertNext(ProcName proc_name, StmtNumInt previous_stmt,
                           StmtNumInt next_stmt) {
  cfg_[previous_stmt].push_back(next_stmt);
  reverse_cfg_[next_stmt].push_back(previous_stmt);
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

CFG* NextTable::GetCFG(ProcName proc_name) { return &(cfg_table_[proc_name].first); }

// TODO: add getter methods, check with pql whether to return string or int
