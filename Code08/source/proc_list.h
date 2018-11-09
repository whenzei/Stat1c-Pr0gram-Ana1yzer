#pragma once

#ifndef SPA_PROC_LIST_H
#define SPA_PROC_LIST_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "result_hasher.h"

using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using ProcName = string;
using ProcIndex = int;
using ProcIndexSet = unordered_set<ProcIndex>;
using ProcNameSet = unordered_set<ProcName>;
using ProcIndexPairSet = unordered_set<pair<ProcIndex, ProcIndex>, ResultHasher>;
using ProcNamePairSet = unordered_set<pair<ProcName, ProcName>, ResultHasher>;
using ProcIndexMap = unordered_map<ProcName, ProcIndex>;
using IndexProcMap = unordered_map<ProcIndex, ProcName>;

// The procedure list class for the PKB component
// Used to store procedure names that are passed into PKB from the parser
class ProcList {
  ProcIndexSet proc_id_set_;
  ProcIndexPairSet proc_id_twin_set_;
  ProcNameSet proc_name_set_;
  ProcNamePairSet proc_name_twin_set_;
  // TODO: Depending on PQL's preferences, might have to add below
  // a list of Proc Indices and just return the entire list.
  ProcIndexMap proc_id_map_;
  IndexProcMap id_proc_map_;

 private:
  // Temporary
  int num_procs = 0;

 public:
  // inserts proc_name into the procedure list
  // @returns index of the inserted procedure name is successful
  // @returns -1 if procedure name is already in the procedure list
  int InsertProcName(ProcName proc_name);

  // @returns the list of all procedure indices (can be empty)
  ProcIndexSet GetAllProcIndices();

  // @returns the list of all procedure indices (can be empty)
  ProcNameSet GetAllProcNames();

  // @returns true if proc_name exists in the proc list
  bool IsProcName(ProcName proc_name);

  // @returns true if proc_index exists in the proc list
  bool IsProcIndex(ProcIndex proc_id);

  // @returns the list of all procedure names in pairs (in each pair, the same
  // procedure name is repeated)
  ProcNamePairSet GetAllProcNameTwin();

  // @returns the list of all procedure indices in pairs (in each pair, the same
  // procedure index is repeated)
  ProcIndexPairSet GetAllProcIndexTwin();

  // @returns an unordered map with Index to Proc mapping
  IndexProcMap GetIndexToProcMapping();

  // @returns an unordered map with Proc to Index mapping
  ProcIndexMap GetProcToIndexMapping();

  // @returns the corresponding procedure name
  ProcName GetProcName(ProcIndex proc_id);

  // @returns the corresponding procedure name index
  ProcIndex GetProcIndex(ProcName proc_name);
};

#endif  // !SPA_PROC_LIST_H
