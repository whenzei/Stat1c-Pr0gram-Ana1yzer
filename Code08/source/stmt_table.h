#pragma once

#ifndef SPA_STMT_TABLE_H
#define SPA_STMT_TABLE_H

#include <string>
#include <unordered_map>
#include "pql_global.h"

using std::pair;
using std::string;
using std::unordered_map;

// StmtNum is defined as a string inside PKB, because query results have to be
// in the format list<string>. For consistency, all StmtNum inside PKB is string
// so that it is easier to search and update the PKB data structures.
using StmtNum = string;
using StmtListIndex = int;
using StmtType = PqlDeclarationEntity;
using StmtMap = unordered_map <string, pair<StmtType, int>>;

// The statement table class for the PKB component
// Used to store statement number and the corresponding statement list index
// that are passed into PKB from the parser
class StmtTable {
  StmtMap stmt_map_;

 public:
  // Insert a statement into the StmtTable
  // Prerequisite: The stmt_num is not an empty string
  // @param stmt_num the statement number of the statement to be inserted
  // @param stmtlist_index the statement list index of the statement to be
  // inserted
  // @return true if the statement is not already in StmtTable and is inserted
  // into the table, false otherwise
  bool InsertStmt(StmtNum stmt_num, StmtType stmt_type, StmtListIndex stmtlist_index);

  // @return the statement list index of a given statement
  StmtListIndex GetStmtListIndex(StmtNum stmt_num);

  // @return the type of a given statement
  StmtType GetStmtType(StmtNum stmt_num);

  // @return true if stmt_num is in the stmt table
  bool IsStmtNum(StmtNum stmt_num);
};

#endif !SPA_STMT_TABLE_H