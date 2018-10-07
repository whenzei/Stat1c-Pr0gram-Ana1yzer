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

using VisitedMap = unordered_map<string, bool>;

struct Node {
  unordered_set<Node*> neighbours_;
  string name_;

  // constructor
  Node(string s) : name_(s) {}
};

using NodeMap = map<string, Node*>;

class Graph {
  NodeMap node_map_;
  int size_;

  void Toposort(const string& name, VisitedMap& visited_map,
                queue<string>& topoqueue);

 public:
  Graph();

  // @returns true if node is successfully added to graph,
  // false if node already exists
  bool AddNode(const string& name);

  // Adds edges between the from node to the to node
  // If the node map does not have a from node or a to node,
  // they are created and inserted into the node map before the edge is added
  void AddEdge(const string& from, const string& to);

  // @returns set of all names of neighbours of node referred by given node name
  unordered_set<string> GetNeighbourNames(const string& name);

  // @returns size of graph
  int GetSize();

  // DFS with toposort
  // @returns vector of procedures in reverse topological order
  vector<string> Toposort();
};

#endif  // !SPA_GRAPH_H
