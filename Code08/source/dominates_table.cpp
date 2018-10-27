#pragma once

#include "dominates_table.h"

void DominatesTable::InsertDominates(Vertex dominating_vertex,
                                     VertexSet dominated_vertices) {
  dominates_set_map_[dominating_vertex] = dominated_vertices;
  for (Vertex dominated_vertex : dominated_vertices) {
    dominates_list_map_[dominating_vertex].push_back(dominated_vertex);
    dominated_by_list_map_[dominated_vertex].push_back(dominating_vertex);
    dominates_pair_list_.push_back(
        make_pair(dominating_vertex, dominated_vertex));
  }
}

bool DominatesTable::IsDominates(StmtNum dominating_stmt,
                                 StmtNum dominated_stmt) {
  DominatesSetMap::iterator iter = dominates_set_map_.find(dominating_stmt);
  if (iter != dominates_set_map_.end()) {
    return (*iter).second.find(dominated_stmt) != (*iter).second.end();
  } else {
    return false;
  }
}

StmtNumList DominatesTable::GetDominating(StmtNum stmt_num) {
  if (dominated_by_list_map_.count(stmt_num)) {
    return dominated_by_list_map_[stmt_num];
  } else {
    return StmtNumList();
  }
}

StmtNumList DominatesTable::GetDominated(StmtNum stmt_num) {
  if (dominates_list_map_.count(stmt_num)) {
    return dominates_list_map_[stmt_num];
  } else {
    return StmtNumList();
  }
}

StmtNumPairList DominatesTable::GetAllDominatesPairs() {
  return dominates_pair_list_;
}

bool DominatesTable::HasDominatesRelationship() { return !dominates_set_map_.empty(); }
