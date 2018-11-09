#pragma once

#ifndef RESULT_HASHER_H
#define RESULT_HASHER_H

#include <vector>
#include <unordered_set>

namespace {
template <typename T>
std::size_t make_hash(const T& v) {
  return std::hash<T>()(v);
}

// adapted from boost::hash_combine
void hash_combine(std::size_t& h, const std::size_t& v) {
  h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
}

// hash any container
template <typename T>
struct hash_container {
  size_t operator()(const T& v) const {
    size_t h = 0;
    for (const auto& e : v) {
      hash_combine(h, make_hash(e));
    }
    return h;
  }
};
}  // namespace

namespace std {
// support for pair<T,U> if T and U can be hashed
template <typename T, typename U>
struct hash<pair<T, U>> {
  size_t operator()(const pair<T, U>& v) const {
    size_t h = make_hash(v.first);
    hash_combine(h, make_hash(v.second));
    return h;
  }
};

// support for vector<T> if T is hashable
// (the T... is a required trick if the vector has a non-standard allocator)
template <typename... T>
struct hash<vector<T...>> : hash_container<vector<T...>> {};
}


#endif  // !RESULT_HASHER_H
