#pragma once

#ifndef RESULT_HASHER_H
#define RESULT_HASHER_H

#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

using std::string;
using std::vector;

struct ResultHasher {
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
};


#endif  // !RESULT_HASHER_H
