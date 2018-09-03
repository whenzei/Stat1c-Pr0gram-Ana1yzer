#pragma once

#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>

using std::string;

class Statement {
private:
  string content_;
  int line_num_;

public:
  Statement() {
  }

  // Constructor
  Statement(int index, string& stmt) {
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