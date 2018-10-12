#pragma once

#ifndef SPA_PROC_LIST_H
#define SPA_PROC_LIST_H

#include <string>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_set;
using std::vector;

using ProcName = string;
using ProcNameList = vector<string>;
using ProcNameSet = unordered_set<string>;

// The procedure list class for the PKB component
// Used to store procedure names that are passed into PKB from the parser
class ProcList {
  
 ProcNameList proc_name_list_;
 ProcNameSet proc_name_set_;

 public:
  
  // inserts proc_name into the procedure list
  void InsertProcName(ProcName proc_name);

  // @returns the list of all procedure names (can be empty)
  ProcNameList GetAllProcName();

  // @return true if proc_name exists in the proc list
  bool IsProcName(ProcName proc_name);
};

#endif  // !SPA_PROC_LIST_H
