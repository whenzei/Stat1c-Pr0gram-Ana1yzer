#pragma once

#ifndef SPA_FOLLOWS_TABLE_H
#define SPA_FOLLOWS_TABLE_H

#include <unordered_map>
#include <unordered_set>
#include "result_hasher.h"

using std::make_pair;
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = int;
using StmtNumSet = unordered_set<int>;
using StmtNumPairSet = unordered_set<pair<StmtNum, StmtNum>, ResultHasher>;
using FollowsMap = unordered_map<int, unordered_set<int>>;
using FollowedByMap = unordered_map<int, unordered_set<int>>;

// The follows table class for the PKB component
// Used to store follows relationships between stmts that are passed into PKB
// from the parser
class FollowsTable {
  /*
  If stmt 1 -> 2, (-> denotes follows)
  stmt 1 is the 'followed' and stmt 2 is the 'follower'.
  */

  // Stores follows relationship in <key=follower, value=set of followed>
  FollowsMap direct_follows_map_;
  FollowsMap follows_map_;
  // Stores follows relationship in <key=followed, value=set of followers>
  FollowedByMap direct_followed_by_map_;
  FollowedByMap followed_by_map_;
  // Set of stmts that follow another stmt
  StmtNumSet follows_set_;
  // Set of stmts that are followed by another stmt
  StmtNumSet followed_by_set_;

 public:

  // Inserts a generic follows relationship to the follows map and followed by map.
  // Decides on its own whether it should be inserted as direct or indirect.
  // @params 2 stmt nums; stmt_1 that follows stmt_2
  void InsertFollows(StmtNum stmt_1, StmtNum stmt_2);

  // @returns true if follows*(stmt_num1, stmt_num2) holds
  bool IsFollows(StmtNum stmt_num1, StmtNum stmt_num2);

  // @returns true if follows(stmt_num1, stmt_num2) holds
  bool IsFollowsT(StmtNum stmt_num1, StmtNum stmt_num2);

  // @returns a set of statements that directly or indirectly follow stmt_num
  // can be empty
  StmtNumSet GetFollowsT(StmtNum stmt_num);

  // @returns a set of statements that directly follow stmt_num
  // conly one element or empty
  StmtNumSet GetFollows(StmtNum stmt_num);

  // @returns a set of all statements that follow some statement
  // can be empty
  StmtNumSet GetAllFollows();

  // @returns a set of statements that are directly or indirectly followed by
  // stmt_num can be empty
  StmtNumSet GetFollowedByT(StmtNum stmt_num);

  // @returns a set of statements that are directly followed by stmt_num
  // only one element or empty
  StmtNumSet GetFollowedBy(StmtNum stmt_num);

  // @returns a set of all statements that are followed by some statement
  // can be empty
  StmtNumSet GetAllFollowedBy();

  // @returns true if there are any follows relationships
  // @returns false if follows map is empty
  bool HasFollowsRelationship();

  // @returns a set of all (followee, follower) pairs (direct + indirect)
  // can be empty
  StmtNumPairSet GetAllFollowsTPair();

  // @returns a set of all (followee, follower) pairs (direct only)
  // can be empty
  StmtNumPairSet GetAllFollowsPair();
};

#endif !SPA_FOLLOWS_TABLE_H