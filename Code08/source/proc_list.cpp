#pragma once

#include "proc_list.h"

int ProcList::InsertProcName(ProcName proc_name) {
  if (proc_id_map_.find(proc_name) == proc_id_map_.end()) {
    int proc_id = num_procs++;
    // populate index list
    proc_id_list_.push_back(proc_id);
    proc_id_twin_list_.push_back(make_pair(proc_id, proc_id));

    // populate name list
    proc_name_list_.push_back(proc_name);
    proc_name_twin_list_.push_back(make_pair(proc_name, proc_name));

    proc_id_map_[proc_name] = proc_id;
    id_proc_map_[proc_id] = proc_name;
    return proc_id;
  }
  return -1;
}

ProcNameList ProcList::GetAllProcNames() { return proc_name_list_; }

ProcIndexList ProcList::GetAllProcIndices() { return proc_id_list_; }

bool ProcList::IsProcIndex(ProcIndex proc_id) {
  return id_proc_map_.find(proc_id) != id_proc_map_.end();
}

bool ProcList::IsProcName(ProcName proc_name) {
  return proc_id_map_.find(proc_name) != proc_id_map_.end();
}

ProcNamePairList ProcList::GetAllProcNameTwin() {
  return proc_name_twin_list_;
}

ProcIndexPairList ProcList::GetAllProcIndexTwin() {
  return proc_id_twin_list_;
}

IndexProcMap ProcList::GetIndexToProcMapping() { return id_proc_map_; }

ProcIndexMap ProcList::GetProcToIndexMapping() { return proc_id_map_; }

ProcName ProcList::GetProcName(ProcIndex proc_id) {
  if (id_proc_map_.count(proc_id)) {
    return id_proc_map_.at(proc_id);
  }

  return ProcName();
}

ProcIndex ProcList::GetProcIndex(ProcName proc_name) {
  if (proc_id_map_.count(proc_name)) {
    return proc_id_map_.at(proc_name);
  }

  return -1;
}
