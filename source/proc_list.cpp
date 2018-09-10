#pragma once

#include "proc_list.h"

bool ProcList::InsertProcName(ProcName proc_name) {
  if (FindProcName(proc_name)) {
    return false;
  } else {
    proc_name_list_.push_back(proc_name);
    return true;
  }
}

ProcNameList ProcList::GetAllProcName() { return proc_name_list_; }

bool ProcList::FindProcName(ProcName proc_name) {
  for (ProcName& element: proc_name_list_) {
    if (proc_name.compare(element) == 0) {
      return true;
    }
  }
  return false;
}
