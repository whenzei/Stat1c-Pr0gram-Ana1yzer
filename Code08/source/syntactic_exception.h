#pragma once

#ifndef SPA_SYNTACTIC_EXCEPTION_H
#define SPA_SYNTACTIC_EXCEPTION_H

#include <exception>
#include <string>

using std::exception;
using std::string;

struct SyntacticErrorExemption : public exception {
  string err_msg_;
  SyntacticErrorExemption(const string& msg) : err_msg_(msg) {}

  const char* what() const throw() { return err_msg_.c_str(); }
};

#endif  // !SPA_SYNTACTIC_EXCEPTION_H
