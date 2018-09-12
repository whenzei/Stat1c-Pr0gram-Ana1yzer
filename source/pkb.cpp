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
                           StmtListIndex stmtlist_index, VarName modified_var_name,
                           VarNameList used_var_name_list) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertAssignStmt(stmt_num);
    var_list_.InsertVarName(modified_var_name);
    for (VarName& var_name : used_var_name_list) {
      var_list_.InsertVarName(var_name);
	  }
    // TODO: add modifies and uses relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    return true;
  } else {
    return false;
  }
}

StmtListIndex PKB::InsertWhileStmt(StmtNumInt stmt_num_int,
                                   StmtListIndex stmtlist_index,
                                   VarNameList control_var_name_list) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertWhileStmt(stmt_num);
    for (VarName& var_name : control_var_name_list) {
      var_list_.InsertVarName(var_name);
    }
    // TODO: insert parent and uses relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    return stmtlist_table_.GetNextStmtListIndex();
  } else {
    return -1;
  }
}

pair<StmtListIndex, StmtListIndex> PKB::InsertIfStmt(
    StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
    VarNameList control_var_name_list) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, stmtlist_index)) {
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertIfStmt(stmt_num);
    for (VarName& var_name : control_var_name_list) {
      var_list_.InsertVarName(var_name);
    }
    // TODO: insert parent and uses relationship
    for (StmtNum& followed_stmt_num : stmtlist_table_.GetStmtNumList(stmtlist_index)) {
      follows_table_.InsertFollows(followed_stmt_num, stmt_num);
    }
    return stmtlist_table_.GetNextTwoStmtListIndices();
  } else {
    return pair<StmtListIndex, StmtListIndex>(-1, -1);
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

bool PKB::IsFollows(StmtNum stmt_num1, StmtNum stmt_num2) { return follows_table_.IsFollows(stmt_num1, stmt_num2);  }

bool PKB::IsDirectFollows(StmtNum stmt_num1, StmtNum stmt_num2) { return follows_table_.IsFollowsT(stmt_num1, stmt_num2);  }

StmtList PKB::GetFollows(StmtNum stmt_num) { return follows_table_.GetFollows(stmt_num); }

StmtNum PKB::GetDirectFollows(StmtNum stmt_num) { return follows_table_.GetFollowsT(stmt_num); }

StmtList PKB::GetFollowedBy(StmtNum stmt_num) { return follows_table_.GetFollowedBy(stmt_num); }

StmtNum PKB::GetDirectFollowedBy(StmtNum stmt_num) { return follows_table_.GetFollowedByT(stmt_num); }

bool PKB::HasFollowsRelationship() { return follows_table_.HasFollowsRelationship(); }

StmtNum PKB::ToString(int stmt_num_int) {
  stringstream stmt_num_ss;
  stmt_num_ss << stmt_num_int;
  return stmt_num_ss.str();
}

