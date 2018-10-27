#include "util.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

using std::find_if;
using std::ifstream;
using std::isspace;
using std::istreambuf_iterator;
using std::stringstream;

bool Util::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

string Util::ReadContentFromFile(string filepath) {
  if (!IsValidFile(filepath)) {
    std::cout << "File not found! Content is set to empty string" << std::endl;
    return string();
  }
  // start reading file
  ifstream ifs(filepath);
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

vector<string> Util::Split(const string &str, char delimiter) {
  vector<string> result;
  stringstream ss(str);
  string item;
  while (getline(ss, item, delimiter)) {
    Trim(item);
    result.push_back(item);
  }
  return result;
}

// Trim functions obtained from
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring/25385766
// trim from start (in place)
inline void Util::LTrim(string &s) {
  s.erase(s.begin(),
          find_if(s.begin(), s.end(), [](int ch) { return !isspace(ch); }));
}

// trim from end (in place)
inline void Util::RTrim(string &s) {
  s.erase(
      find_if(s.rbegin(), s.rend(), [](int ch) { return !isspace(ch); }).base(),
      s.end());
}

// trim from both ends (in place)
inline void Util::Trim(string &s) {
  LTrim(s);
  RTrim(s);
}
