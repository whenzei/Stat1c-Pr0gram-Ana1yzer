#pragma once

#ifndef SPA_CALL_TABLE_H
#define SPA_CALL_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "proc_list.h"

using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = int;
using ProcIndex = int;
using ProcName = string;
// int can be StmtNum or ProcIndex.
using CallMap = unordered_map<int, vector<ProcIndex>>;
using StmtNumProcPairList = vector<pair<StmtNum, ProcIndex>>;
using ProcIndexSet = unordered_set<ProcIndex>;
using StmtNumList = vector<StmtNum>;

class CallTable {
  CallMap call_table_;           // stores <proc calling, proc called>
  CallMap direct_call_table_;    // stores <direct proc calling, proc called>
  CallMap callee_table_;         // stores <proc called by, proc calling>
  CallMap direct_callee_table_;  // stores <direct proc called by, proc calling>
  CallMap
      stmt_num_proc_table_;  // stores <procedure called, stmt_nums calling it>

  ProcIndexList caller_list_;  // stores procs calling any other proc
  ProcIndexSet caller_set_;    // stores procs calling any other proc
  ProcIndexList callee_list_;  // stores procs called by any other proc
  ProcIndexPairList
      callee_twin_list_;     // stores procs called by any other proc (in pairs)
  ProcIndexSet callee_set_;  // stores procs called by any other proc
  ProcList proc_list_;
  unordered_map<StmtNum, ProcIndex>
      stmt_to_call_table_;  // retrieve the procedure name called at StmtNum

  // @returns true if the proc_name is found in the proc_list, false otherwise
  bool IsValidProcName(ProcName proc_name);

 public:
  // PROC-PROC RELATIONSHIP INSERT
  // Inserts an INDIRECt caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false if relationship
  // already exists or if insertion fails.
  // @params caller procedure name and callee procedure name
  bool InsertIndirectCallRelationship(ProcName caller_proc,
                                      ProcName callee_proc);

  // Inserts a DIRECT caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false if relationship
  // already exists or if insertion fails.
  // @params caller procedure name and callee procedure name
  bool InsertDirectCallRelationship(ProcName caller_proc, ProcName callee_proc);

  // STATEMENT-PROC RELATIONSHIP INSERT
  // Inserts a calls relationship to the call table.
  // @params stmt num of statement
  // @params proc name of the procedure *being called*
  void InsertCalls(StmtNum stmt_num, ProcIndex callee_proc);

  // Returns the called procedure name at given StmtNum
  // @params: StmtNum the stmt num to retrieve the procedure name
  // @returns procedure name called at the given statement if exists, empty
  // string otherwise
  ProcName GetCalledProcedure(StmtNum stmt_num);

  // Finds and returns a list of stmt numbers calling the given proc.
  // @params proc name of the procedure being called
  // @returns a list of stmt numbers (can be empty)
  StmtNumList GetCallingStmts(ProcIndex callee_proc);

  // Finds and returns a list of pairs of all stmt numbers
  // and procedures called by that stmt number.
  // @returns a list of <stmt_num, proc> pairs
  StmtNumProcPairList GetAllCallingStmtPairs();

  // Finds and returns the direct callee for given procedure.
  // @returns a list containing one direct callee (can be empty)
  // @params caller procedure name
  ProcIndexList GetCallee(ProcIndex caller_proc);

  ProcNameList GetCallee(ProcName caller_proc);

  // Finds and returns all callees for given procedure.
  // @returns a list containing all callees for given proc (can be empty)
  // @params caller procedure name
  ProcIndexList GetCalleeT(ProcIndex caller_proc);

  // Finds and returns the direct caller for given procedure.
  // @returns a list containing one direct caller (can be empty)
  // @params callee procedure name
  ProcIndexList GetCaller(ProcIndex callee_proc);

  // Finds and returns all callers for given procedure.
  // @returns a list containing all callers for given proc (can be empty)
  // @params callee procedure name
  ProcIndexList GetCallerT(ProcIndex callee_proc);

  // @returns all procedures calling some other proc (can be empty)
  ProcIndexList GetAllCaller();

  // @returns all procedures being called by some other proc (can be empty)
  ProcIndexList GetAllCallee();

  // @returns all procedures being called by some other proc (can be empty)
  ProcIndexPairList GetAllCalleeTwin();

  // @returns a list of all <caller, direct callee> pairs
  ProcIndexPairList GetAllCallPairs();

  // @returns a list of all <caller, callee> pairs
  ProcIndexPairList GetAllCallTPairs();

  // @returns true if Call(caller, callee) is true
  bool IsCall(ProcIndex caller_proc, ProcIndex callee_proc);

  // @returns true if Call*(caller, callee) is true
  bool IsCallT(ProcIndex caller_proc, ProcIndex callee_proc);

  // @returns true if callee_proc is called by any other procedure
  bool IsCalledProc(ProcIndex callee_proc);

  // @returns true if Call Table has any calls relationships
  // false if otherwise
  bool HasCallsRelationship();
};

#endif !SPA_CALL_TABLE_H