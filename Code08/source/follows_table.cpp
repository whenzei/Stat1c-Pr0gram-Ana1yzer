#pragma once

#include "follows_table.h"

void FollowsTable::InsertFollows(StmtNum stmt_num1, StmtNum stmt_num2) {
  if (!IsFollowsT(stmt_num1, stmt_num2)) {
    // follows map has to store stmt numbers in descending order
    // follows by map has to store stmt numbers in ascending order
    follows_map_[stmt_num2].insert(follows_map_[stmt_num2].begin(), stmt_num1);
    followed_by_map_[stmt_num1].push_back(stmt_num2);
    if (follows_set_.insert(stmt_num2).second) {
      follows_list_.push_back(stmt_num2);
	}
    if (followed_by_set_.insert(stmt_num1).second) {
      followed_by_list_.push_back(stmt_num1);
    }
  }
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

StmtNumList FollowsTable::GetFollowsT(StmtNum stmt_num) {
  StmtNumList follows_list;
  if (followed_by_map_.find(stmt_num) != followed_by_map_.end()) {
    follows_list = followed_by_map_[stmt_num];
  }
  return follows_list;
}

StmtNumList FollowsTable::GetFollows(StmtNum stmt_num) {
  StmtNumList follows;
  if (followed_by_map_.find(stmt_num) != followed_by_map_.end()) {
    follows.push_back(followed_by_map_[stmt_num].front());
  }
  return follows;
}

StmtNumList FollowsTable::GetAllFollows() {
  return follows_list_;
}

StmtNumList FollowsTable::GetFollowedByT(StmtNum stmt_num) {
  StmtNumList followed_by_list;
  if (follows_map_.find(stmt_num) != follows_map_.end()) {
    followed_by_list = follows_map_[stmt_num];
  }
  return followed_by_list;
}

StmtNumList FollowsTable::GetFollowedBy(StmtNum stmt_num) {
  StmtNumList followed_by;
  if (follows_map_.find(stmt_num) != follows_map_.end()) {
    followed_by.push_back(follows_map_[stmt_num].front());
  }
  return followed_by;
}

StmtNumList FollowsTable::GetAllFollowedBy() {
  return followed_by_list_;
}

bool FollowsTable::HasFollowsRelationship() {
  return (!follows_map_.empty());
}

StmtNumPairList FollowsTable::GetAllFollowsTPair() {
  StmtNumPairList follows_pair_t_list;
  for (auto entry : followed_by_map_) {
    for (auto follower_stmt_num : entry.second) {
      follows_pair_t_list.push_back(make_pair(entry.first, follower_stmt_num));
    }
  }
  return follows_pair_t_list;
}

StmtNumPairList FollowsTable::GetAllFollowsPair() {
  StmtNumPairList follows_pair_list;
  for (auto entry : followed_by_map_) {
    follows_pair_list.push_back(make_pair(entry.first, entry.second.front()));
  }
  return follows_pair_list;
}