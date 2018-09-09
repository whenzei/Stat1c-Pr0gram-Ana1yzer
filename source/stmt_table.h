#pragma once

#ifndef SPA_STMT_TABLE_H
#define SPA_STMT_TABLE_H

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

// StmtNum is defined as a string inside PKB, because query results have to be in the
// format list<string>. For consistency, all StmtNum inside PKB is string so that it 
// is easier to search and update the PKB data structures.
using StmtNum = string;
using StmtListIndex = int;
using StmtMap = unordered_map<string, int>;

// The statement table class for the PKB component
// Used to store statement number and the corresponding statement list index
// that are passed into PKB from the parser
class StmtTable {
  StmtMap stmt_map_;

public:
	
  // Insert a statement into the StmtTable
  // Prerequisite: The stmt_num is not an empty string
  // @param stmt_num the statement number of the statement to be inserted
  // @param stmtlist_index the statement list index of the statement to be inserted
  // @return true if the statement is not already in StmtTable and is inserted into
  // the table, false otherwise
  bool InsertStmt(StmtNum stmt_num, StmtListIndex stmtlist_index);

  // Get the statement list index of a given statement.
  StmtListIndex GetStmtListIndex(StmtNum stmt_num);
};

#endif !SPA_STMT_TABLE_H