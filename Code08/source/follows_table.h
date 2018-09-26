#pragma once

#ifndef SPA_FOLLOWS_TABLE_H
#define SPA_FOLLOWS_TABLE_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using std::make_pair;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::pair; 
using std::vector;

using StmtNum = string;
using StmtNumList = vector<string>;
using StmtNumSet = unordered_set<string>;
using StmtNumPairList = vector<pair<StmtNum, StmtNum>>;
using FollowsMap = unordered_map<string, vector<string>>;
using FollowedByMap = unordered_map<string, vector<string>>;

// The follows table class for the PKB component
// Used to store follows relationships between stmts that are passed into PKB
// from the parser
class FollowsTable {
  // Stores follows relationship in <key=follower, value=followed_by>
  FollowsMap follows_map_;
  // Stores follows relationship in <key=followed by, value=follower>
  FollowedByMap followed_by_map_;
  StmtNumList follows_list_;
  StmtNumSet follows_set_;
  StmtNumList followed_by_list_;
  StmtNumSet followed_by_set_;

public:
  // Inserts a follows relationship to the follows map and followed by map.
  // @params 2 stmt nums; stmt_num2 that follows stmt_num1
  void InsertFollows(StmtNum stmt_num1, StmtNum stmt_num2);

  // @returns true if follows*(stmt_num1, stmt_num2) holds
  bool IsFollows(StmtNum stmt_num1, StmtNum stmt_num2);

  // @returns true if follows(stmt_num1, stmt_num2) holds
  bool IsFollowsT(StmtNum stmt_num1, StmtNum stmt_num2);

  // @returns a list of statements that directly or indirectly follow stmt_num 
  // can be empty
  StmtNumList GetFollowsT(StmtNum stmt_num);

  // @returns a list of statements that directly follow stmt_num 
  // conly one element or empty
  StmtNumList GetFollows(StmtNum stmt_num);

  // @returns a list of all statements that follow some statement
  // can be empty
  StmtNumList GetAllFollows();

  // @returns a list of statements that are directly or indirectly followed by stmt_num
  // can be empty
  StmtNumList GetFollowedByT(StmtNum stmt_num);

  // @returns a list of statements that are directly followed by stmt_num 
  // only one element or empty
  StmtNumList GetFollowedBy(StmtNum stmt_num);

  // @returns a list of all statements that are followed by some statement
  // can be empty
  StmtNumList GetAllFollowedBy();

  // @returns true if there are any follows relationships
  // @returns false if follows map is empty
  bool HasFollowsRelationship();

  // @returns a list of all (followee, follower) pairs (direct + indirect)
  // can be empty
  StmtNumPairList GetAllFollowsTPair();

  // @returns a list of all (followee, follower) pairs (direct only)
  // can be empty
  StmtNumPairList GetAllFollowsPair();
};

#endif !SPA_FOLLOWS_TABLE_H