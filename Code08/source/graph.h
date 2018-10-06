#pragma once

#ifndef SPA_GRAPH_H
#define SPA_GRAPH_H
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using std::map;
using std::string;
using std::unordered_map;
using std::vector;

using VisitedMap = unordered_map<string, bool>;

struct Node {
  vector<Node*> adj_;  // cost of edge, destination vertex
  string name_;

  // constructor
  Node(string s) : name_(s) {}
};

using NodeMap = map<string, Node*>;

class Graph {
  NodeMap node_map_;
  int size_;
  void DFS(const string& name, VisitedMap& visited_map,
           vector<string>& post_traverse);

 public:
  Graph();

  // @returns true if node is successfully added to graph,
  // false if node already exists
  bool AddNode(const string& name);
  void AddEdge(const string& from, const string& to);

  int GetSize();

  // DFS post traversal of nodes reachable from node with given name
  // @returns vector of procedures to process in order
  vector<string> DFS(const string& name);
};

#endif  // !SPA_GRAPH_H
