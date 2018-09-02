#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "pkb.h"
#include "proc_table.h"
#include "assign_table.h"

int PKB::InsertProc(string proc_name) { return proc_table_.Insert(proc_name); }

vector<string> PKB::GetAllProc() { return proc_table_.List(); }

int PKB::InsertAssign(int statement_number, string assign_statement) {
  return assign_table_.Insert(statement_number, assign_statement);
}

vector<string> PKB::GetAllAssign() { return assign_table_.List(); }

/* original sample methods that came with pkb.cpp
int PKB::SetProcToAst(PROC p, TNode * r) { return NULL; }

TNode* PKB::GetRootAst(PROC p) { return NULL; }
*/
