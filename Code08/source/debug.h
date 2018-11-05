#pragma once

// original code courtesy of Michael J @ StackOverflow
// https://stackoverflow.com/a/3371986/9445374
// modified by Kar Rui

#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>

using std::string;

class Debug {
 public:
  enum DebugLevel { kAll = 0, kInfo, kLog, kWarning, kError, kFatal, kNone };
  static void Print(DebugLevel debug_lvl, const string str);
  static void PrintLn(DebugLevel debug_lvl, const string str = "");
  static void SetLevel(DebugLevel debug_lvl);
  static bool IsLogLevelAbove(DebugLevel debug_lvl);

 protected:
  static void CheckInit();
  static void Init();

 private:
  Debug();
  static bool is_init_;
  static int debug_lvl_;
};


#endif  // !DEBUG_MSG_H