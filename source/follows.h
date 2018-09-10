#pragma once

#ifndef SPA_FOLLOWS_H
#define SPA_FOLLOWS_H

#include <list>
#include <unordered_map>
#include <string>

using std::list;
using std::unordered_map;
using std::string;

using StmtNum = string;
using StmtList = list<string>;
using FollowsMap = unordered_map<string, list<string>>;
using FollowedByMap = unordered_map<string, list<string>>;

class Follows {
  FollowsMap follows_map_;
  FollowedByMap followed_by_map_;

private:
  // Utility function to check if followed by map contains any
  // followed by relationships.
  // @returns true if there are any followed by relationships
  // @returns false if followed by map is empty
  bool HasFollowedBy();

public:
  // Inserts a follows relationship to the follows map and followed by map.
  // @params
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
  bool IsDirectFollows(StmtNum stmt_num1, StmtNum stmt_num2);

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
  StmtNum GetDirectFollows(StmtNum stmt_num);

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
  StmtNum GetDirectFollowedBy(StmtNum stmt_num);

  // Checks and returns if follows map contains any follows relationships.
  // @returns true if there are any follows relationships
  // @returns false if follows map is empty
  bool HasFollowsRelationship();
};

#endif !SPA_FOLLOWS_H