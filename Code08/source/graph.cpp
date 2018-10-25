#include "graph.h"

Graph::Graph() {
  size_ = 0;
  adj_list_ = AdjList();
  adj_set_ = AdjSet();
}

void Graph::AddEdge(const Vertex &from, const Vertex &to) {
  if (!adj_set_.count(from)) {
    size_++;
    adj_set_.emplace(from, VertexSet());
    adj_list_.emplace(from, VertexList());
  }

  if (!adj_set_.count(to)) {
    size_++;
    adj_set_.emplace(to, VertexSet());
    adj_list_.emplace(to, VertexList());
  }

  if (!adj_set_.at(from).count(to)) {
    adj_list_[from].push_back(to);
    adj_set_[from].emplace(to);
  }
}

bool Graph::IsEmpty() { return size_ == 0; }

void Graph::Toposort(const Vertex &v, bool visited[], queue<int> &topoqueue) {
  if (size_ == 0) {
    // only single procedure
    return;
  }

  // mark the current node as visited
  visited[v] = true;

  // recur for all the vertices adjacent to node
  for (auto node : adj_set_[v]) {
    if (!visited[node]) {
      Toposort(node, visited, topoqueue);
    }
  }

  // push to topoqueue;
  topoqueue.push(v);
}

VertexList Graph::Toposort() {
  queue<int> topoqueue;
  VertexList toposorted;
  // mark all the vertices as not visited
  bool *visited = new bool[size_];
  for (int i = 0; i < size_; i++) {
    visited[i] = false;
  }

  // call the recursive helper function to store toposorted nodes
  // starting from all nodes
  for (int i = 0; i < size_; i++) {
    if (visited[i] == false) {
      Toposort(i, visited, topoqueue);
    }
  }

  // add queue to vector in FIFO order
  while (!topoqueue.empty()) {
    toposorted.push_back(topoqueue.front());
    topoqueue.pop();
  }

  delete[] visited;
  return toposorted;
}

vector<int> Graph::DFS() { return vector<int>(); }

bool Graph::HasCycle() {
  // mark all the vertices as not visited
  bool *visited = new bool[size_];
  for (int i = 0; i < size_; i++) {
    visited[i] = false;
  }

  // call the recursive helper function
  for (int i = 0; i < size_; i++) {
    if (HasCycle(i, &adj_set_[i], visited)) {
      return true;
    }
  }

  delete[] visited;
  return false;
}

bool Graph::HasCycle(const Vertex &v, VertexSet *adj, bool visited[]) {
  if (!visited[v]) {
    visited[v] = true;

    for (auto &node : *adj) {
      if (visited[node]) {
        return true;
      }
      if (HasCycle(node, &adj_set_[node], visited)) {
        return true;
      }
    }
    visited[v] = false;
  }

  return false;
}

VertexSet Graph::GetNeighboursSet(const Vertex &v) {
  if (adj_set_.count(v)) {
    return adj_set_.at(v);
  }
  return VertexSet();
}

VertexList Graph::GetNeighboursList(const Vertex &v) { return adj_list_.at(v); }

int Graph::GetSize() { return size_; }

AdjList Graph::GetAdjList() { return adj_list_; }

AdjSet Graph::GetAdjSet() { return adj_set_; }
