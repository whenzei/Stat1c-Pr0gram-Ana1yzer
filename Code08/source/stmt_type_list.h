#pragma once

#ifndef SPA_STMT_TYPE_LIST_H
#define SPA_STMT_TYPE_LIST_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pql_global.h"
#include "result_hasher.h"

using std::pair;
using std::unordered_map;
using std::unordered_set;

using StmtNum = int;
using StmtNumSet = unordered_set<StmtNum>;
using StmtNumPairSet = unordered_set<pair<StmtNum, StmtNum>>;
using StmtType = PqlDeclarationEntity;
using StmtTypeMap = unordered_map<StmtType, StmtNumSet>;
using StmtTypePairMap =
    unordered_map<PqlDeclarationEntity, unordered_set<pair<StmtNum, StmtNum>>>;

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
  void InsertStmt(StmtNum stmt_num, StmtType stmt_type);

  // Get the list of all statements
  StmtNumSet GetAllStmt();

  // Get the list of all statements in pairs
  StmtNumPairSet GetAllStmtTwin();

  // Get the list of all assign statements
  StmtNumSet GetAllAssignStmt();

  // Get the list of all assign statements in pairs
  StmtNumPairSet GetAllAssignStmtTwin();

  // Get the list of all while statements
  StmtNumSet GetAllWhileStmt();

  // Get the list of all while statements in pairs
  StmtNumPairSet GetAllWhileStmtTwin();

  // Get the list of all if statements
  StmtNumSet GetAllIfStmt();

  // Get the list of all if statements in pairs
  StmtNumPairSet GetAllIfStmtTwin();

  // Get the list of all read statements
  StmtNumSet GetAllReadStmt();

  // Get the list of all read statements in pairs
  StmtNumPairSet GetAllReadStmtTwin();

  // Get the list of all print statements
  StmtNumSet GetAllPrintStmt();

  // Get the list of all print statements in pairs
  StmtNumPairSet GetAllPrintStmtTwin();

  // Get the list of all call statements
  StmtNumSet GetAllCallStmt();

  // Get the list of all call statements in pairs
  StmtNumPairSet GetAllCallStmtTwin();
};

#endif !SPA_STMT_TYPE_LIST_H