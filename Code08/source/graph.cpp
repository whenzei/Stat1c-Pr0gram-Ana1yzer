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

int Graph::GetSize() { return size_; }


void Graph::Toposort(const string &name, VisitedMap &visited_map,
                     queue<string> &topoqueue) {
  if (size_ == 0) {
    // only single procedure
    return;
  }

  // mark the current node as visited
  visited_map.at(name) = true;

  // recur for all the vertices adjacent to node
  Node *from_node = node_map_.find(name)->second;
  for (auto node : from_node->adj_) {
    if (!visited_map.at(node->name_)) {
      Toposort(node->name_, visited_map, topoqueue);
    }
  }

  // push to topoqueue;
  topoqueue.push(name);
}

vector<string> Graph::Toposort() {
  queue<string> topoqueue;
  vector<string> toposorted;
  // mark all the vertices as not visited
  VisitedMap visited_map;
  for (auto it : node_map_) {
    visited_map.emplace(it.first, false);
  }

  // call the recursive helper function to store toposorted nodes
  // starting from all nodes
  for (auto it : node_map_) {
    string node_name = it.first;
    if (!visited_map.at(node_name)) {
      Toposort(node_name, visited_map, topoqueue);
    }
  }

  // add queue to vector in FIFO order
  while (!topoqueue.empty()) {
    toposorted.push_back(topoqueue.front());
    topoqueue.pop();
  }

  return toposorted;
}
