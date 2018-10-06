#include "graph.h"

Graph::Graph() { size_ = 0; }

bool Graph::AddNode(const string &name) {
  // not already in the node map
  if (!node_map_.count(name)) {
    Node *node;
    node = new Node(name);
    node_map_[name] = node;
    size_++;
    return true;
  }

  return false;
}

void Graph::AddEdge(const string &from, const string &to) {
  if (!node_map_.count(from)) {
    AddNode(from);
  }
  if (!node_map_.count(to)) {
    AddNode(to);
  }
  Node *from_node = node_map_.find(from)->second;
  Node *to_node = node_map_.find(to)->second;
  from_node->adj_.push_back(to_node);
}

void Graph::DFS(const string &name, VisitedMap &visited_map,
                vector<string> &post_traverse) {
  if (size_ == 0) {
    // Only single procedure
    return;
  }
  // Mark the current node as visited
  visited_map.at(name) = true;

  // Recur for all the vertices adjacent to node
  Node *from_node = node_map_.find(name)->second;
  for (auto node : from_node->adj_) {
    if (!visited_map.at(node->name_)) {
      DFS(node->name_, visited_map, post_traverse);
    }
  }

  // append to post traverse vector
  post_traverse.push_back(name);
}

vector<string> Graph::DFS(const string &name) {
  VisitedMap visited_map;
  vector<string> post_traverse;
  for (auto it : node_map_) {
    visited_map.emplace(it.first, false);
  }

  // Call the recursive helper function to populate traversal vector
  DFS(name, visited_map, post_traverse);

  return post_traverse;
}
