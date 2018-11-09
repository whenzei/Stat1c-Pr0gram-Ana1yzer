#pragma once

#ifndef SPA_DOMINATES_TABLE_H
#define SPA_DOMINATES_TABLE_H

#include <unordered_map>
#include <unordered_set>

#include "graph.h"
#include "result_hasher.h"

using std::make_pair;
using std::pair;
using std::unordered_map;
using std::unordered_set;

using StmtNum = int;
using StmtNumSet = unordered_set<int>;
using StmtNumPairSet = unordered_set<pair<int, int>, ResultHasher>;
using DominatesMap = unordered_map<Vertex, VertexSet>;

// The dominates table class for the PKB component
// Used to store dominates relationships between stmts that are passed into PKB
// from the parser
class DominatesTable {
  DominatesMap dominates_map_;
  DominatesMap dominated_by_map_;
  StmtNumPairSet dominates_pair_set_;

 public:
  void InsertDominates(Vertex dominating_vertex, VertexSet dominated_vertices);

  bool IsDominates(StmtNum dominating_stmt, StmtNum dominated_stmt);

  StmtNumSet GetDominating(StmtNum stmt_num);

  StmtNumSet GetDominated(StmtNum stmt_num);

  StmtNumPairSet GetAllDominatesPairs();

  bool HasDominatesRelationship();
};

#endif !SPA_DOMINATES_TABLE_H
