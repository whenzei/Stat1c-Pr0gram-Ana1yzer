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
  follows_map_[stmt_num2].push_front(stmt_num1);
  followed_by_map_[stmt_num1].push_back(stmt_num2);
  return true;
}

bool FollowsTable::IsFollowsT(StmtNum stmt_num1, StmtNum stmt_num2) {
  if (followed_by_map_.find(stmt_num1) != followed_by_map_.end()) {
    return (find(followed_by_map_[stmt_num1].begin(), followed_by_map_[stmt_num1].end(), stmt_num2) != followed_by_map_[stmt_num1].end());
  } else {
    return false;
  }
}

bool FollowsTable::IsFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  if (followed_by_map_.find(stmt_num1) != followed_by_map_.end()) {
    return (followed_by_map_[stmt_num1].front() == stmt_num2);
  } else {
    return false;
  }
}

StmtList FollowsTable::GetFollowsT(StmtNum stmt_num) {
  StmtList follows_list;
  if (followed_by_map_.find(stmt_num) != followed_by_map_.end()) {
    follows_list = followed_by_map_[stmt_num];
  }
  return follows_list;
}

StmtList FollowsTable::GetFollows(StmtNum stmt_num) {
  StmtList follows;
  if (followed_by_map_.find(stmt_num) != followed_by_map_.end()) {
    follows.push_back(followed_by_map_[stmt_num].front());
  }
  return follows;
}

StmtList FollowsTable::GetAllFollows() {
  StmtList follows;
  for (auto& entry : follows_map_) {
    follows.push_back(entry.first);
  }
  return follows;
}

StmtList FollowsTable::GetFollowedByT(StmtNum stmt_num) {
  StmtList followed_by_list;
  if (follows_map_.find(stmt_num) != follows_map_.end()) {
    followed_by_list = follows_map_[stmt_num];
  }
  return followed_by_list;
}

StmtList FollowsTable::GetFollowedBy(StmtNum stmt_num) {
  StmtList followed_by;
  if (follows_map_.find(stmt_num) != follows_map_.end()) {
    followed_by.push_back(follows_map_[stmt_num].front());
  }
  return followed_by;
}

StmtList FollowsTable::GetAllFollowedBy() {
  StmtList followed_by;
  for (auto& entry : followed_by_map_) {
    followed_by.push_back(entry.first);
  }
  return followed_by;
}

bool FollowsTable::HasFollowsRelationship() {
  return (!follows_map_.empty());
}

StmtNumPairList FollowsTable::GetAllFollowsTPair() {
  StmtNumPairList follows_pair_t_list;
  if (GetFollowsMapSize() == 0) { return follows_pair_t_list; }
  for (auto entry : followed_by_map_) {
    for (auto follower_stmt_num : entry.second) {
      follows_pair_t_list.push_back(make_pair(entry.first, follower_stmt_num));
    }
  }
  return follows_pair_t_list;
}

StmtNumPairList FollowsTable::GetAllFollowsPair() {
  StmtNumPairList follows_pair_list;
  if (GetFollowsMapSize() == 0) { return follows_pair_list; }
  for (auto entry : followed_by_map_) {
    follows_pair_list.push_back(make_pair(entry.first, entry.second.front()));
  }
  return follows_pair_list;
}