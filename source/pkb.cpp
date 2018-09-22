#pragma once

#include "pkb.h"

using std::make_pair;

bool PKB::InsertProcName(ProcName proc_name) {
  return proc_list_.InsertProcName(proc_name);
}

ProcNameList PKB::GetAllProcName() { return proc_list_.GetAllProcName(); }

VarNameList PKB::GetAllVarName() { return var_list_.GetAllVarName(); }

bool PKB::InsertConstValue(ConstValue const_value) {
  return const_list_.InsertConstValue(const_value);
}

ConstValueList PKB::GetAllConstValue() {
  return const_list_.GetAllConstValue();
}

StmtType PKB::GetStmtType(StmtNum stmt_num) {
  return stmt_table_.GetStmtType(stmt_num);
}

void PKB::InsertAssignStmt(AssignStmtData* stmt_data) {
  if (HandleInsertStatement(stmt_data, StmtType::kAssign)) {
    VarNameSet used_vars = stmt_data->GetUsedVariables();
    VarName modified_var = stmt_data->GetModifiedVariable();
    ConstValueSet used_consts = stmt_data->GetUsedConstants();

    HandleInsertVariables(modified_var, used_vars);
    HandleInsertConstants(used_consts);
  }
}

void PKB::InsertWhileStmt(WhileStmtData* stmt_data) {
  if (HandleInsertStatement(stmt_data, StmtType::kWhile)) {
    VarNameSet used_vars = stmt_data->GetUsedVariables();
    ConstValueSet used_consts = stmt_data->GetUsedConstants();

    HandleInsertVariables(used_vars);
    HandleInsertConstants(used_consts);
  }
}

void PKB::InsertIfStmt(IfStmtData* stmt_data) {
  if (HandleInsertStatement(stmt_data, StmtType::kIf)) {
    VarNameSet used_vars = stmt_data->GetUsedVariables();
    ConstValueSet used_consts = stmt_data->GetUsedConstants();

    HandleInsertVariables(used_vars);
    HandleInsertConstants(used_consts);
  }
}

void PKB::InsertReadStmt(ReadStmtData* stmt_data) {
  if (HandleInsertStatement(stmt_data, StmtType::kRead)) {
    VarName modified_var = stmt_data->GetModifiedVariable();
    HandleInsertVariable(modified_var);
  }
}

void PKB::InsertPrintStmt(PrintStmtData* stmt_data) {
  if (HandleInsertStatement(stmt_data, StmtType::kPrint)) {
    VarName used_var = stmt_data->GetUsedVariable();
    HandleInsertVariable(used_var);
  }
}


void PKB::InsertFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  follows_table_.InsertFollows(followee_stmt_num, follower_stmt_num);
}

void PKB::InsertModifies(StmtNum modifying_stmt, VarName modified_var) {
  modifies_table_.InsertModifies(modifying_stmt, modified_var);
}

void PKB::InsertUses(StmtNum using_stmt, VarName used_var) {
  uses_table_.InsertUses(used_var, using_stmt);
}

void PKB::InsertParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  parent_table_.InsertDirectParentRelationship(parent_stmt_num, child_stmt_num);
}

void PKB::InsertParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  parent_table_.InsertIndirectParentRelationship(parent_stmt_num, child_stmt_num);
}

StmtNumList PKB::GetAllStmt() { return stmt_type_list_.GetAllStmt(); }

StmtNumList PKB::GetAllAssignStmt() {
  return stmt_type_list_.GetAllAssignStmt();
}

StmtNumList PKB::GetAllWhileStmt() { return stmt_type_list_.GetAllWhileStmt(); }

StmtNumList PKB::GetAllIfStmt() { return stmt_type_list_.GetAllIfStmt(); }

StmtNumList PKB::GetAllReadStmt() { return stmt_type_list_.GetAllReadStmt(); }

StmtNumList PKB::GetAllPrintStmt() { return stmt_type_list_.GetAllPrintStmt(); }

bool PKB::IsFollowsT(StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  return follows_table_.IsFollowsT(followee_stmt_num, follower_stmt_num);
}

bool PKB::IsFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  return follows_table_.IsFollows(followee_stmt_num, follower_stmt_num);
}

StmtList PKB::GetFollowsT(StmtNum stmt_num) {
  return follows_table_.GetFollowsT(stmt_num);
}

StmtList PKB::GetFollows(StmtNum stmt_num) {
  return follows_table_.GetFollows(stmt_num);
}

StmtList PKB::GetAllFollows() { return follows_table_.GetAllFollows(); }

StmtList PKB::GetFollowedByT(StmtNum stmt_num) {
  return follows_table_.GetFollowedByT(stmt_num);
}

StmtList PKB::GetFollowedBy(StmtNum stmt_num) {
  return follows_table_.GetFollowedBy(stmt_num);
}

StmtList PKB::GetAllFollowedBy() { return follows_table_.GetAllFollowedBy(); }

bool PKB::HasFollowsRelationship() {
  return follows_table_.HasFollowsRelationship();
}

StmtNumPairList PKB::GetAllFollowsPair() {
  return follows_table_.GetAllFollowsPair();
}

StmtNumPairList PKB::GetAllFollowsTPair() {
  return follows_table_.GetAllFollowsTPair();
}

bool PKB::IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  return parent_table_.IsParent(parent_stmt_num, child_stmt_num);
}

bool PKB::IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  return parent_table_.IsParentT(parent_stmt_num, child_stmt_num);
}

StmtNumList PKB::GetParent(StmtNum stmt_num) {
  StmtNumList direct_parent_stmtnum_list;
  StmtNum direct_parent_stmtnum = parent_table_.GetParent(stmt_num);
  if (direct_parent_stmtnum.compare("") != 0) {
    direct_parent_stmtnum_list.push_back(direct_parent_stmtnum);
  }
  return direct_parent_stmtnum_list;
}

StmtNumList PKB::GetParentT(StmtNum stmt_num) {
  return parent_table_.GetParentT(stmt_num);
}

StmtNumList PKB::GetAllParent() {
  return parent_table_.GetAllParent();
}

StmtNumList PKB::GetChild(StmtNum stmt_num) {
  return parent_table_.GetChild(stmt_num);
}

StmtNumList PKB::GetChildT(StmtNum stmt_num) {
  return parent_table_.GetChildT(stmt_num);
}

StmtNumList PKB::GetAllChild() {
  return parent_table_.GetAllChild();
}

bool PKB::HasParentRelationship() {
  return parent_table_.HasParentRelationship();
}

StmtNumPairList PKB::GetAllParentPair() {
  StmtNumPairList parent_pair_list;
  DirectParentMap parents_map = parent_table_.GetDirectParentMap();
  for (auto entry : parents_map) {
    parent_pair_list.push_back(make_pair(entry.first, entry.second));
  }
  return parent_pair_list;
}

StmtNumPairList PKB::GetAllParentTPair() {
  StmtNumPairList parent_t_pair_list;
  ParentsMap parents_map = parent_table_.GetParentsMap();
  for (auto entry : parents_map) {
    for (StmtNum& parent_stmt_num : entry.second) {
      parent_t_pair_list.push_back(make_pair(parent_stmt_num, entry.first));
    }
  }
  return parent_t_pair_list;
}

bool PKB::IsModifiedByS(StmtNum stmt_num, VarName var_name) {
  return modifies_table_.IsModifiedBy(stmt_num, var_name);
}

bool PKB::IsModifiedByP(ProcName proc_name, VarName var_name) {
  if (proc_name == proc_list_.GetAllProcName().front()) {
    return modifies_table_.IsModified(var_name);
  } else {
    return false;
  }
}

VarNameList PKB::GetModifiedVarS(StmtNum stmt_num) {
  return modifies_table_.GetModifiedVar(stmt_num);
}

VarNameList PKB::GetModifiedVarP(ProcName proc_name) {
  if (proc_name == proc_list_.GetAllProcName().front()) {
    return modifies_table_.GetAllModifiedVar();
  } else {
    return VarNameList();
  }
}

VarNameList PKB::GetAllModifiedVar() {
  return modifies_table_.GetAllModifiedVar();
}

StmtNumList PKB::GetModifyingS(VarName var_name) {
  return modifies_table_.GetModifyingStmt(var_name);
}

ProcNameList PKB::GetModifyingP(VarName var_name) {
  if (modifies_table_.IsModified(var_name)) {
    return proc_list_.GetAllProcName();
  } else {
    return ProcNameList();
  }
}

StmtNumList PKB::GetAllModifyingS() {
  return modifies_table_.GetAllModifyingStmt();
}

ProcNameList PKB::GetAllModifyingP() {
  if (modifies_table_.HasModifiesRelationship()) {
    return proc_list_.GetAllProcName();
  } else {
    return ProcNameList();
  }
}

bool PKB::HasModifiesRelationship() {
  return modifies_table_.HasModifiesRelationship();
}

StmtVarPairList PKB::GetAllModifiesPairS() {
  return modifies_table_.GetAllModifiesPair();
}

ProcVarPairList PKB::GetAllModifiesPairP() {
  ProcVarPairList proc_var_pair_list;
  ProcName proc_name = proc_list_.GetAllProcName().front();
  for (VarName& var_name : modifies_table_.GetAllModifiedVar()) {
    proc_var_pair_list.push_back(make_pair(proc_name, var_name));
  }
  return proc_var_pair_list;
}

VarNameList PKB::GetAllUsedVar() { return uses_table_.GetAllUsedVar(); }

VarNameList PKB::GetUsedVarS(StmtNum stmt_num) {
  return uses_table_.GetAllUsedVar(stmt_num);
}

VarNameList PKB::GetUsedVarP(ProcName proc_name) {
  VarNameList used_var;
  ProcNameList proc_names = proc_list_.GetAllProcName();
  if (find(proc_names.begin(), proc_names.end(), proc_name) !=
      proc_names.end()) {
    used_var = uses_table_.GetAllUsedVar();
  }
  return used_var;
}

StmtNumList PKB::GetAllUsingStmt() { return uses_table_.GetAllUsingStmt(); }

ProcNameList PKB::GetAllUsingProc() {
  ProcNameList using_proc;
  if (uses_table_.HasUsesRelationship()) {
    using_proc = proc_list_.GetAllProcName();
  }
  return using_proc;
}

StmtNumList PKB::GetUsingStmt(VarName var_name) {
  return uses_table_.GetAllUsingStmt(var_name);
}

ProcNameList PKB::GetUsingProc(VarName var_name) {
  ProcNameList using_proc;
  VarNameList used_vars = uses_table_.GetAllUsedVar();
  if (find(used_vars.begin(), used_vars.end(), var_name) != used_vars.end()) {
    using_proc = proc_list_.GetAllProcName();
  }
  return using_proc;
}

bool PKB::IsUsedByS(StmtNum stmt_num, VarName var_name) {
  return uses_table_.IsUsedBy(var_name, stmt_num);
}

bool PKB::IsUsedByP(ProcName proc_name, VarName var_name) {
  ProcNameList proc_names = proc_list_.GetAllProcName();
  VarNameList used_vars = uses_table_.GetAllUsedVar();
  // For iteration 1, return true if given procedure exists and given variable
  // is used anywhere in the program
  if (find(proc_names.begin(), proc_names.end(), proc_name) !=
          proc_names.end() &&
      find(used_vars.begin(), used_vars.end(), var_name) != used_vars.end()) {
    return true;
  }
  return false;
}

bool PKB::HasUsesRelationship() { return uses_table_.HasUsesRelationship(); }

StmtVarPairList PKB::GetAllUsesPairS() { return uses_table_.GetAllUsesPair(); }

ProcVarPairList PKB::GetAllUsesPairP() {
  VarNameList used_vars = uses_table_.GetAllUsedVar();
  ProcVarPairList proc_var_list;
  for (auto& var : used_vars) {
    proc_var_list.push_back(
        make_pair(proc_list_.GetAllProcName().front(), var));
  }
  return proc_var_list;
}

/***********************************
**** Private methods begin here ****
***********************************/
bool PKB::HandleInsertStatement(StatementData* stmt_data, StmtType stmt_type) {
  StmtNum stmt_num = stmt_data->GetStmtNum();
  StmtListIndex stmtlist_index = stmt_data->GetStmtListIndex();

  // stmt already inserted into stmt_table_, no further processing required
  if (!stmt_table_.InsertStmt(stmt_num, stmt_type, stmtlist_index)) {
    return false;
  }
  stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
  stmt_type_list_.InsertStmt(stmt_num, stmt_type);

  return true;
}

void PKB::HandleInsertVariables(VarName variable, VarNameSet var_set) {
  var_list_.InsertVarName(variable);
  for (auto& var_name : var_set) {
    var_list_.InsertVarName(var_name);
  }
}

// overloaded method for just the set
void PKB::HandleInsertVariables(VarNameSet var_set) {
  for (auto& var_name : var_set) {
    var_list_.InsertVarName(var_name);
  }
}

// just a single variable
void PKB::HandleInsertVariable(VarName variable) {
  var_list_.InsertVarName(variable);
}

void PKB::HandleInsertConstants(ConstValueSet constants) {
  for (auto& constant : constants) {
    const_list_.InsertConstValue(constant);
  }
}
