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
using StmtNumSet = unordered_set<string>;
using VarName = string;
using VarNameList = vector<string>;
using ProcName = string;
using ProcNameList = vector<string>;
using ProcNameSet = unordered_set<string>;
using ModifiesMap = unordered_map<string, vector<string>>;
using ModifiedByMap = unordered_map<string, vector<string>>;
using StmtVarPairList = vector<pair<string, string>>;
using ProcVarPairList = vector<pair<string, string>>;

class ModifiesTable {
  StmtNumList modifying_stmt_list_;
  StmtNumSet modifying_stmt_set_;
  ProcNameList modifying_proc_list_;
  ProcNameSet modifying_proc_set_;
  ModifiesMap modifies_s_map_;
  ModifiedByMap modified_by_s_map_;
  ModifiesMap modifies_p_map_;
  ModifiedByMap modified_by_p_map_;

 public:
  void InsertModifiesS(StmtNum stmt_num, VarName var_name);

  void InsertModifiesP(ProcName proc_name, VarName var_name);

  bool IsModifiedByS(StmtNum stmt_num, VarName var_name);

  bool IsModifiedByP(ProcName proc_name, VarName var_name);

  VarNameList GetModifiedVarS(StmtNum stmt_num);

  VarNameList GetModifiedVarP(ProcName proc_name);

  StmtNumList GetModifyingStmt(VarName var_name);

  StmtNumList GetAllModifyingStmt();

  ProcNameList GetModifyingProc(VarName var_name);

  ProcNameList GetAllModifyingProc();

  StmtVarPairList GetAllModifiesPairS();

  ProcVarPairList GetAllModifiesPairP();
};

#endif !SPA_MODIFIES_TABLE_H