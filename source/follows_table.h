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
  StmtNumList GetFollowsT(StmtNum stmt_num);

  // Finds and returns list of stmt num containing the stmt that directly follows the
  // given statement. Only the direct follower is returned.
  // @params
  // @returns a stmt_number if a direct follower exists
  // @returns -1 if otherwise
  StmtNumList GetFollows(StmtNum stmt_num);

  // Returns a list of all statements that follow some statement.
  // @returns a list of stmt_nums
  // can be empty
  StmtNumList GetAllFollows();

  // Finds list of statements that are followed by the given statement.
  // Transitive relationships are included.
  // @params
  // @returns a list of statement numbers of statements that are followed by s
  // list can be empty
  StmtNumList GetFollowedByT(StmtNum stmt_num);

  // Finds and returns a stmt list containing the stmt that is directly followed
  // by the given statement. Only the direct followee is returned.
  // @params
  // @returns a stmt_number if a direct followee exists
  // @returns -1 if otherwise
  StmtNumList GetFollowedBy(StmtNum stmt_num);

  // Returns a list of statement numbers of statements that are followed by some statement.
  // @returns a list of stmt_nums
  // can be empty
  StmtNumList GetAllFollowedBy();

  // Checks and returns if follows map contains any follows relationships.
  // @returns true if there are any follows relationships
  // @returns false if follows map is empty
  bool HasFollowsRelationship();

  // Returns a list of (followee, follower) pair of statement numbers.
  // @returns list of pairs of stmt_nums (may be empty)
  StmtNumPairList GetAllFollowsTPair();

  // Returns a list of (followee, direct follower) pair of statement numbers.
  // @returns list of pairs of stmt_nums (may be empty)
  StmtNumPairList GetAllFollowsPair();
};

#endif !SPA_FOLLOWS_TABLE_H