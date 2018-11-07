#pragma once

#include "next_table.h"

void NextTable::InsertCFG(ProcName proc_name) {
  cfg_table_.emplace(proc_name, CFG());
  reversed_cfg_table_.emplace(proc_name, CFG());
}

void NextTable::InsertNext(ProcName proc_name, StmtNum previous_stmt,
                           StmtNum next_stmt) {
  combined_cfg_.AddEdge(previous_stmt, next_stmt);
  reversed_combined_cfg_.AddEdge(next_stmt, previous_stmt);

  // assert that there will be a cfg table for the proc name provided
  CFG* proc_cfg_ = &cfg_table_[proc_name];
  CFG* proc_reverse_cfg_ = &reversed_cfg_table_[proc_name];
  proc_cfg_->AddEdge(previous_stmt, next_stmt);
  proc_reverse_cfg_->AddEdge(next_stmt, previous_stmt);

  if (previous_set_.insert(previous_stmt).second) {
    previous_list_.push_back(previous_stmt);
  }
  if (next_set_.insert(next_stmt).second) {
    next_list_.push_back(next_stmt);
  }
}

void NextTable::InsertStatement(ProcName proc_name, StmtNum stmt_num) {
  combined_cfg_.AddNode(stmt_num);
  reversed_combined_cfg_.AddNode(stmt_num);

  // assert that there will be a cfg table for the proc name provided
  CFG* proc_cfg_ = &cfg_table_[proc_name];
  CFG* proc_reverse_cfg_ = &reversed_cfg_table_[proc_name];
  proc_cfg_->AddNode(stmt_num);
  proc_reverse_cfg_->AddNode(stmt_num);
}

CFG* NextTable::GetCFG(ProcName proc_name) { return &(cfg_table_[proc_name]); }

CFG* NextTable::GetReverseCFG(ProcName proc_name) {
  return &(reversed_cfg_table_[proc_name]);
}

bool NextTable::IsNext(StmtNum previous_stmt, StmtNum next_stmt) {
  VertexSet neighbours = combined_cfg_.GetNeighboursSet(previous_stmt);
  return neighbours.find(next_stmt) != neighbours.end();
}

bool NextTable::IsNext(StmtNum stmt_num) {
  return next_set_.find(stmt_num) != next_set_.end();
}

bool NextTable::IsPrevious(StmtNum stmt_num) {
  return previous_set_.find(stmt_num) != previous_set_.end();
}

StmtNumList NextTable::GetNext(StmtNum stmt_num) {
  return combined_cfg_.GetNeighboursList(stmt_num);
}

StmtNumList NextTable::GetPrevious(StmtNum stmt_num) {
  return reversed_combined_cfg_.GetNeighboursList(stmt_num);
}

StmtNumList NextTable::GetAllNext() { return next_list_; }

StmtNumList NextTable::GetAllPrevious() { return previous_list_; }

StmtNumPairList NextTable::GetAllNextPairs() { return next_pair_list_; }

CFG* NextTable::GetCombinedCFG() { return &combined_cfg_; }

CFG* NextTable::GetReverseCombinedCFG() { return &reversed_combined_cfg_; }

void NextTable::SetAllNextPairs(StmtNumPairList next_pair_list) {
  next_pair_list_ = next_pair_list;
}

void NextTable::SetProgramCFG(const CFG& program_cfg) {
  program_cfg_ = program_cfg;
}

void NextTable::SetReverseProgramCFG(const CFG& reversed_program_cfg) {
  reversed_program_cfg_ = reversed_program_cfg;
}

CFG* NextTable::GetProgramCFG() { return &program_cfg_; }

CFG* NextTable::GetReverseProgramCFG() { return &reversed_program_cfg_; }


bool NextTable::HasNextRelationship() { return !combined_cfg_.IsEmpty(); }
