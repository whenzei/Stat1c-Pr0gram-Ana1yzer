#include "graph.h"

Graph::Graph() {
  size_ = 0;
  adj_list_ = unordered_map<int, vector<int>>();
}

bool Graph::AddNode(const string &name) { 
  // node already exists
  if (str_to_int_.count(name)) {
    return false;
  }
  int uniq_id = size_++;
  str_to_int_[name] = uniq_id;
  int_to_str_[uniq_id] = name;

  return true;
}

void Graph::AddEdge(const int &from, const int &to) {
  if (!adj_list_.count(from)) {
    adj_list_.emplace(from, vector<int>());
  }
  adj_list_[from].push_back(to);
}

void Graph::AddEdge(const string &from, const string &to) {
  if (!str_to_int_.count(from)) {
    AddNode(from);
  }
  if (!str_to_int_.count(to)) {
    AddNode(to);
  }

  AddEdge(str_to_int_.at(from), str_to_int_.at(to));
}

unordered_set<string> Graph::GetNeighbourNames(const string &name) {
  unordered_set<string> result;
  if (str_to_int_.count(name) && adj_list_.count(str_to_int_.at(name))) {
    vector<int> node_neighbours = adj_list_.at(str_to_int_.at(name));
    for (const auto node : node_neighbours) {
      result.insert(int_to_str_.at(node));
    }
  }
  return result;
}

int Graph::GetSize() { return size_; }

void Graph::Toposort(const int &v, bool visited[], queue<int> &topoqueue) {
  if (size_ == 0) {
    // only single procedure
    return;
  }

  // mark the current node as visited
  visited[v] = true;

  // recur for all the vertices adjacent to node
  if (adj_list_.count(v)) {
    for (auto node : adj_list_.at(v)) {
      if (!visited[node]) {
        Toposort(node, visited, topoqueue);
      }
    }
  }

  // push to topoqueue;
  topoqueue.push(v);
}

vector<string> Graph::Toposort() {
  queue<int> topoqueue;
  vector<string> toposorted;
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
    string name = int_to_str_.at(topoqueue.front());
    toposorted.push_back(name);
    topoqueue.pop();
  }

  delete[] visited;
  return toposorted;
}

vector<int> Graph::DFS() { return vector<int>(); }

bool Graph::HasCycle(const int &v, vector<int> &adj, bool visited[]) {
  if (!visited[v]) {
    visited[v] = true;

    for (auto node : adj) {
      if (visited[node]) {
        return true;
      }

      if (adj_list_.count(node)) {
        vector<int> neighbours = adj_list_.at(node);

        if (HasCycle(node, neighbours, visited)) {
          return true;
        }
      }
    }
    visited[v] = false;
  }
  return false;
}

bool Graph::HasCycle() {
  // mark all the vertices as not visited
  bool *visited = new bool[size_];
  for (int i = 0; i < size_; i++) {
    visited[i] = false;
  }

  // call the recursive helper function
  for (int i = 0; i < size_; i++) {
    if (!adj_list_.count(i)) {
      continue;
    }
    vector<int> neighbours = adj_list_.at(i);
    if (HasCycle(i, neighbours, visited)) {
      return true;
    }
  }

  delete[] visited;
  return false;
}
