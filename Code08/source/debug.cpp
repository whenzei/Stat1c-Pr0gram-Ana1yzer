#include "debug.h"

bool Debug::is_init_;
int Debug::debug_lvl_;

void Debug::Print(DebugLevel debug_lvl, const string str) {
  CheckInit();
  if (IsLogLevelAbove(debug_lvl)) {
    std::cerr << str;
  }
}

void Debug::PrintLn(DebugLevel debug_lvl, const string str) {
  CheckInit();
  if (IsLogLevelAbove(debug_lvl)) {
    std::cerr << str << std::endl;
  }
}

void Debug::SetLevel(DebugLevel debug_lvl) {
  debug_lvl_ = debug_lvl;
  is_init_ = true;
}

bool Debug::IsLogLevelAbove(DebugLevel debug_lvl) {
  return debug_lvl >= debug_lvl_;
}

void Debug::CheckInit() {
  if (!is_init_) {
    Init();
  }
}

void Debug::Init() {
  DebugLevel debug_lvl(Debug::kLog);
  SetLevel(debug_lvl);
}
