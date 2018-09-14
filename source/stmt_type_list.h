#pragma once

#ifndef SPA_STMT_TYPE_LIST_H
#define SPA_STMT_TYPE_LIST_H

#include <string>
#include <list>
#include <vector>

using std::string;
using std::list;
using std::vector;

// StmtNum is defined as a string inside PKB, because query results have to be
// in the format list<string>. For consistency, all StmtNum inside PKB is string
// so that it is easier to search and update the PKB data structures.
using StmtNum = string;
using StmtNumList = list<string>;
using StmtTypeVector = vector<list<string>>;

// The statement type list class for the PKB component
// Used to store the statement numbers that belong to each statement type (e.g. assign, if)
class StmtTypeList {
  StmtTypeVector stmt_type_vector_;

  enum StmtTypeEnum { 
	kAll,
    kAssign,
    kWhile,
	kIf,
	kRead,
	kPrint,
	kSizeOfVector
  };
  
 public:
  // constructor
  StmtTypeList();

  // Insert an assign statement into the StmtTypeList
  // @param stmt_num the statement number of the statement to be inserted
  void InsertAssignStmt(StmtNum stmt_num);

  // Insert a while statement into the StmtTypeList
  // @param stmt_num the statement number of the statement to be inserted
  void InsertWhileStmt(StmtNum stmt_num);

  // Insert an if statement into the StmtTypeList
  // @param stmt_num the statement number of the statement to be inserted
  void InsertIfStmt(StmtNum stmt_num);

  // Insert a read statement into the StmtTypeList
  // @param stmt_num the statement number of the statement to be inserted
  void InsertReadStmt(StmtNum stmt_num);

  // Insert a print statement into the StmtTypeList
  // @param stmt_num the statement number of the statement to be inserted
  void InsertPrintStmt(StmtNum stmt_num);

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