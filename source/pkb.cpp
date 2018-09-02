#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "pkb.h"
#include "proc_table.h"

int PKB::InsertProc(string proc_name) { return proc_table_.Insert(proc_name); }

vector<string> PKB::GetAllProc() { return proc_table_.List(); }

/* original sample methods that came with pkb.cpp
int PKB::SetProcToAst(PROC p, TNode * r) { return NULL; }

TNode* PKB::GetRootAst(PROC p) { return NULL; }
*/
