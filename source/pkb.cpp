#pragma once

#include "pkb.h"
#include <sstream>

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

bool PKB::InsertAssignStmt(StmtNumInt stmt_num_int,
                           StmtListIndex stmtlist_index,
                           VarName modified_var_name,
                           VarNameSet used_var_name_set) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertAssignStmt(stmt_num);
    var_list_.InsertVarName(modified_var_name);
    // insert variables
    for (auto& var_name : used_var_name_set) {
      var_list_.InsertVarName(var_name);
<<<<<<< HEAD
	  }
    // TODO: add modifies and uses relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
=======
    }
    // TODO: add follows, modifies and uses relationship
>>>>>>> master
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
  if (stmt_table_.InsertStmt(stmt_num, parent_stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, parent_stmtlist_index);
    stmt_type_list_.InsertWhileStmt(stmt_num);
    // insert variables
    for (auto& var_name : control_var_name_set) {
      var_list_.InsertVarName(var_name);
    }
<<<<<<< HEAD
    // TODO: insert parent and uses relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    return stmtlist_table_.GetNextStmtListIndex();
=======
    // insert parent relationships
    parent_table_.InsertDirectParentRelationship(stmt_num,
                                                 child_stmtlist_index);
    StmtNumList indirect_parents = parent_table_.GetParents(parent_stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     child_stmtlist_index);
	}
    // TODO: insert follows and uses relationship
    return true;
>>>>>>> master
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
  if (stmt_table_.InsertStmt(stmt_num, parent_stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, parent_stmtlist_index);
    stmt_type_list_.InsertIfStmt(stmt_num);
	// insert variables
    for (auto& var_name : control_var_name_set) {
      var_list_.InsertVarName(var_name);
    }
<<<<<<< HEAD
    // TODO: insert parent and uses relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    return stmtlist_table_.GetNextTwoStmtListIndices();
=======
    // insert parent relationships
    parent_table_.InsertDirectParentRelationship(
        stmt_num, then_stmtlist_index);
    parent_table_.InsertDirectParentRelationship(
        stmt_num, else_stmtlist_index);
    StmtNumList indirect_parents = parent_table_.GetParents(parent_stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(
          indirect_parent, then_stmtlist_index);
      parent_table_.InsertIndirectParentRelationship(
          indirect_parent, else_stmtlist_index);
    }
    // TODO: insert follows and uses relationship
    return true;
>>>>>>> master
  } else {
    return false;
  }
}

bool PKB::InsertReadStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                         VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertReadStmt(stmt_num);
    var_list_.InsertVarName(var_name);
    // TODO: insert modifies relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertPrintStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                          VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertPrintStmt(stmt_num);
    var_list_.InsertVarName(var_name);
    // TODO: insert uses relationship
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

bool PKB::IsFollows(StmtNumInt followee_stmt_num_int, StmtNumInt follower_stmt_num_int) { 
  StmtNum followee_stmt_num = ToString(followee_stmt_num_int);
  StmtNum follower_stmt_num = ToString(follower_stmt_num_int);;
  return follows_table_.IsFollows(followee_stmt_num, follower_stmt_num);
}

bool PKB::IsFollowsT(StmtNumInt followee_stmt_num_int, StmtNumInt follower_stmt_num_int) {
  StmtNum followee_stmt_num = ToString(followee_stmt_num_int);
  StmtNum follower_stmt_num = ToString(follower_stmt_num_int);;
  return follows_table_.IsFollowsT(followee_stmt_num, follower_stmt_num);
}

StmtList PKB::GetFollows(StmtNumInt stmt_num_int) {
  StmtNum followee_stmt_num = ToString(stmt_num_int);
  return follows_table_.GetFollows(followee_stmt_num);
}

StmtNum PKB::GetFollowsT(StmtNumInt stmt_num_int) {
  StmtNum followee_stmt_num = ToString(stmt_num_int);
  return follows_table_.GetFollowsT(followee_stmt_num);
}

StmtList PKB::GetFollowedBy(StmtNumInt stmt_num_int) {
  StmtNum follower_stmt_num = ToString(stmt_num_int);
  return follows_table_.GetFollowedBy(follower_stmt_num);
}

StmtNum PKB::GetFollowedByT(StmtNumInt stmt_num_int) {
StmtNum follower_stmt_num = ToString(stmt_num_int);
  return follows_table_.GetFollowedByT(followee_stmt_num);
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

bool PKB::IsDirectParent(StmtNumInt parent_stmt_num_int, StmtNumInt child_stmt_num_int) {
  StmtNum parent_stmt_num = ToString(parent_stmt_num_int);
  StmtNum child_stmt_num = ToString(child_stmt_num_int);
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsDirectParent(parent_stmt_num, child_stmtlist_index);
}

bool PKB::IsParent(StmtNumInt parent_stmt_num_int, StmtNumInt child_stmt_num_int) {
  StmtNum parent_stmt_num = ToString(parent_stmt_num_int);
  StmtNum child_stmt_num = ToString(child_stmt_num_int);
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsParent(parent_stmt_num, child_stmtlist_index);
}

StmtNumList PKB::GetDirectParent(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  StmtNumList direct_parent_stmtnum_list;
  StmtNum direct_parent_stmtnum = parent_table_.GetDirectParent(stmtlist_index);
  if (direct_parent_stmtnum.compare("") != 0) {
    direct_parent_stmtnum_list.push_back(direct_parent_stmtnum);
  }
  return direct_parent_stmtnum_list;
}

StmtNumList PKB::GetAllParent(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  return parent_table_.GetParents(stmtlist_index);
}

StmtNumList PKB::GetDirectChild(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetDirectChildren(stmt_num);
  StmtNumList result;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    result.insert(result.end(), stmt_num_list.begin(), stmt_num_list.end());
  }
  return result;
}

StmtNumList PKB::GetAllChild(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetChildren(stmt_num);
  StmtNumList result;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    result.insert(result.end(), stmt_num_list.begin(), stmt_num_list.end());
  }
  return result;
}

bool PKB::HasParentRelationship() { return parent_table_.HasParentRelationship(); }

StmtNum PKB::ToString(int stmt_num_int) {
  stringstream stmt_num_ss;
  stmt_num_ss << stmt_num_int;
  return stmt_num_ss.str();
}

