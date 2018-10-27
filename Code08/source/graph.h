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
using VertexList = vector<Vertex>;
using VertexSet = unordered_set<Vertex>;
using AdjSet = unordered_map<Vertex, VertexSet>;
using AdjList = unordered_map<Vertex, VertexList>;
using VisitedMap = unordered_map<Vertex, bool>;

class Graph {
  // have both set and vector for O(1) retrieval of different actions
  Vertex root_;
  AdjList adj_list_;
  AdjSet adj_set_;
  int size_;

  void Toposort(const Vertex& v, VisitedMap* visited, queue<Vertex>* topoqueue);

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
  VertexList DFS(const Vertex v);

  // Get all unreachable vertices when the given vertex v is removed
  // @returns set of all non-visited vertices
  VertexSet GetUnreachableVertices(Vertex v);

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

  // @returns set of all vertices in the graph
  VertexSet GetAllVertices();
};

#endif  // !SPA_GRAPH_H
