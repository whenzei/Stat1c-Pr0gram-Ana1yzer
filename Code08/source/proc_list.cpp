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

ProcIndexList ProcList::GetAllProcName() { return proc_name_index_list_; }

bool ProcList::IsProcName(ProcIndex proc_index) {
  return index_proc_map_.find(proc_index) != index_proc_map_.end();
}

ProcIndexPairList ProcList::GetAllProcNameTwin() {
  return proc_name_index_twin_list_;
}

IndexProcMap ProcList::GetIndexToProcMapping() { return index_proc_map_; }

ProcIndexMap ProcList::GetProcToIndexMapping() { return proc_index_map_; }

ProcName ProcList::GetProcName(ProcIndex index) {
  IndexProcMap::iterator iter = index_proc_map_.find(index);
  if (iter != index_proc_map_.end()) {
    return (*iter).second;
  } else {
    return ProcName();
  }
}

ProcIndex ProcList::GetProcIndex(ProcName proc_name) {
  ProcIndexMap::iterator iter = proc_index_map_.find(proc_name);
  if (iter != proc_index_map_.end()) {
    return (*iter).second;
  } else {
    return -1;
  }
}
