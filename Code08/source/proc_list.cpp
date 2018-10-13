#pragma once

#include "proc_list.h"

void ProcList::InsertProcName(ProcName proc_name) {
  if (proc_name_set_.insert(proc_name).second) {
    proc_name_list_.push_back(proc_name);
    proc_name_twin_list_.push_back(make_pair(proc_name, proc_name));
  }
}

ProcNameList ProcList::GetAllProcName() { return proc_name_list_; }

ProcNamePairList ProcList::GetAllProcNameTwin() { return proc_name_twin_list_; }
