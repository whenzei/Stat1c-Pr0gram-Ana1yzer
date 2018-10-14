#pragma once

#ifndef SPA_SEMANTIC_EXCEPTION_H
#define SPA_SEMANTIC_EXCEPTION_H

#include <exception>
#include <string>

using std::exception;
using std::string;

struct SemanticErrorExemption : public exception {
  string err_msg_;
  SemanticErrorExemption(const string& msg) : err_msg_(msg) {}

  const char* what() const throw() { return err_msg_.c_str(); }
};

#endif  // !SPA_SEMANTIC_EXCEPTION_H
