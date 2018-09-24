#pragma once

#ifndef SPA_PARENT_TABLE_H
#define SPA_PARENT_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// StmtNum is defined as a string inside PKB, because query results have to be
// in the format list<string>. For consistency, all StmtNum inside PKB is string
// so that it is easier to search and update the PKB data structures.
using StmtNum = string;
using StmtNumList = vector<string>;
using ParentsMap = unordered_map<string, vector<string>>;
using DirectParentMap = unordered_map<string, string>;
using ParentsSet = unordered_set<string>;
using ParentsList = vector<string>;
using ChildrenMap = unordered_map<string, vector<string>>;
using DirectChildrenMap = unordered_map<string, vector<string>>;
using ChildrenSet = unordered_set<string>;
using ChildrenList = vector<string>;
using StmtNumPairList = vector<pair<string, string>>;

// The parent table class for the PKB component
// Used to store (both direct and indirect) parent-children relationships
// between statements and statement list indices
class ParentTable {
  // Stores parents relationships as <key=child, value=parent>
  ParentsMap parents_map_;
  DirectParentMap direct_parent_map_;
  ParentsSet parents_set_;
  ParentsList parents_list_;
  // Stores parents relationships as <key=parent, value=child>
  ChildrenMap children_map_;
  DirectChildrenMap direct_children_map_;
  ChildrenSet children_set_;
  ChildrenList children_list_;

 public:
  // Insert a direct parent-child relationship into parents_map and children_map
  // @param stmt_num the statement number of the parent statement
  // @param stmtlist_index the statement list index of the child
  void InsertDirectParentRelationship(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // Insert an indirect parent-child relationship into parents_map and children_map
  // @param stmt_num the statement number of the parent statement
  // @param stmtlist_index the statement list index of the child
  void InsertIndirectParentRelationship(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns true if the statement specified by stmt_num is the direct parent
  // of the statement list specified by stmtlist_index
  bool IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns true if the statement specified by stmt_num is a parent (direct or
  // indirect) of the statement list specified by stmtlist_index
  bool IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns the statement number of the direct parent
  StmtNum GetParent(StmtNum child_stmt_num);

  // @returns a list of statement numbers of the parents (direct + indirect) of the give stmtlist index
  StmtNumList GetParentT(StmtNum child_stmt_num);

  // @returns a list of statement numbers of the statements that are parents to some statement
  ParentsList GetAllParent();

  // @returns a list of statement list indices of the direct children of the given statement
  StmtNumList GetChild(StmtNum parent_stmt_num);

  // @returns a list of statement list indices of the children (direct +
  // indirect) of the given statement
  ChildrenList GetAllChild();

  // @returns a list of statement list indices of the statements that are the children (direct +
  // indirect) of some statement
  StmtNumList GetChildT(StmtNum parent_stmt_num);

  // @returns true if there exists any parent-child relationship in the table
  bool HasParentRelationship();

  // @returns the direct_parent_map_
  DirectParentMap GetDirectParentMap();

  // @returns the parents_map_ 
  ParentsMap GetParentsMap();
};

#endif !SPA_PARENT_TABLE_H