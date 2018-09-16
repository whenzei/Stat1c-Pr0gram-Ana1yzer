#pragma once

#include "pkb.h"
#include <sstream>

using std::make_pair;
using std::stringstream;

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

bool PKB::InsertAssignStmt(StmtNumInt stmt_num_int,
                           StmtListIndex stmtlist_index,
                           VarName modified_var_name,
                           VarNameSet used_var_name_set) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kAssign, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertAssignStmt(stmt_num);
    var_list_.InsertVarName(modified_var_name);
    // insert variables
    for (auto& var_name : used_var_name_set) {
      var_list_.InsertVarName(var_name);
	  }
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    modifies_table_.InsertModifies(stmt_num, modified_var_name);
    StmtNumList parents = parent_table_.GetParentT(stmtlist_index);
    for (StmtNum& parent : parents) {
      modifies_table_.InsertModifies(parent, modified_var_name);
    }
    // add uses relationship
    for (auto& var_name : used_var_name_set) {
      uses_table_.InsertUses(var_name, stmt_num);
    }
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertWhileStmt(StmtNumInt stmt_num_int,
                          StmtListIndex parent_stmtlist_index,
                          StmtListIndex child_stmtlist_index,
                          VarNameSet control_var_name_set) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kWhile,
                             parent_stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, parent_stmtlist_index);
    stmt_type_list_.InsertWhileStmt(stmt_num);
    // insert variables
    for (auto& var_name : control_var_name_set) {
      var_list_.InsertVarName(var_name);
    }
    // insert follow relationships
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(parent_stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    // insert parent relationships
    parent_table_.InsertDirectParentRelationship(stmt_num,
                                                 child_stmtlist_index);
    StmtNumList indirect_parents =
        parent_table_.GetParentT(parent_stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     child_stmtlist_index);
    }
    // insert uses relationship
    for (auto& var_name : control_var_name_set) {
      uses_table_.InsertUses(var_name, stmt_num);
    }
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertIfStmt(StmtNumInt stmt_num_int,
                       StmtListIndex parent_stmtlist_index,
                       StmtListIndex then_stmtlist_index,
                       StmtListIndex else_stmtlist_index,
                       VarNameSet control_var_name_set) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kIf, parent_stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, parent_stmtlist_index);
    stmt_type_list_.InsertIfStmt(stmt_num);
    // insert variables
    for (auto& var_name : control_var_name_set) {
      var_list_.InsertVarName(var_name);
    }
    // insert follow relationships
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(parent_stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    // insert parent relationships
    parent_table_.InsertDirectParentRelationship(stmt_num, then_stmtlist_index);
    parent_table_.InsertDirectParentRelationship(stmt_num, else_stmtlist_index);
    StmtNumList indirect_parents =
        parent_table_.GetParentT(parent_stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     then_stmtlist_index);
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     else_stmtlist_index);
    }
    // insert uses relationship
    for (auto& var_name : control_var_name_set) {
      uses_table_.InsertUses(var_name, stmt_num);
    }
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertReadStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                         VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kRead, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertReadStmt(stmt_num);
    // insert variable
    var_list_.InsertVarName(var_name);
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    // insert modifies relationship
    StmtNumList parents = parent_table_.GetParentT(stmtlist_index);
    modifies_table_.InsertModifies(stmt_num, var_name);
    for (StmtNum& parent : parents) {
      modifies_table_.InsertModifies(parent, var_name);
    }
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertPrintStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                          VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kPrint, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertPrintStmt(stmt_num);
    // insert variable
    var_list_.InsertVarName(var_name);
    // insert uses relationship
    uses_table_.InsertUses(var_name, stmt_num);
    // insert follows relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    return true;
  } else {
    return false;
  }
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

StmtNum PKB::GetFollows(StmtNum stmt_num) {
  return follows_table_.GetFollows(stmt_num);
}

StmtList PKB::GetFollowedByT(StmtNum stmt_num) {
  return follows_table_.GetFollowedByT(stmt_num);
}

StmtNum PKB::GetFollowedBy(StmtNum stmt_num) {
  return follows_table_.GetFollowedBy(stmt_num);
}

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

VarNameList PKB::GetAllUsedVar() {
  return uses_table_.GetAllUsedVar();
}

VarNameList PKB::GetUsedVarS(StmtNum stmt_num) {
  return uses_table_.GetAllUsedVar(stmt_num);
}

VarNameList PKB::GetUsedVarP(ProcName proc_name) {
  VarNameList used_var;
  if (find(proc_list_.GetAllProcName().begin(), proc_list_.GetAllProcName().end(), proc_name) != proc_list_.GetAllProcName().end()) {
    used_var = uses_table_.GetAllUsedVar();
  }
  return used_var;
}

StmtNumList PKB::GetAllUsingStmt() {
  return uses_table_.GetAllUsingStmt();
}

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
  if (find(uses_table_.GetAllUsedVar().begin(), uses_table_.GetAllUsedVar().end(), var_name) != uses_table_.GetAllUsedVar().end()) {
    using_proc = proc_list_.GetAllProcName();
  }
  return using_proc;
}

bool PKB::IsUsedByS(StmtNum stmt_num, VarName var_name) {
  return uses_table_.IsUsedBy(var_name, stmt_num);
}

bool PKB::IsUsedByP(ProcName proc_name, VarName var_name) {
  // For iteration 1, return true if given procedure exists and given variable is used anywhere in the program
  if (find(proc_list_.GetAllProcName().begin(), proc_list_.GetAllProcName().end(), proc_name) != proc_list_.GetAllProcName().end() 
    && find(uses_table_.GetAllUsedVar().begin(), uses_table_.GetAllUsedVar().end(), var_name) != uses_table_.GetAllUsedVar().end()) {
    return true;
  }
  return false;
}

bool PKB::HasUsesRelationship() {
  return uses_table_.HasUsesRelationship();
}

StmtVarPairList PKB::GetAllUsesPairS() {
  return uses_table_.GetAllUsesPair();
}

ProcVarPairList PKB::GetAllUsesPairP() {
  VarNameList used_vars = uses_table_.GetAllUsedVar();
  ProcVarPairList proc_var_list;
  for (auto& var : used_vars) {
    proc_var_list.push_back(make_pair(proc_list_.GetAllProcName().front(), var));
  }
  return proc_var_list;
}

StmtNum PKB::ToString(int stmt_num_int) {
  stringstream stmt_num_ss;
  stmt_num_ss << stmt_num_int;
  return stmt_num_ss.str();
}