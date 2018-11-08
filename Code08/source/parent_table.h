#pragma once

#ifndef SPA_PARENT_TABLE_H
#define SPA_PARENT_TABLE_H

#include <unordered_map>
#include <unordered_set>

using std::make_pair;
using std::pair;
using std::unordered_map;
using std::unordered_set;

// StmtNum is defined as a integers inside the PKB,
// for faster processing and retrieval.
using StmtNum = int;
using StmtNumSet = unordered_set<int>;
using ParentsMap = unordered_map<int, StmtNumSet>;
using DirectParentMap = unordered_map<int, int>;
using ChildrenMap = unordered_map<int, StmtNumSet>;
using DirectChildrenMap = unordered_map<int, StmtNumSet>;
using StmtNumPairSet = unordered_set<pair<int, int>>;

// The parent table class for the PKB component
// Used to store (both direct and indirect) parent-children relationships
// between statements and statement list indices
class ParentTable {
  // Stores parents relationships as <key=child, value=parent>
  ParentsMap parents_map_;
  DirectParentMap direct_parent_map_;
  StmtNumSet parents_set_;
  // Stores parents relationships as <key=parent, value=child>
  ChildrenMap children_map_;
  DirectChildrenMap direct_children_map_;
  StmtNumSet children_set_;

 public:
  // Insert a direct parent-child relationship between parent_stmt_num and
  // child_stmt_num into direct_parents_map and direct_children_map
  void InsertDirectParentRelationship(StmtNum parent_stmt_num,
                                      StmtNum child_stmt_num);

  // Insert an indirect parent-child relationship between parent_stmt_num and
  // child_stmt_num into parents_map and children_map
  void InsertIndirectParentRelationship(StmtNum parent_stmt_num,
                                        StmtNum child_stmt_num);

  // @returns true if parent_stmt_num is the direct parent of child_stmt_num
  bool IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns true if parent_stmt_num is a direct or indirect parent of
  // child_stmt_num
  bool IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns the direct parent of child_stmt_num
  StmtNum GetParent(StmtNum child_stmt_num);

  // @returns a list of parents (direct + indirect) of child_stmt_num
  StmtNumSet GetParentT(StmtNum child_stmt_num);

  // @returns a list of all parents
  StmtNumSet GetAllParent();

  // @returns a list of direct children of parent_stmt_num
  StmtNumSet GetChild(StmtNum parent_stmt_num);

  // @returns a list of all children
  StmtNumSet GetAllChild();

  // @returns a list of direct + indirect children of parent_stmt_num
  StmtNumSet GetChildT(StmtNum parent_stmt_num);

  // @returns true if there exists any parent-child relationship in the table
  bool HasParentRelationship();

  // @returns a list of direct <parent_stmt_num, child_stmt_num> pairs
  StmtNumPairSet GetAllParentPair();

  // @returns a list of <parent_stmt_num, child_stmt_num> pairs
  StmtNumPairSet GetAllParentTPair();
};

#endif !SPA_PARENT_TABLE_H