#pragma once

#ifndef SPA_FOLLOWS_TABLE_H
#define SPA_FOLLOWS_TABLE_H

#include <list>
#include <unordered_map>
#include <utility>
#include <string>

using std::list;
using std::make_pair;
using std::unordered_map;
using std::string;
using std::pair; 

using StmtNum = string;
using StmtList = list<string>;
using StmtNumPairList = list<pair<StmtNum, StmtNum>>;
using FollowsMap = unordered_map<string, list<string>>;
using FollowedByMap = unordered_map<string, list<string>>;

class FollowsTable {
  FollowsMap follows_map_;
  FollowedByMap followed_by_map_;

private:
  // Utility function to check if followed by map contains any
  // followed by relationships.
  // @returns true if there are any followed by relationships
  // @returns false if followed by map is empty
  bool HasFollowedBy();

  // Utility function to get the size of follows map
  // @returns size of follows map
  int GetFollowsMapSize();

public:
  // Inserts a follows relationship to the follows map and followed by map.
  // @params 2 stmt nums; stmt_num2 that follows stmt_num1
  // @returns true if relationship is successfully added to both maps
  // @returns false if otherwise
  bool InsertFollows(StmtNum stmt_num1, StmtNum stmt_num2);

  // Checks if stmt s2 follows stmt s1. Includes transitive follows.
  // @params
  // @returns true if follows*(s1, s2) holds
  // @returns false if otherwise
  bool IsFollows(StmtNum stmt_num1, StmtNum stmt_num2);

  // Checks if stmt s2 is a direct follower of stmt s1.
  // @params
  // @returns true if follows(s1, s2) holds
  // @returns false if otherwise
  bool IsFollowsT(StmtNum stmt_num1, StmtNum stmt_num2);

  // Finds and returns a list of statements that follow the given statement.
  // Transitive relationships are included.
  // @params
  // @returns a list of statement numbers of statements that follow s, if any
  // list can be empty
  StmtList GetFollows(StmtNum stmt_num);

  // Finds and returns a stmt number for the stmt that directly follows the
  // given statement. Only the direct follower is returned.
  // @params
  // @returns a stmt_number if a direct follower exists
  // @returns -1 if otherwise
  StmtNum GetFollowsT(StmtNum stmt_num);

  // Finds list of statements that are followed by the given statement.
  // Transitive relationships are included.
  // @params
  // @returns a list of statement numbers of statements that are followed by s
  // list can be empty
  StmtList GetFollowedBy(StmtNum stmt_num);

  // Finds and returns a stmt number for the stmt that is directly followed
  // by the given statement. Only the direct followee is returned.
  // @params
  // @returns a stmt_number if a direct followee exists
  // @returns -1 if otherwise
  StmtNum GetFollowedByT(StmtNum stmt_num);

  // Checks and returns if follows map contains any follows relationships.
  // @returns true if there are any follows relationships
  // @returns false if follows map is empty
  bool HasFollowsRelationship();

  // Returns a list of (followee, follower) pair of statement numbers.
  // @returns list of pairs of stmt_nums (may be empty)
  StmtNumPairList GetAllFollowsPair();

  // Returns a list of (followee, direct follower) pair of statement numbers.
  // @returns list of pairs of stmt_nums (may be empty)
  StmtNumPairList GetAllFollowsTPair();
};

#endif !SPA_FOLLOWS_TABLE_H