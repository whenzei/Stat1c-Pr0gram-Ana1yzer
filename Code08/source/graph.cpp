#include "graph.h"

bool Graph::AddNode(const string &name) {
  if (node_map_.count(name)) {
    Node *node;
    node = new Node(name);
    node_map_[name] = node;
    return true;
  }

  return false;
}

void Graph::AddEdge(const string &from, const string &to) {
  Node *from_node = node_map_.find(from)->second;
  Node *to_node = node_map_.find(to)->second;
  from_node->adj_.push_back(to_node);
}
