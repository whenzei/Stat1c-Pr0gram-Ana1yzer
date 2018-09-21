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

bool PKB::InsertAssignStmt(AssignStmtData* stmt_data) {
  if (!HandleInsertStatement(stmt_data, StmtType::kAssign)) {
    return false;
  }

  VarNameSet used_vars = stmt_data->GetUsedVariables();
  VarName modified_var = stmt_data->GetModifiedVariable();
  ConstValueSet used_consts = stmt_data->GetUsedConstants();

  HandleInsertVariables(modified_var, used_vars);
  HandleInsertConstants(used_consts);
  HandleUses(stmt_data, used_vars);
  HandleModifies(stmt_data, modified_var);

  // just in case, update parents
  UpdateParentModifies(stmt_data, modified_var);
  UpdateParentUses(stmt_data, used_vars);

  return true;
}

bool PKB::InsertWhileStmt(WhileStmtData* stmt_data) {
  if (!HandleInsertStatement(stmt_data, StmtType::kWhile)) {
    return false;
  }
  VarNameSet used_vars = stmt_data->GetUsedVariables();
  ConstValueSet used_consts = stmt_data->GetUsedConstants();
  StmtListIndex child_stmtlist_index = stmt_data->GetChildStmtListIndex();

  HandleInsertVariables(used_vars);
  HandleInsertConstants(used_consts);
  HandleUses(stmt_data, used_vars);

  UpdateParentRelationship(stmt_data, child_stmtlist_index);


  UpdateModifiesFromChild(stmt_data, child_stmtlist_index);
  UpdateUsesFromChild(stmt_data, child_stmtlist_index);

  UpdateParentUses(stmt_data, used_vars);

  return true;
}

bool PKB::InsertIfStmt(IfStmtData* stmt_data) {
  if (!HandleInsertStatement(stmt_data, StmtType::kIf)) {
    return false;
  }
  VarNameSet used_vars = stmt_data->GetUsedVariables();
  ConstValueSet used_consts = stmt_data->GetUsedConstants();
  StmtListIndex then_stmtlist_index = stmt_data->GetThenStmtListIndex();
  StmtListIndex else_stmtlist_index = stmt_data->GetElseStmtListIndex();

  HandleInsertVariables(used_vars);
  HandleInsertConstants(used_consts);
  HandleUses(stmt_data, used_vars);

  UpdateParentRelationship(stmt_data, then_stmtlist_index);
  UpdateParentRelationship(stmt_data, else_stmtlist_index);

  UpdateModifiesFromChild(stmt_data, then_stmtlist_index);
  UpdateModifiesFromChild(stmt_data, else_stmtlist_index);
  UpdateUsesFromChild(stmt_data, then_stmtlist_index);
  UpdateUsesFromChild(stmt_data, else_stmtlist_index);

  UpdateParentUses(stmt_data, used_vars);

  return true;
}

bool PKB::InsertReadStmt(ReadStmtData* stmt_data) {
  if (!HandleInsertStatement(stmt_data, StmtType::kRead)) {
    return false;
  }

  VarName modified_var = stmt_data->GetModifiedVariable();

  HandleInsertVariable(modified_var);
  HandleModifies(stmt_data, modified_var);

  UpdateParentModifies(stmt_data, modified_var);

  return true;
}

bool PKB::InsertPrintStmt(PrintStmtData* stmt_data) {
  if (!HandleInsertStatement(stmt_data, StmtType::kPrint)) {
    return false;
  }

  VarName used_var = stmt_data->GetUsedVariable();

  HandleInsertVariable(used_var);
  HandleUses(stmt_data, used_var);

  UpdateParentUses(stmt_data, VarNameSet{used_var});

  return true;
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
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsParent(parent_stmt_num, child_stmtlist_index);
}

bool PKB::IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsParentT(parent_stmt_num, child_stmtlist_index);
}

StmtNumList PKB::GetParent(StmtNum stmt_num) {
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  StmtNumList direct_parent_stmtnum_list;
  StmtNum direct_parent_stmtnum = parent_table_.GetParent(stmtlist_index);
  if (direct_parent_stmtnum.compare("") != 0) {
    direct_parent_stmtnum_list.push_back(direct_parent_stmtnum);
  }
  return direct_parent_stmtnum_list;
}

StmtNumList PKB::GetParentT(StmtNum stmt_num) {
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  return parent_table_.GetParentT(stmtlist_index);
}

StmtNumList PKB::GetAllParent() {
  ParentsSet parent_stmt_num_set = parent_table_.GetParentsSet();
  StmtNumList parent_stmt_num_list;
  for (auto parent_stmt_num : parent_stmt_num_set) {
    parent_stmt_num_list.push_back(parent_stmt_num);
  }
  return parent_stmt_num_list;
}

StmtNumList PKB::GetChild(StmtNum stmt_num) {
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetChild(stmt_num);
  StmtNumList result;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    result.insert(result.end(), stmt_num_list.begin(), stmt_num_list.end());
  }
  return result;
}

StmtNumList PKB::GetChildT(StmtNum stmt_num) {
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetChildT(stmt_num);
  StmtNumList child_stmt_num_list;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    child_stmt_num_list.insert(child_stmt_num_list.end(), stmt_num_list.begin(),
                               stmt_num_list.end());
  }
  return child_stmt_num_list;
}

StmtNumList PKB::GetAllChild() {
  ChildrenSet child_stmtlist_index_set = parent_table_.GetChildrenSet();
  StmtNumList child_stmt_num_list;
  for (auto child_stmtlist_index : child_stmtlist_index_set) {
    StmtNumList stmt_num_list =
        stmtlist_table_.GetStmtNumList(child_stmtlist_index);
    child_stmt_num_list.insert(child_stmt_num_list.end(), stmt_num_list.begin(),
                               stmt_num_list.end());
  }
  return child_stmt_num_list;
}

bool PKB::HasParentRelationship() {
  return parent_table_.HasParentRelationship();
}

StmtNumPairList PKB::GetAllParentPair() {
  StmtNumPairList parent_pair_list;
  DirectParentMap parents_map = parent_table_.GetDirectParentMap();
  for (auto entry : parents_map) {
    StmtNumList child_stmt_num_list =
        stmtlist_table_.GetStmtNumList(entry.first);
    for (StmtNum& child_stmt_num : child_stmt_num_list) {
      parent_pair_list.push_back(make_pair(entry.second, child_stmt_num));
    }
  }
  return parent_pair_list;
}

StmtNumPairList PKB::GetAllParentTPair() {
  StmtNumPairList parent_t_pair_list;
  ParentsMap parents_map = parent_table_.GetParentsMap();
  for (auto entry : parents_map) {
    StmtNumList child_stmt_num_list =
        stmtlist_table_.GetStmtNumList(entry.first);
    for (StmtNum& child_stmt_num : child_stmt_num_list) {
      for (StmtNum& parent_stmt_num : entry.second) {
        parent_t_pair_list.push_back(
            make_pair(parent_stmt_num, child_stmt_num));
      }
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
void PKB::HandleUses(StatementData* stmt_data, VarNameSet used_vars) {
  StmtListIndex stmt_list_index = stmt_data->GetStmtListIndex();
  StmtNum stmt_num = stmt_data->GetStmtNum();

  for (auto& var_name : used_vars) {
    uses_table_.InsertUses(var_name, stmt_num, stmt_list_index);
  }
}

void PKB::HandleUses(StatementData* stmt_data, VarName used_var) {
  StmtListIndex stmt_list_index = stmt_data->GetStmtListIndex();
  StmtNum stmt_num = stmt_data->GetStmtNum();

  uses_table_.InsertUses(used_var, stmt_num, stmt_list_index);
}

void PKB::UpdateParentUses(StatementData* stmt_data, VarNameSet used_vars) {
  StmtNum stmt_num = stmt_data->GetStmtNum();
  StmtListIndex stmt_list_index = stmt_data->GetStmtListIndex();

  StmtNumList indirect_parents = parent_table_.GetParentT(stmt_list_index);
  // insert uses relationship for parents
  for (auto& var_name : used_vars) {
    for (StmtNum& indirect_parent : indirect_parents) {
      uses_table_.InsertUses(var_name, indirect_parent,
                             stmt_table_.GetStmtListIndex(indirect_parent));
    }
  }
}

void PKB::UpdateParentModifies(StatementData* stmt_data, VarName modified_var) {
  StmtListIndex stmt_list_index = stmt_data->GetStmtListIndex();
  StmtNum stmt_num = stmt_data->GetStmtNum();
  StmtNumList indirect_parents = parent_table_.GetParentT(stmt_list_index);
  for (StmtNum& indirect_parent : indirect_parents) {
    modifies_table_.InsertModifies(
        indirect_parent, stmt_table_.GetStmtListIndex(indirect_parent),
        modified_var);
  }
}

void PKB::HandleModifies(StatementData* stmt_data, VarName modified_var) {
  StmtListIndex stmt_list_index = stmt_data->GetStmtListIndex();
  StmtNum stmt_num = stmt_data->GetStmtNum();

  modifies_table_.InsertModifies(stmt_num, stmt_list_index, modified_var);
}

void PKB::UpdateModifiesFromChild(StatementData* stmt_data,
                                  StmtListIndex child_stmtlist_index) {
  StmtListIndex stmt_list_index = stmt_data->GetStmtListIndex();
  StmtNum stmt_num = stmt_data->GetStmtNum();

  // insert modifies relationship (if any)
  VarNameSet var_modified_by_children =
      modifies_table_.GetModifiedVar(child_stmtlist_index);
  for (auto var_name : var_modified_by_children) {
    modifies_table_.InsertModifies(stmt_num, stmt_list_index, var_name);
  }
}

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


void PKB::InsertFollows (StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  follows_table_.InsertFollows(followee_stmt_num, follower_stmt_num);
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

void PKB::UpdateParentRelationship(StatementData* stmt_data,
                                   StmtListIndex child_stmtlist_index) {
  StmtNum stmt_num = stmt_data->GetStmtNum();
  StmtListIndex stmt_list_index = stmt_data->GetStmtListIndex();
  // insert parent relationships
  parent_table_.InsertDirectParentRelationship(stmt_num, child_stmtlist_index);

  StmtNumList indirect_parents = parent_table_.GetParentT(stmt_list_index);

  StmtNumList children_stmtnum_list =
      stmtlist_table_.GetStmtNumList(child_stmtlist_index);

  for (StmtNum& child_stmtnum : children_stmtnum_list) {
    StmtListIndexList indirect_children_stmtlist_indices =
        parent_table_.GetChildT(child_stmtnum);
    for (StmtListIndex& indirect_child_stmtlist_index :
         indirect_children_stmtlist_indices) {
      parent_table_.InsertIndirectParentRelationship(
          stmt_num, indirect_child_stmtlist_index);
      for (StmtNum& indirect_parent : indirect_parents) {
        parent_table_.InsertIndirectParentRelationship(
            indirect_parent, indirect_child_stmtlist_index);
      }
    }
  }

  for (StmtNum& indirect_parent : indirect_parents) {
    parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                   child_stmtlist_index);
  }
}

void PKB::UpdateUsesFromChild(StatementData* stmt_data,
                              StmtListIndex child_stmtlist_index) {
  StmtNum stmt_num = stmt_data->GetStmtNum();
  StmtListIndex stmtlist_index = stmt_data->GetStmtListIndex();
  // insert uses relationship for already added children's variables
  for (auto& child_var_name :
       uses_table_.GetVarUsedByStmtList(child_stmtlist_index)) {
    uses_table_.InsertUses(child_var_name, stmt_num, stmtlist_index);
  }
}