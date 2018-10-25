#pragma once

#ifndef SPA_PROC_LIST_H
#define SPA_PROC_LIST_H

#include <string>
#include <unordered_map>
#include <vector>

using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

using ProcName = string;
using ProcIndex = int;
using ProcIndexList = vector<ProcIndex>;
using ProcIndexPairList = vector<pair<ProcIndex, ProcIndex>>;
using ProcIndexMap = unordered_map<ProcName, ProcIndex>;
using IndexProcMap = unordered_map<ProcIndex, ProcName>;

// The procedure list class for the PKB component
// Used to store procedure names that are passed into PKB from the parser
class ProcList {
  
 ProcIndexList proc_name_index_list_;
 ProcIndexPairList proc_name_index_twin_list_;
 // TODO: Depending on PQL's preferences, might have to add below
 // a list of Proc Indices and just return the entire list.
 ProcIndexMap proc_index_map_;
 IndexProcMap index_proc_map_;

 private:
  // Temporary
  int num_procs = 0;

 public:
  
  // inserts proc_name into the procedure list
  // @returns index of the inserted procedure name is successful
  // @returns -1 if procedure name is already in the procedure list
  int InsertProcName(ProcName proc_name);

  // @returns the list of all procedure names (can be empty)
  ProcIndexList GetAllProcName();

  // @returns true if proc_name exists in the proc list
  bool IsProcName(ProcIndex proc_index);

  // @returns the list of all procedure names in pairs (in each pair, the same procedure name is repeated)
  ProcIndexPairList GetAllProcNameTwin();

  // @returns an unordered map with Index to Proc mapping
  IndexProcMap GetIndexToProcMapping();

  // @returns an unordered map with Proc to Index mapping
  ProcIndexMap GetProcToIndexMapping();

  // @returns the corresponding procedure name
  ProcName GetProcName(ProcIndex index);

  // @returns the corresponding procedure name index
  ProcIndex GetProcIndex(ProcName proc_name);
};

#endif  // !SPA_PROC_LIST_H
