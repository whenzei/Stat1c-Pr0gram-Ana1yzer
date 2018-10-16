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
  from_node->neighbours_.insert(to_node);
}

unordered_set<string> Graph::GetNeighbourNames(const string &name) {
  unordered_set<string> result;
  if (node_map_.count(name)) {
    unordered_set<Node *> node_neighbours = node_map_.at(name)->neighbours_;
    for (const auto node : node_neighbours) {
      result.insert(node->name_);
    }
  }
  return result;
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
  for (auto node : from_node->neighbours_) {
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

bool Graph::HasCycle(const string &name, NodeSet &adj,
                     VisitedMap &visited_map) {
  if (!visited_map.at(name)) {
    visited_map.at(name) = true;

    for (auto node : adj) {
      string node_name = node->name_;
      if (visited_map.at(node_name)) {
        return true;
      }
      NodeSet neighbours = node_map_.at(node_name)->neighbours_;

      if (HasCycle(node_name, neighbours, visited_map)) {
        return true;
      }
    }
    visited_map.at(name) = false;
  }
  return false;
}

bool Graph::HasCycle() {
  // mark all the vertices as not visited
  VisitedMap visited_map;

  for (auto it : node_map_) {
    visited_map.emplace(it.first, false);
  }

  // call the recursive helper function
  for (auto it : node_map_) {
    string node_name = it.second->name_;
    NodeSet adj = it.second->neighbours_;
    if (HasCycle(node_name, adj, visited_map)) {
      return true;
    }
  }

  return false;
}
