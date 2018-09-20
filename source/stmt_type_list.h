#pragma once

#ifndef SPA_STMT_TYPE_LIST_H
#define SPA_STMT_TYPE_LIST_H

#include <list>
#include <string>
#include <vector>

using std::list;
using std::string;
using std::vector;

// StmtNum is defined as a string inside PKB, because query results have to be
// in the format vector<string>. For consistency, all StmtNum inside PKB is string
// so that it is easier to search and update the PKB data structures.
using StmtNum = string;
using StmtNumList = vector<string>;
using StmtTypeVector = vector<vector<string>>;

// The statement type list class for the PKB component
// Used to store the statement numbers that belong to each statement type (e.g.
// assign, if)
class StmtTypeList {
  StmtTypeVector stmt_type_vector_;

 public:
  enum StmtTypeEnum {
    kAll,
    kAssign,
    kWhile,
    kIf,
    kRead,
    kPrint,
    kSizeOfVector
  };
  // constructor
  StmtTypeList();

  // Insert a statement into the StmtTypeList using type provided
  // @param stmt_num the statement number of the statement to be inserted
  // @param stmt_type the type of statement
  void InsertStmt(StmtNum stmt_num, StmtTypeEnum stmt_type);

  // Get the list of all statement numbers
  StmtNumList GetAllStmt();

  // Get the list of all assign statement numbers
  StmtNumList GetAllAssignStmt();

  // Get the list of all while statement numbers
  StmtNumList GetAllWhileStmt();

  // Get the list of all if statement numbers
  StmtNumList GetAllIfStmt();

  // Get the list of all read statement numbers
  StmtNumList GetAllReadStmt();

  // Get the list of all print statement numbers
  StmtNumList GetAllPrintStmt();
};

#endif !SPA_STMT_TYPE_LIST_H