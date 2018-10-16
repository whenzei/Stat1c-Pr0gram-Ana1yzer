#pragma once

#ifndef SPA_STMT_TYPE_LIST_H
#define SPA_STMT_TYPE_LIST_H

#include <string>
#include <vector>
#include <unordered_map>

#include "pql_global.h"

// StmtNum is defined as a string inside PKB, because query results have to be
// in the format vector<string>. For consistency, all StmtNum inside PKB is string
// so that it is easier to search and update the PKB data structures.
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

using StmtNum = int;
using StmtNumList = vector<int>;
using StmtNumPairList = vector<pair<int, int>>;
using StmtTypeMap = unordered_map<PqlDeclarationEntity, vector<int>>;
using StmtTypePairMap = unordered_map<PqlDeclarationEntity, vector<pair<int, int>>>;

// The statement type list class for the PKB component
// Used to store the statement numbers that belong to each statement type (e.g.
// assign, if)
class StmtTypeList {
  StmtTypeMap stmt_type_map_;
  StmtTypePairMap stmt_type_twin_map_;

 public:
  // constructor
  StmtTypeList();

  // Insert a statement into the StmtTypeList using type provided
  // @param stmt_num the statement number of the statement to be inserted
  // @param stmt_type the PqlDeclarationEntity type of statement
  void InsertStmt(StmtNum stmt_num, PqlDeclarationEntity stmt_type);

  // Get the list of all statements
  StmtNumList GetAllStmt();

  // Get the list of all statements in pairs
  StmtNumPairList GetAllStmtTwin();

  // Get the list of all assign statements
  StmtNumList GetAllAssignStmt();

  // Get the list of all assign statements in pairs
  StmtNumPairList GetAllAssignStmtTwin();

  // Get the list of all while statements
  StmtNumList GetAllWhileStmt();

  // Get the list of all while statements in pairs
  StmtNumPairList GetAllWhileStmtTwin();

  // Get the list of all if statements
  StmtNumList GetAllIfStmt();

  // Get the list of all if statements in pairs
  StmtNumPairList GetAllIfStmtTwin();

  // Get the list of all read statements
  StmtNumList GetAllReadStmt();

  // Get the list of all read statements in pairs
  StmtNumPairList GetAllReadStmtTwin();

  // Get the list of all print statements
  StmtNumList GetAllPrintStmt();

  // Get the list of all print statements in pairs
  StmtNumPairList GetAllPrintStmtTwin();

  // Get the list of all call statements
  StmtNumList GetAllCallStmt();

  // Get the list of all call statements in pairs
  StmtNumPairList GetAllCallStmtTwin();
};

#endif !SPA_STMT_TYPE_LIST_H