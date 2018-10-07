#pragma once

#ifndef SPA_NEXT_TABLE_H
#define SPA_NEXT_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNumInt = int;
using ProcName = string;
using StmtNumIntList = vector<int>;
using StmtNumIntSet = unordered_set<int>;
using CFG = unordered_map<int, vector<int>>;
using CFGTable = unordered_map<string, pair<CFG, CFG>>;

// The next table class for the PKB component
// Used to store next relationships between stmts that are passed into PKB
// from the parser
class NextTable {
  CFG cfg_;
  CFG reverse_cfg_;
  CFGTable cfg_table_;
  StmtNumIntList previous_list_;
  StmtNumIntSet previous_set_;
  StmtNumIntList next_list_;
  StmtNumIntSet next_set_;

 public:
  void InsertCFG(ProcName proc_name);

  void InsertNext(ProcName proc_name, StmtNumInt previous_stmt, StmtNumInt next_stmt);

  CFG* GetCFG(ProcName proc_name);

  //TODO: add getter methods, check with pql whether to return string or int
};

#endif !SPA_NEXT_TABLE_H