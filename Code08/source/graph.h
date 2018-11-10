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
using VisitedCountMap = unordered_map<Vertex, int>;

class Graph {
  // have both set and vector for O(1) retrieval of different actions
  Vertex root_;
  AdjList adj_list_;
  AdjSet adj_set_;
  VertexSet all_vertices_;
  VertexSet parent_vertices_;
  int size_;

  // Overloaded method for the Toposort() function to populate the topoqueue
  void Toposort(const Vertex& v, VisitedMap* visited, queue<Vertex>* topoqueue);

  // Helper method to determine if there are cycles
  bool HasCycle(const Vertex& v, VisitedMap* visited);

  // Generic DFS method, keeps a list of the path traversed
  void DFS(const Vertex& v, VisitedMap*, VertexSet* path);

  // Helper method for CanReach
  // @returns true if v can reach target, false otherwise
  bool DFS(Vertex v, Vertex target, VisitedMap* visited);

 public:
  Graph();

  bool operator==(const Graph& other_graph);

  // sets the root of the graph to the given value
  // If the given value is not in the graph, no root change will occur
  void SetRoot(const Vertex& new_root);

  // Adds edges between the from node to the to node
  void AddEdge(const Vertex& from, const Vertex& to);

  // Remove edges between the from node and the to node
  void RemoveEdge(const Vertex& from, const Vertex& to);

  // Adds node into the graph
  void AddNode(const Vertex& vertex);

  // Remove node from the graph
  void RemoveNode(const Vertex& vertex);

  // @returns set of neighbours of given vertex v
  VertexSet GetNeighboursSet(const Vertex& v);

  // @returns vertex of neighbours of given vertex v
  VertexList GetNeighboursList(const Vertex& v);

  // @returns a list of all nodes without neighbours
  VertexList GetTerminalNodes();

  // A vertex is a parent if it points to another vertex
  // @returns parent all vertices
  VertexSet GetParentVertices();

  // @returns true if graph is empty, false otherwise
  bool IsEmpty();

  // DFS with toposort
  // @returns vector of vertices in reverse topological order
  VertexList Toposort();

  // Performs a DFS of all vertices reachable from given vertex v
  // @returns vector of vertices in DFS
  VertexSet DFS(const Vertex v);

  // Performs a DFS of neighbouring vertices
  // @returns a set of vertices in DFS
  VertexSet DFSNeighbours(const Vertex v);

  // Get all unreachable vertices when the given vertex v is removed
  // @returns set of all non-visited vertices
  VertexSet GetUnreachableVertices(Vertex v);

  // Check for cycles in the graph
  // @returns true if cycle exists, false otherwise
  bool HasCycle();

  // @returns true if vertex 'to' can be reached from vertex 'from'
  bool CanReach(Vertex from, Vertex to);

  /* GETTERS */
  // @returns size of graph
  int GetSize();

  // @returns the AdjList of the graph
  AdjList GetAdjList();

  // @returns the AdjSet of the graph
  AdjSet GetAdjSet();

  // @returns set of all vertices in the graph
  VertexSet GetAllVertices();

  // @returns the root of the graph
  Vertex GetRoot();
};

#endif  // !SPA_GRAPH_H
