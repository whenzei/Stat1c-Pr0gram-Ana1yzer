#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
 public:
  static VarTable* var_table_;
  static int SetProcToAst(PROC p, TNode* r);
  static TNode* GetRootAst(PROC p);
};