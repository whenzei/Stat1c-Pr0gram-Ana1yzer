#pragma once

#ifndef SPA_CALL_TABLE_H
#define SPA_CALL_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = int;
using ProcIndex = int;
using ProcName = string;
using ProcIndexList = vector<ProcIndex>;
using ProcIndexSet = unordered_set<ProcIndex>;
using ProcNameSet = unordered_set<ProcName>;
using ProcIndexPairList = vector<pair<ProcIndex, ProcIndex>>;
using ProcIndexPairSet = unordered_set<pair<ProcIndex, ProcIndex>>;
// int can be StmtNum or ProcIndex.
using CallMap = unordered_map<int, unordered_set<ProcIndex>>;
using StmtNumProcPairList = vector<pair<StmtNum, ProcIndex>>;
using StmtNumProcPairSet = unordered_set<pair<StmtNum, ProcIndex>>;
using StmtNumList = vector<StmtNum>;
using StmtNumSet = unordered_set<StmtNum>;
using StmtProcMap = unordered_map<StmtNum, ProcIndex>;

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
  ProcIndexPairSet
      callee_twin_set_;     // stores procs called by any other proc (in pairs)
  ProcIndexSet callee_set_;  // stores procs called by any other proc
  StmtProcMap stmt_to_call_table_;  // retrieve the procedure name called at StmtNum

 public:
  // PROC-PROC RELATIONSHIP INSERT
  // Inserts an INDIRECt caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false if relationship
  // already exists or if insertion fails.
  // @params caller procedure name and callee procedure name
  bool InsertIndirectCallRelationship(ProcIndex caller_proc,
                                      ProcIndex callee_proc);

  // Inserts a DIRECT caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false if relationship
  // already exists or if insertion fails.
  // @params caller procedure name and callee procedure name
  bool InsertDirectCallRelationship(ProcIndex caller_proc, ProcIndex callee_proc);

  // STATEMENT-PROC RELATIONSHIP INSERT
  // Inserts a calls relationship to the call table.
  // @params stmt num of statement
  // @params proc name of the procedure *being called*
  void InsertCalls(StmtNum stmt_num, ProcIndex callee_proc);

  // Returns the called procedure name at given StmtNum
  // @params: StmtNum the stmt num to retrieve the procedure name
  // @returns procedure name called at the given statement if exists, empty
  // string otherwise
  ProcIndex GetCalledProcedure(StmtNum stmt_num);

  // Finds and returns a set of stmt numbers calling the given proc.
  // @params proc name of the procedure being called
  // @returns a set of stmt numbers (can be empty)
  StmtNumSet GetCallingStmts(ProcIndex callee_proc);

  // Finds and returns a set of pairs of all stmt numbers
  // and procedures called by that stmt number.
  // @returns a set of <stmt_num, proc> pairs
  StmtNumProcPairSet GetAllCallingStmtPairs();

  // Finds and returns the direct callee for given procedure.
  // @returns a set containing one direct callee (can be empty)
  // @params caller procedure name
  ProcIndexSet GetCallee(ProcIndex caller_proc);

  // Finds and returns all callees for given procedure.
  // @returns a set containing all callees for given proc (can be empty)
  // @params caller procedure name
  ProcIndexSet GetCalleeT(ProcIndex caller_proc);

  // Finds and returns the direct caller for given procedure.
  // @returns a set containing one direct caller (can be empty)
  // @params callee procedure name
  ProcIndexSet GetCaller(ProcIndex callee_proc);

  // Finds and returns all callers for given procedure.
  // @returns a set containing all callers for given proc (can be empty)
  // @params callee procedure name
  ProcIndexSet GetCallerT(ProcIndex callee_proc);

  // @returns a set of all procedures calling some other proc (can be empty)
  ProcIndexSet GetAllCaller();

  // @returns a set of all procedures being called by some other proc (can be empty)
  ProcIndexSet GetAllCallee();

  // @returns a set of all procedures being called by some other proc (can be empty)
  ProcIndexPairSet GetAllCalleeTwin();

  // @returns a set of all <caller, direct callee> pairs
  ProcIndexPairSet GetAllCallPairs();

  // @returns a set of all <caller, callee> pairs
  ProcIndexPairSet GetAllCallTPairs();

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