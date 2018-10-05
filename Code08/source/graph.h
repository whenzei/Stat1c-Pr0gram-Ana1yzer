#pragma once

#ifndef SPA_GRAPH_H
#define SPA_GRAPH_H
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

struct Node {
  vector<Node*> adj_;  // cost of edge, destination vertex
  string name_;

  // constructor
  Node(string s) : name_(s) {}
};

using NodeMap = map<string, Node*>;

class Graph {
  NodeMap node_map_;

 public:
  // @returns true if node is successfully added to graph,
  // false if node already exists
  bool AddNode(const string& name);
  void AddEdge(const string& from, const string& to);
};

#endif  // !SPA_GRAPH_H
