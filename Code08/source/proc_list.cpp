#pragma once

#include "proc_list.h"

int ProcList::InsertProcName(ProcName proc_name) {
  if (proc_index_map_.find(proc_name) == proc_index_map_.end()) {
    int index = num_procs;
    proc_name_index_list_.push_back(index);
    proc_name_index_twin_list_.push_back(make_pair(index, index));
    proc_index_map_[proc_name] = index;
    index_proc_map_[index] = proc_name;
    num_procs++;
    return index;
  }
  return -1;
}

ProcNameIndexList ProcList::GetAllProcName() { return proc_name_index_list_; }

bool ProcList::IsProcName(ProcName proc_name) {
  return proc_index_map_.find(proc_name) != proc_index_map_.end();
}

ProcNameIndexPairList ProcList::GetAllProcNameTwin() { return proc_name_index_twin_list_; }

IndexProcMap ProcList::GetIndexToProcMapping() {
  return index_proc_map_;
}

ProcIndexMap ProcList::GetProcToIndexMapping() {
  return proc_index_map_; }

ProcName ProcList::GetProcName(ProcNameIndex index) { return index_proc_map_[index]; }

ProcNameIndex ProcList::GetProcNameIndex(ProcName proc_name) {
  return proc_index_map_[proc_name];
}
