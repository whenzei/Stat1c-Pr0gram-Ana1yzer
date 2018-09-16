#pragma once

#include "follows_table.h"

bool FollowsTable::HasFollowedBy() {
  return (!followed_by_map_.empty());
}

int FollowsTable::GetFollowsMapSize() {
  return follows_map_.size();
}

bool FollowsTable::InsertFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  // if relationship is already in the follows map, return false
  if (IsFollowsT(stmt_num1, stmt_num2)) { return false; }
  follows_map_[stmt_num1].push_back(stmt_num2);
  followed_by_map_[stmt_num2].push_back(stmt_num1);
  return true;
}

bool FollowsTable::IsFollowsT(StmtNum stmt_num1, StmtNum stmt_num2) {
  if (follows_map_.find(stmt_num1) != follows_map_.end()) {
    return (find(follows_map_[stmt_num1].begin(), follows_map_[stmt_num1].end(), stmt_num2) != follows_map_[stmt_num1].end());
  } else {
    return false;
  }
}

bool FollowsTable::IsFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  if (follows_map_.find(stmt_num1) != follows_map_.end()) {
    return (follows_map_[stmt_num1].front() == stmt_num2);
  } else {
    return false;
  }
}

StmtList FollowsTable::GetFollowsT(StmtNum stmt_num) {
  StmtList follows_list;
  if (follows_map_.find(stmt_num) != follows_map_.end()) {
    follows_list = follows_map_[stmt_num];
  }
  return follows_list;
}

StmtNum FollowsTable::GetFollows(StmtNum stmt_num) {
  StmtNum follows;
  if (follows_map_.find(stmt_num) != follows_map_.end()) {
    follows = follows_map_[stmt_num].front();
  }
  return follows;
}

StmtList FollowsTable::GetFollowedByT(StmtNum stmt_num) {
  StmtList followed_by_list;
  if (followed_by_map_.find(stmt_num) != followed_by_map_.end()) {
    followed_by_list = followed_by_map_[stmt_num];
  }
  return followed_by_list;
}

StmtNum FollowsTable::GetFollowedBy(StmtNum stmt_num) {
  StmtNum followed_by;
  if (followed_by_map_.find(stmt_num) != followed_by_map_.end()) {
    followed_by = followed_by_map_[stmt_num].front();
  }
  return followed_by;
}

bool FollowsTable::HasFollowsRelationship() {
  return (!follows_map_.empty());
}

StmtNumPairList FollowsTable::GetAllFollowsTPair() {
  StmtNumPairList follows_pair_t_list;
  if (GetFollowsMapSize() == 0) { return follows_pair_t_list; }
  for (auto entry : follows_map_) {
    for (auto follower_stmt_num : entry.second) {
      follows_pair_t_list.push_back(make_pair(entry.first, follower_stmt_num));
    }
  }
  return follows_pair_t_list;
}

StmtNumPairList FollowsTable::GetAllFollowsPair() {
  StmtNumPairList follows_pair_list;
  if (GetFollowsMapSize() == 0) { return follows_pair_list; }
  for (auto entry : follows_map_) {
    follows_pair_list.push_back(make_pair(entry.first, entry.second.front()));
  }
  return follows_pair_list;
}