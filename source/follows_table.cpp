#pragma once

#include "follows_table.h"

bool FollowsTable::HasFollowedBy() {
  return (!followed_by_map_.empty());
}

bool FollowsTable::InsertFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  // if relationship is already in the follows map, return false
  if (IsFollows(stmt_num1, stmt_num2)) { return false; }
  follows_map_[stmt_num1].push_back(stmt_num2);
  followed_by_map_[stmt_num2].push_back(stmt_num1);
  return true;
}

bool FollowsTable::IsFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  return (find(follows_map_[stmt_num1].begin(), follows_map_[stmt_num1].end(), stmt_num2) != follows_map_[stmt_num1].end());
}

bool FollowsTable::IsDirectFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  return (follows_map_[stmt_num1].front() == stmt_num2);
}

StmtList FollowsTable::GetFollows(StmtNum stmt_num) {
  return follows_map_[stmt_num];
}

StmtNum FollowsTable::GetDirectFollows(StmtNum stmt_num) {
  return follows_map_[stmt_num].front();
}

StmtList FollowsTable::GetFollowedBy(StmtNum stmt_num) {
  return followed_by_map_[stmt_num];
}

StmtNum FollowsTable::GetDirectFollowedBy(StmtNum stmt_num) {
  return followed_by_map_[stmt_num].front();
}

bool FollowsTable::HasFollowsRelationship() {
  return (!follows_map_.empty());
}