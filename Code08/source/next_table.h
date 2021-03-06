#pragma once

#ifndef SPA_NEXT_TABLE_H
#define SPA_NEXT_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

using std::pair;
using std::string;
using std::to_string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = int;
using ProcName = string;
using StmtNumList = vector<int>;
using StmtNumSet = unordered_set<int>;
using StmtNumPairList = vector<pair<int, int>>;
using CFG = Graph;
using CFGTable = unordered_map<string, CFG>;

// The next table class for the PKB component
// Used to store next relationships between stmts that are passed into PKB
// from the parser
class NextTable {
  CFG combined_cfg_;
  CFG reversed_combined_cfg_;
  CFG program_cfg_;
  CFG reversed_program_cfg_;
  CFGTable cfg_table_;
  CFGTable reversed_cfg_table_;
  StmtNumList previous_list_;
  StmtNumSet previous_set_;
  StmtNumList next_list_;
  StmtNumSet next_set_;
  StmtNumPairList next_pair_list_;

 public:
  void InsertCFG(ProcName proc_name);

  void InsertNext(ProcName proc_name, StmtNum previous_stmt_int,
                  StmtNum next_stmt_int);

  // adds the statement into the CFG node
  void InsertStatement(ProcName proc_name, StmtNum stmt_num);

  CFG* GetCFG(ProcName proc_name);

  CFG* GetReverseCFG(ProcName proc_name);

  bool IsNext(StmtNum previous_stmt, StmtNum next_stmt);

  bool IsNext(StmtNum stmt_num);

  bool IsPrevious(StmtNum stmt_num);

  StmtNumList GetNext(StmtNum stmt_num);

  StmtNumList GetPrevious(StmtNum stmt_num);

  StmtNumList GetAllNext();

  StmtNumList GetAllPrevious();

  StmtNumPairList GetAllNextPairs();

  CFG* GetCombinedCFG();

  CFG* GetReverseCombinedCFG();

  void SetAllNextPairs(StmtNumPairList next_pair_list);

  bool HasNextRelationship();

  /*****************************
   * AffectsBip Table Functions *
   *****************************/
  // Sets the program_cfg_ to the given program_cfg
  void SetProgramCFG(const CFG& program_cfg);

  // Sets the reversed_program_cfg_ to the given reversed_program_cfg
  void SetReverseProgramCFG(const CFG& reversed_program_cfg);

  // @returns the program cfg
  CFG* GetProgramCFG();

  // @returns the reversed program cfg
  CFG* GetReverseProgramCFG();
};

#endif !SPA_NEXT_TABLE_H
