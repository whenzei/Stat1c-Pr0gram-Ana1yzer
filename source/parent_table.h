#pragma once

#ifndef SPA_PARENT_TABLE_H
#define SPA_PARENT_TABLE_H

#include <string>
#include <list>
#include <unordered_map>

using std::string;
using std::list;
using std::unordered_map;

// StmtNum is defined as a string inside PKB, because query results have to be
// in the format list<string>. For consistency, all StmtNum inside PKB is string
// so that it is easier to search and update the PKB data structures.
using StmtNum = string;
using StmtNumList = list<string>;
using StmtListIndex = int;
using StmtListIndexList = list<int>;
using ParentsMap = unordered_map<int, list<string>>;
using DirectParentMap = unordered_map<int, string>;
using ChildrenMap = unordered_map<string, list<int>>;
using DirectChildrenMap = unordered_map<string, list<int>>;

// The parent table class for the PKB component
// Used to store (both direct and indirect) parent-children relationships
// between statements and statement list indices
class ParentTable {
  ParentsMap parents_map_;
  DirectParentMap direct_parent_map_;
  ChildrenMap children_map_;
  DirectChildrenMap direct_children_map_;

 public:
  // Insert a direct parent-child relationship into parents_map and children_map
  // @param stmt_num the statement number of the parent statement
  // @param stmtlist_index the statement list index of the child
  void InsertDirectParentRelationship(StmtNum stmt_num, StmtListIndex stmtlist_index);

  // Insert an indirect parent-child relationship into parents_map and children_map
  // @param stmt_num the statement number of the parent statement
  // @param stmtlist_index the statement list index of the child
  void InsertIndirectParentRelationship(StmtNum stmt_num, StmtListIndex stmtlist_index);

  // @returns true if the statement specified by stmt_num is the direct parent
  // of the statement list specified by stmtlist_index
  bool IsDirectParent(StmtNum stmt_num, StmtListIndex stmtlist_index);

  // @returns true if the statement specified by stmt_num is a parent (direct or
  // indirect) of the statement list specified by stmtlist_index
  bool IsParent(StmtNum stmt_num_1, StmtListIndex stmtlist_index);

  // @returns the statement number of the direct parent
  StmtNum GetDirectParent(StmtListIndex stmtlist_index);

  // @returns a list of statement numbers of the parents (direct + indirect)
  StmtNumList GetParents(StmtListIndex stmtlist_index);

  // @returns a list of statement list indices of the direct children
  StmtListIndexList GetDirectChildren(StmtNum stmt_num);

  // @returns a list of statement list indices of the children (direct +
  // indirect)
  StmtListIndexList GetChildren(StmtNum stmt_num);

  // @returns true if there exists any parent-child relationship in the table
  bool HasParentRelationship();
};

#endif !SPA_PARENT_TABLE_H