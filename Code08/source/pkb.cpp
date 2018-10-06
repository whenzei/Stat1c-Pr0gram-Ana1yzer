#pragma once

#include "pkb.h"
#include "design_extractor.h"

using std::make_pair;

void PKB::InsertProcName(ProcName proc_name) {
  proc_list_.InsertProcName(proc_name);
}

ProcNameList PKB::GetAllProcName() { return proc_list_.GetAllProcName(); }

VarNameList PKB::GetAllVarName() { return var_list_.GetAllVarName(); }

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
    HandleInsertPattern(StmtType::kAssign, stmt_data);
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

void PKB::InsertCallStmt(CallStmtData* stmt_data) {
  if (HandleInsertStatement(stmt_data, StmtType::kCall)) {
    VarNameSet used_var = stmt_data->GetUsedVars();
    VarNameSet modified_var = stmt_data->GetModifiedVars();
    HandleInsertVariables(used_var);
    HandleInsertVariables(modified_var);
  }
}

void PKB::InsertFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  follows_table_.InsertFollows(followee_stmt_num, follower_stmt_num);
}

void PKB::InsertModifiesS(StmtNum modifying_stmt, VarName modified_var) {
  modifies_table_.InsertModifiesS(modifying_stmt, modified_var);
}

void PKB::InsertModifiesP(ProcName modifying_proc, VarName modified_var) {
  modifies_table_.InsertModifiesP(modifying_proc, modified_var);
}

void PKB::InsertUsesS(StmtNum using_stmt, VarName used_var) {
  uses_table_.InsertUsesS(using_stmt, used_var);
}

void PKB::InsertUsesP(ProcName using_proc, VarName used_var) {
  uses_table_.InsertUsesP(using_proc, used_var);
}

void PKB::InsertParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  parent_table_.InsertDirectParentRelationship(parent_stmt_num, child_stmt_num);
}

void PKB::InsertParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  parent_table_.InsertIndirectParentRelationship(parent_stmt_num,
                                                 child_stmt_num);
}

StmtNumList PKB::GetAllStmt() { return stmt_type_list_.GetAllStmt(); }

StmtNumList PKB::GetAllAssignStmt() {
  return stmt_type_list_.GetAllAssignStmt();
}

StmtNumList PKB::GetAllWhileStmt() { return stmt_type_list_.GetAllWhileStmt(); }

StmtNumList PKB::GetAllIfStmt() { return stmt_type_list_.GetAllIfStmt(); }

StmtNumList PKB::GetAllReadStmt() { return stmt_type_list_.GetAllReadStmt(); }

StmtNumList PKB::GetAllPrintStmt() { return stmt_type_list_.GetAllPrintStmt(); }

StmtNumList PKB::GetAllCallStmt() { return stmt_type_list_.GetAllCallStmt(); }

bool PKB::IsFollowsT(StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  return follows_table_.IsFollowsT(followee_stmt_num, follower_stmt_num);
}

bool PKB::IsFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  return follows_table_.IsFollows(followee_stmt_num, follower_stmt_num);
}

StmtNumList PKB::GetFollowsT(StmtNum stmt_num) {
  return follows_table_.GetFollowsT(stmt_num);
}

StmtNumList PKB::GetFollows(StmtNum stmt_num) {
  return follows_table_.GetFollows(stmt_num);
}

StmtNumList PKB::GetAllFollows() { return follows_table_.GetAllFollows(); }

StmtNumList PKB::GetFollowedByT(StmtNum stmt_num) {
  return follows_table_.GetFollowedByT(stmt_num);
}

StmtNumList PKB::GetFollowedBy(StmtNum stmt_num) {
  return follows_table_.GetFollowedBy(stmt_num);
}

StmtNumList PKB::GetAllFollowedBy() { return follows_table_.GetAllFollowedBy(); }

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

StmtNumList PKB::GetAllParent() { return parent_table_.GetAllParent(); }

StmtNumList PKB::GetChild(StmtNum stmt_num) {
  return parent_table_.GetChild(stmt_num);
}

StmtNumList PKB::GetChildT(StmtNum stmt_num) {
  return parent_table_.GetChildT(stmt_num);
}

StmtNumList PKB::GetAllChild() { return parent_table_.GetAllChild(); }

bool PKB::HasParentRelationship() {
  return parent_table_.HasParentRelationship();
}

StmtNumPairList PKB::GetAllParentPair() {
  return parent_table_.GetAllParentPair();
}

StmtNumPairList PKB::GetAllParentTPair() {
  return parent_table_.GetAllParentTPair();
}

bool PKB::IsModifiedByS(StmtNum stmt_num, VarName var_name) {
  return modifies_table_.IsModifiedByS(stmt_num, var_name);
}

bool PKB::IsModifiedByP(ProcName proc_name, VarName var_name) {
  return modifies_table_.IsModifiedByP(proc_name, var_name);
}

VarNameList PKB::GetModifiedVarS(StmtNum stmt_num) {
  return modifies_table_.GetModifiedVarS(stmt_num);
}

VarNameList PKB::GetModifiedVarP(ProcName proc_name) {
  return modifies_table_.GetModifiedVarP(proc_name);
}

StmtNumList PKB::GetModifyingS(VarName var_name) {
  return modifies_table_.GetModifyingStmt(var_name);
}

ProcNameList PKB::GetModifyingP(VarName var_name) {
  return modifies_table_.GetModifyingProc(var_name);
}

StmtNumList PKB::GetAllModifyingS() {
  return modifies_table_.GetAllModifyingStmt();
}

ProcNameList PKB::GetAllModifyingP() {
  return modifies_table_.GetAllModifyingProc();
}

StmtVarPairList PKB::GetAllModifiesPairS() {
  return modifies_table_.GetAllModifiesPairS();
}

ProcVarPairList PKB::GetAllModifiesPairP() {
  return modifies_table_.GetAllModifiesPairP();
}

VarNameList PKB::GetUsedVarS(StmtNum stmt_num) {
  return uses_table_.GetUsedVarS(stmt_num);
}

VarNameList PKB::GetUsedVarP(ProcName proc_name) {
  return uses_table_.GetUsedVarP(proc_name);
}

StmtNumList PKB::GetAllUsingStmt() { return uses_table_.GetAllUsingStmt(); }

ProcNameList PKB::GetAllUsingProc() { return uses_table_.GetAllUsingProc(); }

StmtNumList PKB::GetUsingStmt(VarName var_name) {
  return uses_table_.GetUsingStmt(var_name);
}

ProcNameList PKB::GetUsingProc(VarName var_name) {
  return uses_table_.GetUsingProc(var_name);
}

bool PKB::IsUsedByS(StmtNum stmt_num, VarName var_name) {
  return uses_table_.IsUsedByS(stmt_num, var_name);
}

bool PKB::IsUsedByP(ProcName proc_name, VarName var_name) {
  return uses_table_.IsUsedByP(proc_name, var_name);
}

StmtVarPairList PKB::GetAllUsesPairS() { return uses_table_.GetAllUsesSPair(); }

ProcVarPairList PKB::GetAllUsesPairP() { return uses_table_.GetAllUsesPPair(); }

StmtNumList PKB::GetAssignWithPattern(VarName var_name, TokenList sub_expr) {
  if (var_name.compare("") == 0) {
    return pattern_table_.GetAssignWithSubExpr(sub_expr);
  } else if (sub_expr.empty()) {
    return pattern_table_.GetAssignWithLfsVar(var_name);
  } else {
    return pattern_table_.GetAssignWithPattern(var_name, sub_expr);
  }
}

StmtNumList PKB::GetAssignWithExactPattern(VarName var_name,
                                           TokenList exact_expr) {
  if (var_name.compare("") == 0) {
    return pattern_table_.GetAssignWithExactExpr(exact_expr);
  } else {
    return pattern_table_.GetAssignWithExactPattern(var_name, exact_expr);
  }
}

StmtVarPairList PKB::GetAllAssignPatternPair(TokenList sub_expr) {
  return pattern_table_.GetAllAssignPatternPair(sub_expr);
}

StmtVarPairList PKB::GetAllAssignExactPatternPair(TokenList exact_expr) {
  return pattern_table_.GetAllAssignExactPatternPair(exact_expr);
}

bool PKB::InsertIndirectCallRelationship(ProcName caller_proc,
                                         ProcName callee_proc) {
  return call_table_.InsertIndirectCallRelationship(caller_proc, callee_proc);
}

bool PKB::InsertDirectCallRelationship(ProcName caller_proc,
                                       ProcName callee_proc) {
  return call_table_.InsertDirectCallRelationship(caller_proc, callee_proc);
}

void PKB::InsertCalls(StmtNum stmt_num, ProcName callee_proc) {
  call_table_.InsertCalls(stmt_num, callee_proc);
}

StmtNumList PKB::GetCallingStmts(ProcName callee_proc) {
  return call_table_.GetCallingStmts(callee_proc);
}

StmtNumProcPairList PKB::GetAllCallingStmtPairs() {
  return call_table_.GetAllCallingStmtPairs();
}

ProcNameList PKB::GetCallee(ProcName caller_proc) {
  return call_table_.GetCallee(caller_proc);
}

ProcNameList PKB::GetCalleeT(ProcName caller_proc) {
  return call_table_.GetCalleeT(caller_proc);
}

ProcNameList PKB::GetCaller(ProcName callee_proc) {
  return call_table_.GetCaller(callee_proc);
}

ProcNameList PKB::GetCallerT(ProcName callee_proc) {
  return call_table_.GetCallerT(callee_proc);
}

ProcNameList PKB::GetAllCaller() { return call_table_.GetAllCaller(); }

ProcNameList PKB::GetAllCallee() { return call_table_.GetAllCallee(); }

ProcNamePairList PKB::GetAllCallPairs() {
  return call_table_.GetAllCallPairs();
}

ProcNamePairList PKB::GetAllCallTPairs() {
  return call_table_.GetAllCallTPairs();
}

bool PKB::IsCall(ProcName caller_proc, ProcName callee_proc) {
  return call_table_.IsCall(caller_proc, callee_proc);
}

bool PKB::IsCallT(ProcName caller_proc, ProcName callee_proc) {
  return call_table_.IsCallT(caller_proc, callee_proc);
}

bool PKB::HasCallsRelationship() { return call_table_.HasCallsRelationship(); }

void PKB::NotifyParseEnd() {
  DesignExtractor de =  DesignExtractor(this);
  de.UpdatePkb();
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

void PKB::HandleInsertPattern(StmtType stmt_type, void* stmt_data) {
  switch (stmt_type) {
    case StmtType::kAssign: {
      AssignStmtData* assign_stmt_data = (AssignStmtData*)stmt_data;
      pattern_table_.InsertAssignPattern(
          assign_stmt_data->GetStmtNum(),
          assign_stmt_data->GetModifiedVariable(),
          assign_stmt_data->GetPostfixedExpr());
      break;
    }
    case StmtType::kWhile: {
      WhileStmtData* while_stmt_data = (WhileStmtData*)stmt_data;
      // TODO: insert while pattern
      break;
    }
    case StmtType::kIf: {
      IfStmtData* if_stmt_data = (IfStmtData*)stmt_data;
      // TODO: insert if pattern
      break;
    }
  }
}
