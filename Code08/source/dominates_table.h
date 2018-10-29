#pragma once

#ifndef SPA_DOMINATES_TABLE_H
#define SPA_DOMINATES_TABLE_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

using std::make_pair;
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = int;
using StmtNumList = vector<int>;
using StmtNumSet = unordered_set<int>;
using StmtNumPairList = vector<pair<int, int>>;
using DominatesSetMap = unordered_map<Vertex, VertexSet>;
using DominatesListMap = unordered_map<Vertex, VertexList>;

// The dominates table class for the PKB component
// Used to store dominates relationships between stmts that are passed into PKB
// from the parser
class DominatesTable {
  DominatesSetMap dominates_set_map_;
  DominatesListMap dominates_list_map_;
  DominatesListMap dominated_by_list_map_;
  StmtNumPairList dominates_pair_list_;

 public:
  void InsertDominates(Vertex dominating_vertex, VertexSet dominated_vertices);

  bool IsDominates(StmtNum dominating_stmt, StmtNum dominated_stmt);

  StmtNumList GetDominating(StmtNum stmt_num);

  StmtNumList GetDominated(StmtNum stmt_num);

  StmtNumPairList GetAllDominatesPairs();

  bool HasDominatesRelationship();
};

#endif !SPA_DOMINATES_TABLE_H
