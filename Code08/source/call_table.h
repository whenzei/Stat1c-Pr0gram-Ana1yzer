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

using CallMap = unordered_map<string, vector<string>>;
using StmtNum = string;
using StmtNumList = vector<string>;
using StmtNumProcPairList = vector<pair<string, string>>;
using ProcName = string;
using ProcNameList = vector<string>;
using ProcNameSet = unordered_set<string>;
using ProcNamePairList = vector<pair<string, string>>;

class CallTable {
  CallMap call_table_; // stores <proc calling, proc called>
  CallMap direct_call_table_; // stores <direct proc calling, proc called>
  CallMap callee_table_; // stores <proc called by, proc calling>
  CallMap direct_callee_table_; // stores <direct proc called by, proc calling>
  
  CallMap stmt_num_proc_table_; // stores <procedure called, stmt_nums calling it>

  ProcNameList caller_list_;  // stores procs calling any other proc
  ProcNameSet caller_set_; // stores procs calling any other proc
  ProcNameList callee_list_; // stores procs called by any other proc
  ProcNamePairList callee_twin_list_; // stores procs called by any other proc (in pairs)
  ProcNameSet callee_set_; // stores procs called by any other proc

public:
  // PROC-PROC RELATIONSHIP INSERT
  // Inserts an INDIRECt caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false if relationship
  // already exists or if insertion fails.
  // @params caller procedure name and callee procedure name
  bool InsertIndirectCallRelationship(ProcName caller_proc, ProcName callee_proc);

  // Inserts a DIRECT caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false if relationship
  // already exists or if insertion fails.
  // @params caller procedure name and callee procedure name
  bool InsertDirectCallRelationship(ProcName caller_proc, ProcName callee_proc);

  // STATEMENT-PROC RELATIONSHIP INSERT
  // Inserts a calls relationship to the call table.
  // @params stmt num of statement
  // @params proc name of the procedure *being called*
  void InsertCalls(StmtNum stmt_num, ProcName callee_proc);

  // Finds and returns a list of stmt numbers calling the given proc.
  // @params proc name of the procedure being called
  // @returns a list of stmt numbers (can be empty)
  StmtNumList GetCallingStmts(ProcName callee_proc);

  // Finds and returns a list of pairs of all stmt numbers
  // and procedures called by that stmt number.
  // @returns a list of <stmt_num, proc> pairs
  StmtNumProcPairList GetAllCallingStmtPairs();

  // Finds and returns the direct callee for given procedure.
  // @returns a list containing one direct callee (can be empty)
  // @params caller procedure name
  ProcNameList GetCallee(ProcName caller_proc);

  // Finds and returns all callees for given procedure.
  // @returns a list containing all callees for given proc (can be empty)
  // @params caller procedure name 
  ProcNameList GetCalleeT(ProcName caller_proc);

  // Finds and returns the direct caller for given procedure.
  // @returns a list containing one direct caller (can be empty)
  // @params callee procedure name
  ProcNameList GetCaller(ProcName callee_proc);

  // Finds and returns all callers for given procedure.
  // @returns a list containing all callers for given proc (can be empty)
  // @params callee procedure name 
  ProcNameList GetCallerT(ProcName callee_proc);

  // @returns all procedures calling some other proc (can be empty)
  ProcNameList GetAllCaller();

  // @returns all procedures being called by some other proc (can be empty)
  ProcNameList GetAllCallee();

  // @returns all procedures being called by some other proc (can be empty)
  ProcNamePairList GetAllCalleeTwin();

  // @returns a list of all <caller, direct callee> pairs
  ProcNamePairList GetAllCallPairs();

  // @returns a list of all <caller, callee> pairs
  ProcNamePairList GetAllCallTPairs();

  // @returns true if Call(caller, callee) is true
  bool IsCall(ProcName caller_proc, ProcName callee_proc);

  // @returns true if Call*(caller, callee) is true
  bool IsCallT(ProcName caller_proc, ProcName callee_proc);

  // @returns true if Call Table has any calls relationships
  // false if otherwise
  bool HasCallsRelationship();

};

#endif !SPA_CALL_TABLE_H