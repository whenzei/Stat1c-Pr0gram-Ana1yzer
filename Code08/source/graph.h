#pragma once

#ifndef SPA_GRAPH_H
#define SPA_GRAPH_H

#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::map;
using std::queue;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using AdjList = unordered_map<int, vector<int>>;

class Graph {
  AdjList adj_list_;
  unordered_map<string, int> str_to_int_;
  unordered_map<int, string> int_to_str_;
  int size_;

  void Toposort(const int& v, bool visited[], queue<int>& topoqueue);

  bool HasCycle(const int& v, vector<int>& adj, bool visited[]);

 public:
  Graph();

  // Adds a node into the graph,
  // @returns true if node is added, false if node already exists
  bool AddNode(const string& name);

  // Adds edges between the from node to the to node
  void AddEdge(const int& from, const int& to);

  // temporary function to convert strings to int before str-to-int is merged in
  // if node is not in the graph yet, the node will be created and then inserted
  void AddEdge(const string& from, const string& to);

  // @returns set of all names of neighbours of node referred by given node name
  unordered_set<string> GetNeighbourNames(const string& name);

  // @returns size of graph
  int GetSize();

  // DFS with toposort
  // @returns vector of procedures in reverse topological order
  vector<string> Toposort();

  // DFS
  // @returns vector of integers in DFS
  vector<int> DFS();

  // Check for cycles in the graph
  // @returns true if cycle exists, false otherwise
  bool HasCycle();
};

#endif  // !SPA_GRAPH_H
