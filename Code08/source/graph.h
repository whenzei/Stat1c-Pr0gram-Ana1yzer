#pragma once

#ifndef SPA_GRAPH_H
#define SPA_GRAPH_H

#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::queue;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using Vertex = int;
using VertexList = vector<int>;
using VertexSet = unordered_set<int>;
using AdjSet = unordered_map<int, VertexSet>;
using AdjList = unordered_map<int, VertexList>;
using VisitedMap = unordered_map<int, bool>;

class Graph {
  // have both set and vector for O(1) retrieval of different actions
  AdjList adj_list_;
  AdjSet adj_set_;
  int size_;

  void Toposort(const Vertex& v, VisitedMap* visited, queue<int>* topoqueue);

  bool HasCycle(const Vertex& v, VisitedMap* visited, VertexSet* neighbours);

  void DFS(const Vertex& v, VisitedMap*, VertexList* path);

 public:
  Graph();

  // Adds edges between the from node to the to node
  void AddEdge(const Vertex& from, const Vertex& to);

  // @returns set of neighbours of given vertex v
  VertexSet GetNeighboursSet(const Vertex& v);

  // @returns vertex of neighbours of given vertex v
  VertexList GetNeighboursList(const Vertex& v);

  // @returns true if graph is empty, false otherwise
  bool IsEmpty();

  // DFS with toposort
  // @returns vector of vertices in reverse topological order
  VertexList Toposort();

  // Performs a DFS of all vertices reachable from given vertex v
  // @returns vector of vertices in DFS
  VertexList DFS(const Vertex& v);

  // Check for cycles in the graph
  // @returns true if cycle exists, false otherwise
  bool HasCycle();

  /* GETTERS */
  // @returns size of graph
  int GetSize();

  // @returns the AdjList of the graph
  AdjList GetAdjList();

  // @returns the AdjSet of the graph
  AdjSet GetAdjSet();
};

#endif  // !SPA_GRAPH_H
