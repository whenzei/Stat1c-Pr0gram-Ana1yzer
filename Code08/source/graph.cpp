#include "graph.h"
#include <iostream>
#include <algorithm>

Graph::Graph() {
  size_ = 0;
  adj_list_ = AdjList();
  adj_set_ = AdjSet();
}

void Graph::SetRoot(const Vertex &new_root) {
  if (adj_set_.count(new_root)) {
    root_ = new_root;
  }
}

void Graph::AddEdge(const Vertex &from, const Vertex &to) {
  if (size_ == 0) {
    // first vertex in graph is the root
    root_ = from;
  }

  AddNode(from);
  AddNode(to);

  if (!adj_set_[from].count(to)) {
    adj_list_[from].push_back(to);
    adj_set_[from].emplace(to);
  }
}

void Graph::RemoveEdge(const Vertex &from, const Vertex &to) {
  if (adj_set_[from].count(to)) {
    // remove from adjList
    VertexList::iterator position = std::find(adj_list_[from].begin(), adj_list_[from].end(), to);
    if (position != adj_list_[from].end()) {
      adj_list_[from].erase(position);
    }
    adj_set_[from].erase(to);
  }
}

void Graph::AddNode(const Vertex & vertex) {
  if (!adj_set_.count(vertex)) {
    size_++;
    adj_set_.emplace(vertex, VertexSet());
    adj_list_.emplace(vertex, VertexList());
  }
}

bool Graph::IsEmpty() { return size_ == 0; }

void Graph::Toposort(const Vertex &v, VisitedMap *visited,
                     queue<Vertex> *topoqueue) {
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
  queue<Vertex> topoqueue;
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

VertexList Graph::GetTerminalNodes() {
  // retrieve all nodes with empty adj_list_
  VertexSet all_vertices = GetAllVertices();
  VertexList result;
  for (auto& vertex : all_vertices) {
    if (adj_list_[vertex].empty()) {
      result.push_back(vertex);
    }
  }

  return result;
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

Vertex Graph::GetRoot() { return root_; }
