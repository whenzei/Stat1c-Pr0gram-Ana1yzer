#pragma once

#include <string>

using std::string;

#ifndef STATEMENT_H
#define STATEMENT_H

class Statement {
private:
  string content;
  int line_num;

public: 
  // Default constructor
  Statement() {};

  // Constructor
  Statement(int index, string stmt) {
    line_num = index;
    content = stmt;
  }

  // Accessors
  string GetContent() {
    return content;
  }

  int GetLineNum() {
    return line_num;
  }
};

#endif !STATEMENT_H