#pragma once

#include <string>

using std::string;

#ifndef STATEMENT_H
#define STATEMENT_H

class Statement {
private:
  string content_;
  int line_num_;

public: 
  // Default constructor
  Statement() {};

  // Constructor
  Statement(int index, string stmt) {
    line_num_ = index;
    content_ = stmt;
  }

  // Accessors
  string GetContent() {
    return content_;
  }

  int GetLineNum() {
    return line_num_;
  }
};

#endif !STATEMENT_H