#pragma once

#include "dominates_table.h"

void DominatesTable::InsertDominates(Vertex dominating_vertex,
                                     VertexSet dominated_vertices) {
  dominates_map_[dominating_vertex] = dominated_vertices;
  for (Vertex dominated_vertex : dominated_vertices) {
    dominated_by_map_[dominated_vertex].insert(dominating_vertex);
    dominates_pair_set_.insert(make_pair(dominating_vertex, dominated_vertex));
  }
}

bool DominatesTable::IsDominates(StmtNum dominating_stmt,
                                 StmtNum dominated_stmt) {
  DominatesMap::iterator iter = dominates_map_.find(dominating_stmt);
  if (iter != dominates_map_.end()) {
    return (*iter).second.find(dominated_stmt) != (*iter).second.end();
  } else {
    return false;
  }
}

StmtNumSet DominatesTable::GetDominating(StmtNum stmt_num) {
  if (dominated_by_map_.count(stmt_num)) {
    return dominated_by_map_[stmt_num];
  } else {
    return StmtNumSet();
  }
}

StmtNumSet DominatesTable::GetDominated(StmtNum stmt_num) {
  if (dominates_map_.count(stmt_num)) {
    return dominates_map_[stmt_num];
  } else {
    return StmtNumSet();
  }
}

StmtNumPairSet DominatesTable::GetAllDominatesPairs() {
  return dominates_pair_set_;
}

bool DominatesTable::HasDominatesRelationship() { return !dominates_map_.empty(); }
