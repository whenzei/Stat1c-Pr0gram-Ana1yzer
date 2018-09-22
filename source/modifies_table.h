#pragma once

#ifndef SPA_MODIFIES_TABLE_H
#define SPA_MODIFIES_TABLE_H

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::list;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using StmtNum = string;
using StmtNumList = vector<string>;
using VarName = string;
using VarNameList = vector<string>;
using VarNameSet = unordered_set<string>;
using ModifiesMap = unordered_map<string, vector<string>>;
using ModifiedByMap = unordered_map<string, vector<string>>;
using StmtVarPairList = vector<pair<string, string>>;
using ProcVarPairList = vector<pair<string, string>>;

class ModifiesTable {
  StmtNumList modifying_stmt_num_list_;
  VarNameList modified_var_name_list_;
  ModifiesMap modifies_map_;
  ModifiedByMap modified_by_map_;

 public:
  void InsertModifies(StmtNum stmt_num, VarName var_name);

  bool IsModifiedBy(StmtNum stmt_num, VarName var_name);

  bool IsModified(VarName var_name);

  VarNameList GetModifiedVar(StmtNum stmt_num);

  VarNameList GetAllModifiedVar();

  StmtNumList GetModifyingStmt(VarName var_name);

  StmtNumList GetAllModifyingStmt();

  bool HasModifiesRelationship();

  StmtVarPairList GetAllModifiesPair();
};

#endif !SPA_MODIFIES_TABLE_H