#pragma once

#ifndef SPA_NEXT_TABLE_H
#define SPA_NEXT_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::to_string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNumInt = int;
using StmtNum = string;
using ProcName = string;
using StmtNumList = vector<string>;
using StmtNumSet = unordered_set<string>;
using StmtNumPairList = vector<pair<string, string>>;
using CFG = unordered_map<string, vector<string>>;
using CFGTable = unordered_map<string, pair<CFG, CFG>>;

// The next table class for the PKB component
// Used to store next relationships between stmts that are passed into PKB
// from the parser
class NextTable {
  CFG combined_cfg_;
  CFG combined_reverse_cfg_;
  CFGTable cfg_table_;
  StmtNumList previous_list_;
  StmtNumSet previous_set_;
  StmtNumList next_list_;
  StmtNumSet next_set_;

 public:
  void InsertCFG(ProcName proc_name);

  void InsertNext(ProcName proc_name, StmtNumInt previous_stmt_int, StmtNumInt next_stmt_int);

  CFG* GetCFG(ProcName proc_name);

  bool IsNext(StmtNum previous_stmt, StmtNum next_stmt);

  bool IsNext(StmtNum stmt_num);

  bool IsPrevious(StmtNum stmt_num);

  StmtNumList GetNext(StmtNum stmt_num);

  StmtNumList GetPrevious(StmtNum stmt_num);

  StmtNumList GetAllNext();

  StmtNumList GetAllPrevious();

  StmtNumPairList GetAllNextPairs();

  bool HasNextRelationship();
};

#endif !SPA_NEXT_TABLE_H