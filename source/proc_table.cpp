#pragma once

#include "proc_table.h"

int ProcTable::InsertProcName(ProcName proc_name) {
  if (proc_name.compare("") == 0) {
    return -1;
  } else if (FindProcName(proc_name)) {
    return -1;
  } else {
    proc_vector_.push_back(proc_name);
    return 0;
  }
}

ProcList ProcTable::GetProcTable() { return proc_vector_; }

bool ProcTable::FindProcName(ProcName proc_name) {
  for (ProcName& element: proc_vector_) {
    if (proc_name.compare(element) == 0) {
      return true;
    }
  }
  return false;
}
