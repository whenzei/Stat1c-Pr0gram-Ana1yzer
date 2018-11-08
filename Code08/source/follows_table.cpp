#pragma once

#include "follows_table.h"

void FollowsTable::InsertFollows(StmtNum stmt_1, StmtNum stmt_2) {
  if (followed_by_set_.insert(stmt_1).second) {
    // stmt_1 is not followed by any other stmt, direct direct follows
    // relationship
    direct_followed_by_map_[stmt_1].insert(stmt_2);
    direct_follows_map_[stmt_2].insert(stmt_1);
  }
  followed_by_map_[stmt_1].insert(stmt_2);
  follows_map_[stmt_2].insert(stmt_1);
  follows_set_.insert(stmt_2);
}

bool FollowsTable::IsFollowsT(StmtNum stmt_num1, StmtNum stmt_num2) {
  if (followed_by_map_.find(stmt_num1) != followed_by_map_.end()) {
    return (followed_by_map_[stmt_num1].find(stmt_num2) !=
            followed_by_map_[stmt_num1].end());
  } else {
    return false;
  }
}

bool FollowsTable::IsFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  if (direct_followed_by_map_.find(stmt_num1) !=
      direct_followed_by_map_.end()) {
    return (direct_followed_by_map_[stmt_num1].find(stmt_num2) !=
            direct_followed_by_map_[stmt_num1].end());
  } else {
    return false;
  }
}

StmtNumSet FollowsTable::GetFollowsT(StmtNum stmt_num) {
  if (followed_by_map_.find(stmt_num) != followed_by_map_.end()) {
    return followed_by_map_[stmt_num];
  }
  return StmtNumSet();
}

StmtNumSet FollowsTable::GetFollows(StmtNum stmt_num) {
  if (direct_followed_by_map_.find(stmt_num) != direct_followed_by_map_.end()) {
    return direct_followed_by_map_[stmt_num];
  }
  return StmtNumSet();
}

StmtNumSet FollowsTable::GetAllFollows() { return follows_set_; }

StmtNumSet FollowsTable::GetFollowedByT(StmtNum stmt_num) {
  if (follows_map_.find(stmt_num) != follows_map_.end()) {
    return follows_map_[stmt_num];
  }
  return StmtNumSet();
}

StmtNumSet FollowsTable::GetFollowedBy(StmtNum stmt_num) {
  if (direct_follows_map_.find(stmt_num) != direct_follows_map_.end()) {
    return direct_follows_map_[stmt_num];
  }
  return StmtNumSet();
}

StmtNumSet FollowsTable::GetAllFollowedBy() { return followed_by_set_; }

bool FollowsTable::HasFollowsRelationship() { return (!follows_map_.empty()); }

StmtNumPairSet FollowsTable::GetAllFollowsTPair() {
  StmtNumPairSet follows_pair_t_set;
  for (auto entry : followed_by_map_) {
    for (auto follower_stmt_num : entry.second) {
      follows_pair_t_set.insert(make_pair(entry.first, follower_stmt_num));
    }
  }
  return follows_pair_t_set;
}

StmtNumPairSet FollowsTable::GetAllFollowsPair() {
  StmtNumPairSet follows_pair_set;
  for (auto entry : direct_followed_by_map_) {
    for (auto stmt : entry.second) {
      follows_pair_set.insert(make_pair(entry.first, stmt));
    }
  }
  return follows_pair_set;
}