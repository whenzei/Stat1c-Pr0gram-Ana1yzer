#pragma once

#include <string>

using std::string;

#ifndef STMT_TABLE_H
#define STMT_TABLE_H

class StmtTable {
private:
	string* stmt_table;

public:
	// Constructor
	StmtTable();
	// Takes in the statement in string format and inserts it in the StmtTable.
	// Returns the index of the statement.
	int InsertStmt(string stmt);
	// Returns the statement with its index as the given statement number.
	// Returns -1 if given index is empty.
	string GetStmt(int stmt_num);
	// Returns all statements in the StmtTable.
	// Returns null if StmtTable is empty. 
	string* GetAllStmt();
};

#endif !STMT_TABLE_H