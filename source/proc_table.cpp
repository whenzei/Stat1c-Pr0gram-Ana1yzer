#pragma once

#include <string>
#include <vector>

#include "proc_table.h"

using namespace std;

bool ProcTable::Find(string proc_name) {
  for (vector<string>::iterator iter = proc_vector_.begin();
       iter != proc_vector_.end(); iter++) {
    if (proc_name.compare(*iter) == 0) {
      return true;
    }
  }
  return false; 
}

int ProcTable::Insert(string proc_name) {
  if (proc_name.compare("") == 0) {
    return -1;
  } else if (Find(proc_name)) {
    return -1;
  } else {
    proc_vector_.push_back(proc_name);
    return 0;
  }
}

vector<string> ProcTable::List() { return proc_vector_; }
