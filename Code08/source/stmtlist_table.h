#pragma once

#ifndef SPA_STMTLIST_TABLE_H
#define SPA_STMTLIST_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::pair;
using std::unordered_map;
using std::vector;

// StmtNum is defined as a string inside PKB, because query results have to be
// in the format vector<string>. For consistency, all StmtNum inside PKB is string
// so that it is easier to search and update the PKB data structures.
using StmtNum = string;
using StmtNumList = vector<string>;
using StmtListIndex = int;
using StmtListMap = unordered_map<int, vector<string>>;

// The statement list table class for the PKB component
// Used to store statement list indices and corresponding statement numbers
class StmtListTable {
  StmtListMap stmtlist_map_;

 public:
  // Insert a statement into the StmtListTable
  // @param stmt_num the statement number of the statement to be inserted
  // @param stmtlist_index the statement list index of the statement to be
  // inserted
  void InsertStmt(StmtNum stmt_num, StmtListIndex stmtlist_index);

  // Get all statement numbers that belong to the statement list specified by
  // stmtlist_index
  StmtNumList GetStmtNumList(StmtListIndex stmtlist_index);
};

#endif !SPA_STMTLIST_TABLE_H