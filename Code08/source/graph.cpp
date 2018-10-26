#include "graph.h"
#include <iostream>

Graph::Graph() {
  size_ = 0;
  adj_list_ = AdjList();
  adj_set_ = AdjSet();
}

void Graph::AddEdge(const Vertex &from, const Vertex &to) {
  if (size_ == 0) {
    // first vertex in graph is the root
    root_ = from;
  }
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

  if (!adj_set_[from].count(to)) {
    adj_list_[from].push_back(to);
    adj_set_[from].emplace(to);
  }
}

bool Graph::IsEmpty() { return size_ == 0; }

void Graph::Toposort(const Vertex &v, VisitedMap *visited,
                     queue<int> *topoqueue) {
  if (size_ == 0) {
    // only single procedure
    return;
  }

  // mark the current node as visited
  (*visited)[v] = true;

  // recur for all the vertices adjacent to node
  for (auto node : adj_set_[v]) {
    if (!(*visited)[node]) {
      Toposort(node, visited, topoqueue);
    }
  }

  // push to topoqueue;
  topoqueue->push(v);
}

VertexList Graph::Toposort() {
  queue<int> topoqueue;
  VertexList toposorted;
  // mark all the vertices as not visited
  VisitedMap visited;
  for (auto &kv : adj_set_) {
    visited[kv.first] = false;
  }

  // call the recursive helper function to store toposorted nodes
  // starting from all nodes
  for (auto &kv : adj_set_) {
    int node = kv.first;
    if (visited[node] == false) {
      Toposort(node, &visited, &topoqueue);
    }
  }

  // add queue to vector in FIFO order
  while (!topoqueue.empty()) {
    toposorted.push_back(topoqueue.front());
    topoqueue.pop();
  }

  return toposorted;
}

bool Graph::HasCycle() {
  // mark all the vertices as not visited
  VisitedMap visited;
  for (auto &kv : adj_set_) {
    visited[kv.first] = false;
  }

  // call the recursive helper function
  for (int i = 0; i < size_; i++) {
    if (HasCycle(i, &visited, &adj_set_[i])) {
      return true;
    }
  }

  return false;
}

bool Graph::HasCycle(const Vertex &v, VisitedMap *visited, VertexSet *adj) {
  if (!(*visited)[v]) {
    (*visited)[v] = true;

    for (auto &node : *adj) {
      if ((*visited)[node]) {
        return true;
      }
      if (HasCycle(node, visited, &adj_set_[node])) {
        return true;
      }
    }
    (*visited)[v] = false;
  }

  return false;
}

VertexList Graph::DFS(const Vertex from) {
  VisitedMap visited;
  for (auto &kv : adj_set_) {
    visited[kv.first] = false;
  }
  VertexList path;

  DFS(from, &visited, &path);
  return path;
}

VertexSet Graph::GetUnreachableVertices(Vertex v) {
  VisitedMap visited;
  for (auto &kv : adj_set_) {
    visited[kv.first] = false;
  }

  visited[v] = true;

  VertexList path;  // don't need this, just here for reusability
  std::cout << "At vertex " << v << std::endl;
  DFS(root_, &visited, &path);
  std::cout << std::endl;

  VertexSet result;

  // set visited[v] back to false
  visited[v] = false;

  for (auto &kv : visited) {
    bool is_visited = kv.second;
    if (!is_visited) {
      // add to result if node is not visited
      result.emplace(kv.first);
    }
  }

  return result;
}

void Graph::DFS(const Vertex &v, VisitedMap *visited, VertexList *path) {
  if ((*visited)[v]) {
    return;
  }

  (*visited)[v] = true;

  // add saving of path here if want pre-order traversal
  std::cout << v << " ";
  path->push_back(v);

  VertexSet *neighbours = &adj_set_[v];

  for (auto &node : *neighbours) {
    if (!(*visited)[node]) {
      DFS(node, visited, path);
    }
  }
  // add saving of path here if post-order traversal
}

VertexSet Graph::GetNeighboursSet(const Vertex &v) {
  if (adj_set_.count(v)) {
    return adj_set_[v];
  }
  return VertexSet();
}

VertexList Graph::GetNeighboursList(const Vertex &v) {
  if (adj_list_.count(v)) {
    return adj_list_[v];
  }
  return VertexList();
}

int Graph::GetSize() { return size_; }

AdjList Graph::GetAdjList() { return adj_list_; }

AdjSet Graph::GetAdjSet() { return adj_set_; }

VertexSet Graph::GetAllVertices() {
  VertexSet result;
  for (auto &kv : adj_set_) {
    // return all keys (meaning the vertex itself)
    result.emplace(kv.first);
  }

  return result;
}
