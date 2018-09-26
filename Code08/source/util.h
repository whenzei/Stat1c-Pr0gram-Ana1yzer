#pragma once

#ifndef SPA_UTIL_H
#define SPA_UTIL_H

#include <iostream>
#include <vector>
using std::string;
using std::vector;

class Util {
public:
  // Checks whether the file located at given filepath exists
  // @param filepath path to file to check
  // @returns true if file exists, false otherwise
  static bool IsValidFile(string filepath);

  // Reads the content of the file located at the given filepath
  // @param filepath path to file
  // @returns contents of file in string form
  static string ReadContentFromFile(string filepath);

  // Splits a given string by a given delimiter
  // NOTE: The string will be trimmed so there are no whitespaces before or
  // after the strings
  // @param &str pointer to string that you want to be delimited
  // @param delim the delimiter character
  // @returns vector<string> of the delimited strings
  static vector<string> Split(const string &str, char delim);

  // Trims whitespace from both ends of a given string in place
  static inline void Trim(string &s);

private:
  static inline void LTrim(string &s);
  static inline void RTrim(string &s);

};
#endif // !SPA_UTIL_H
