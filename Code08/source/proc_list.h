#pragma once

#ifndef SPA_PROC_LIST_H
#define SPA_PROC_LIST_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using ProcName = string;
using ProcNameList = vector<string>;
using ProcNamePairList = vector<pair<string, string>>;
using ProcNameSet = unordered_set<string>;
using ProcIndexMap = unordered_map<string, int>;
using IndexProcMap = unordered_map<int, string>;

// The procedure list class for the PKB component
// Used to store procedure names that are passed into PKB from the parser
class ProcList {
  
 ProcNameList proc_name_list_;
 ProcNamePairList proc_name_twin_list_;
 ProcNameSet proc_name_set_;
 // TODO: Depending on PQL's preferences, might have to add below
 // a list of Proc Indices and just return the entire list.
 ProcIndexMap proc_index_map_;
 IndexProcMap index_proc_map_;

 private:
  // Temporary
  int num_procs = 0;

 public:
  
  // inserts proc_name into the procedure list
  void InsertProcName(ProcName proc_name);

  // @returns the list of all procedure names (can be empty)
  ProcNameList GetAllProcName();

  // @returns true if proc_name exists in the proc list
  bool IsProcName(ProcName proc_name);

  // @returns the list of all procedure names in pairs (in each pair, the same procedure name is repeated)
  ProcNamePairList GetAllProcNameTwin();

  // @returns an unordered map with Index to Proc mapping
  IndexProcMap GetIndexToProcMapping();

  // @returns an unordered map with Proc to Index mapping
  ProcIndexMap GetProcToIndexMapping();
};

#endif  // !SPA_PROC_LIST_H
