#pragma once
#include "statement.h"

#include <string>

using std::string;

const int kTempArraySize = 10;

#ifndef STMT_TABLE_H
#define STMT_TABLE_H

class StmtTable {
private:
	Statement* stmt_table[kTempArraySize];
	int num_stmt;

public:
	// Constructor
	StmtTable();

	// Destructor
	~StmtTable();
	
	// Takes in the statement in string format and inserts it in the StmtTable.
	// Returns the index of the statement.
	int InsertStmt(int line_num, string stmt);

	// Returns a pointer to a list of all line numbers of all statements in the StmtTable.
	// Returns a pointer to an empty list if StmtTable is empty. 
	int* GetAllStmtNums();
};

#endif !STMT_TABLE_H