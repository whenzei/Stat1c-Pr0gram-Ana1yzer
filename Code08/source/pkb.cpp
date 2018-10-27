#pragma once

#include "pkb.h"
#include "design_extractor.h"

using std::make_pair;

void PKB::InsertProcName(ProcName proc_name) {
  proc_list_.InsertProcName(proc_name);
  next_table_.InsertCFG(proc_name);
}

ProcNameList PKB::GetAllProcNames() { return proc_list_.GetAllProcNames(); }

ProcIndexList PKB::GetAllProcIndices() {
  return proc_list_.GetAllProcIndices();
}

ProcIndexPairList PKB::GetAllProcIndexTwin() {
  return proc_list_.GetAllProcIndexTwin();
}

ProcNamePairList PKB::GetAllProcNameTwin() {
  return proc_list_.GetAllProcNameTwin();
}

ProcIndexMap PKB::GetProcToIndexMapping() {
  return proc_list_.GetProcToIndexMapping();
}

IndexProcMap PKB::GetIndexToProcMapping() {
  return proc_list_.GetIndexToProcMapping();
}

ProcName PKB::GetProcName(ProcIndex index) {
  return proc_list_.GetProcName(index);
}

ProcIndex PKB::GetProcIndex(ProcName proc_name) {
  return proc_list_.GetProcIndex(proc_name);
}

VarIndexList PKB::GetAllVarIndices() { return var_list_.GetAllVarIndices(); }

VarIndexPairList PKB::GetAllVarIndexTwin() {
  return var_list_.GetAllVarIndexTwin();
}

VarIndexMap PKB::GetVarToIndexMapping() {
  return var_list_.GetVarToIndexMapping();
}

IndexVarMap PKB::GetIndexToVarMapping() {
  return var_list_.GetIndexToVarMapping();
}

VarName PKB::GetVarName(VarIndex index) { return var_list_.GetVarName(index); }

VarIndex PKB::GetVarIndex(VarName var_name) {
  return var_list_.GetVarIndex(var_name);
}

ConstValueList PKB::GetAllConstValue() {
  return const_list_.GetAllConstValue();
}

ConstValuePairList PKB::GetAllConstValueTwin() {
  return const_list_.GetAllConstValueTwin();
}

StmtType PKB::GetStmtType(StmtNum stmt_num) {
  return stmt_table_.GetStmtType(stmt_num);
}

ProcIndex PKB::GetProcOfStmt(StmtNum stmt_num) {
  return stmt_table_.GetProcOfStmt(stmt_num);
}

CallGraph* PKB::GetCallGraph() { return &call_graph_; }

void PKB::InsertEdgeInCallGraph(ProcName curr_proc_name,
                                ProcName called_proc_name) {
  // add the procedures into pkb in case they have not been added yet
  this->InsertProcName(curr_proc_name);
  this->InsertProcName(called_proc_name);
  ProcIndex curr_proc_id = this->GetProcIndex(curr_proc_name);
  ProcIndex called_proc_id = this->GetProcIndex(called_proc_name);
  call_graph_.AddEdge(curr_proc_id, called_proc_id);
}

vector<int> PKB::GetToposortedCalls() { return call_graph_.Toposort(); }

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
    HandleInsertPattern(StmtType::kWhile, stmt_data);
  }
}

void PKB::InsertIfStmt(IfStmtData* stmt_data) {
  if (HandleInsertStatement(stmt_data, StmtType::kIf)) {
    VarNameSet used_vars = stmt_data->GetUsedVariables();
    ConstValueSet used_consts = stmt_data->GetUsedConstants();

    HandleInsertVariables(used_vars);
    HandleInsertConstants(used_consts);
    HandleInsertPattern(StmtType::kIf, stmt_data);
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
    ProcName caller_proc_name = stmt_data->GetCallerProcName();
    ProcName callee_proc_name = stmt_data->GetCalleeProcName();
    StmtNum stmt_num = stmt_data->GetStmtNum();
    InsertDirectCallRelationship(caller_proc_name, callee_proc_name);
    InsertCalls(stmt_num, callee_proc_name);
  }
}

void PKB::InsertFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num) {
  follows_table_.InsertFollows(followee_stmt_num, follower_stmt_num);
}

void PKB::InsertModifiesS(StmtNum modifying_stmt, VarName modified_var) {
  // insert the variable if it hasn't already been inserted
  var_list_.InsertVarName(modified_var);
  modifies_table_.InsertModifiesS(modifying_stmt, GetVarIndex(modified_var));
}

void PKB::InsertModifiesS(StmtNum modifying_stmt, VarIndex modified_var_id) {
  modifies_table_.InsertModifiesS(modifying_stmt, modified_var_id);
}

void PKB::InsertModifiesP(ProcName modifying_proc, VarName modified_var) {
  // insert the variable and procedure name if they haven't already been
  // inserted
  proc_list_.InsertProcName(modifying_proc);
  var_list_.InsertVarName(modified_var);
  modifies_table_.InsertModifiesP(GetProcIndex(modifying_proc),
                                  GetVarIndex(modified_var));
}

void PKB::InsertModifiesP(ProcIndex modifying_proc_id,
                          VarIndex modified_var_id) {
  modifies_table_.InsertModifiesP(modifying_proc_id, modified_var_id);
}

void PKB::InsertUsesS(StmtNum using_stmt, VarName used_var) {
  // insert the variable if it hasn't already been inserted
  var_list_.InsertVarName(used_var);
  uses_table_.InsertUsesS(using_stmt, GetVarIndex(used_var));
}

void PKB::InsertUsesS(StmtNum using_stmt, VarIndex used_var_id) {
  uses_table_.InsertUsesS(using_stmt, used_var_id);
}

void PKB::InsertUsesP(ProcName using_proc, VarName used_var) {
  // insert the variable and procedure name if they haven't already been
  // inserted
  proc_list_.InsertProcName(using_proc);
  var_list_.InsertVarName(used_var);
  uses_table_.InsertUsesP(GetProcIndex(using_proc), GetVarIndex(used_var));
}

void PKB::InsertUsesP(ProcIndex using_proc_id, VarIndex used_var_id) {
  uses_table_.InsertUsesP(using_proc_id, used_var_id);
}

void PKB::InsertParent(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  parent_table_.InsertDirectParentRelationship(parent_stmt_num, child_stmt_num);
}

void PKB::InsertParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  parent_table_.InsertIndirectParentRelationship(parent_stmt_num,
                                                 child_stmt_num);
}

void PKB::InsertNext(ProcName proc_name, StmtNum previous_stmt,
                     StmtNum next_stmt) {
  next_table_.InsertNext(proc_name, previous_stmt, next_stmt);
}

void PKB::SetAllNextPairs(StmtNumPairList next_pair_list) {
  next_table_.SetAllNextPairs(next_pair_list);
}

StmtNumList PKB::GetAllStmt() { return stmt_type_list_.GetAllStmt(); }

StmtNumPairList PKB::GetAllStmtTwin() {
  return stmt_type_list_.GetAllStmtTwin();
}

StmtNumList PKB::GetAllAssignStmt() {
  return stmt_type_list_.GetAllAssignStmt();
}

StmtNumPairList PKB::GetAllAssignStmtTwin() {
  return stmt_type_list_.GetAllAssignStmtTwin();
}

StmtNumList PKB::GetAllWhileStmt() { return stmt_type_list_.GetAllWhileStmt(); }

StmtNumPairList PKB::GetAllWhileStmtTwin() {
  return stmt_type_list_.GetAllWhileStmtTwin();
}

StmtNumList PKB::GetAllIfStmt() { return stmt_type_list_.GetAllIfStmt(); }

StmtNumPairList PKB::GetAllIfStmtTwin() {
  return stmt_type_list_.GetAllIfStmtTwin();
}

StmtNumList PKB::GetAllReadStmt() { return stmt_type_list_.GetAllReadStmt(); }

StmtNumPairList PKB::GetAllReadStmtTwin() {
  return stmt_type_list_.GetAllReadStmtTwin();
}

StmtNumList PKB::GetAllPrintStmt() { return stmt_type_list_.GetAllPrintStmt(); }

StmtNumPairList PKB::GetAllPrintStmtTwin() {
  return stmt_type_list_.GetAllPrintStmtTwin();
}

StmtNumList PKB::GetAllCallStmt() { return stmt_type_list_.GetAllCallStmt(); }

StmtNumPairList PKB::GetAllCallStmtTwin() {
  return stmt_type_list_.GetAllCallStmtTwin();
}

bool PKB::IsVarIndex(VarIndex var_id) { return var_list_.IsVarIndex(var_id); }

bool PKB::IsVarName(VarName var_name) { return var_list_.IsVarName(var_name); }

bool PKB::IsStmtNum(StmtNum stmt_num) {
  return stmt_table_.IsStmtNum(stmt_num);
}

bool PKB::IsProcName(ProcName proc_name) {
  return proc_list_.IsProcName(proc_name);
}

bool PKB::IsProcIndex(ProcIndex proc_id) {
  return proc_list_.IsProcIndex(proc_id);
}

bool PKB::IsConstValue(ConstValue const_value) {
  return const_list_.IsConstValue(const_value);
}

bool PKB::IsAssignStmt(StmtNum stmt_num) {
  return GetStmtType(stmt_num) == StmtType::kAssign;
}

bool PKB::IsWhileStmt(StmtNum stmt_num) {
  return GetStmtType(stmt_num) == StmtType::kWhile;
}

bool PKB::IsIfStmt(StmtNum stmt_num) {
  return GetStmtType(stmt_num) == StmtType::kIf;
}

bool PKB::IsReadStmt(StmtNum stmt_num) {
  return GetStmtType(stmt_num) == StmtType::kRead;
}

bool PKB::IsPrintStmt(StmtNum stmt_num) {
  return GetStmtType(stmt_num) == StmtType::kPrint;
}

bool PKB::IsCallStmt(StmtNum stmt_num) {
  return GetStmtType(stmt_num) == StmtType::kCall;
}

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

StmtNumList PKB::GetAllFollowedBy() {
  return follows_table_.GetAllFollowedBy();
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
  return parent_table_.IsParent(parent_stmt_num, child_stmt_num);
}

bool PKB::IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num) {
  return parent_table_.IsParentT(parent_stmt_num, child_stmt_num);
}

StmtNumList PKB::GetParent(StmtNum stmt_num) {
  StmtNumList direct_parent_stmtnum_list;
  StmtNum direct_parent_stmtnum = parent_table_.GetParent(stmt_num);
  if (direct_parent_stmtnum != 0) {
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
  return modifies_table_.IsModifiedByS(stmt_num, GetVarIndex(var_name));
}

bool PKB::IsModifiedByP(ProcName proc_name, VarName var_name) {
  return modifies_table_.IsModifiedByP(GetProcIndex(proc_name),
                                       GetVarIndex(var_name));
}

VarIndexList PKB::GetModifiedVarS(StmtNum stmt_num) {
  return modifies_table_.GetModifiedVarS(stmt_num);
}

VarIndexList PKB::GetModifiedVarP(ProcName proc_name) {
  return modifies_table_.GetModifiedVarP(GetProcIndex(proc_name));
}

VarIndexList PKB::GetModifiedVarP(ProcIndex proc_id) {
  return modifies_table_.GetModifiedVarP(proc_id);
}

StmtNumList PKB::GetModifyingS(VarName var_name) {
  return modifies_table_.GetModifyingStmt(GetVarIndex(var_name));
}

ProcIndexList PKB::GetModifyingP(VarName var_name) {
  return modifies_table_.GetModifyingProc(GetVarIndex(var_name));
}

StmtNumList PKB::GetAllModifyingS() {
  return modifies_table_.GetAllModifyingStmt();
}

ProcIndexList PKB::GetAllModifyingP() {
  return modifies_table_.GetAllModifyingProc();
}

StmtVarIndexPairList PKB::GetAllModifiesPairS() {
  return modifies_table_.GetAllModifiesPairS();
}

ProcVarPairList PKB::GetAllModifiesPairP() {
  return modifies_table_.GetAllModifiesPairP();
}

VarIndexList PKB::GetUsedVarS(StmtNum stmt_num) {
  return uses_table_.GetUsedVarS(stmt_num);
}

VarIndexList PKB::GetUsedVarP(ProcName proc_name) {
  return uses_table_.GetUsedVarP(GetProcIndex(proc_name));
}

VarIndexList PKB::GetUsedVarP(ProcIndex proc_id) {
  return uses_table_.GetUsedVarP(proc_id);
}

StmtNumList PKB::GetAllUsingStmt() { return uses_table_.GetAllUsingStmt(); }

ProcIndexList PKB::GetAllUsingProc() { return uses_table_.GetAllUsingProc(); }

StmtNumList PKB::GetUsingStmt(VarName var_name) {
  return uses_table_.GetUsingStmt(GetVarIndex(var_name));
}

ProcIndexList PKB::GetUsingProc(VarName var_name) {
  return uses_table_.GetUsingProc(GetVarIndex(var_name));
}

bool PKB::IsUsedByS(StmtNum stmt_num, VarName var_name) {
  return uses_table_.IsUsedByS(stmt_num, GetVarIndex(var_name));
}

bool PKB::IsUsedByP(ProcName proc_name, VarName var_name) {
  return uses_table_.IsUsedByP(GetProcIndex(proc_name), GetVarIndex(var_name));
}

StmtVarIndexPairList PKB::GetAllUsesPairS() {
  return uses_table_.GetAllUsesSPair();
}

ProcVarPairList PKB::GetAllUsesPairP() { return uses_table_.GetAllUsesPPair(); }

StmtNumList PKB::GetAssignWithPattern(VarName var_name, TokenList sub_expr) {
  if (var_name.compare("") == 0) {
    return pattern_table_.GetAssignWithSubExpr(sub_expr);
  } else if (sub_expr.empty()) {
    return pattern_table_.GetAssignWithLfsVar(GetVarIndex(var_name));
  } else {
    return pattern_table_.GetAssignWithPattern(GetVarIndex(var_name), sub_expr);
  }
}

StmtNumList PKB::GetAssignWithExactPattern(VarName var_name,
                                           TokenList exact_expr) {
  if (var_name.compare("") == 0) {
    return pattern_table_.GetAssignWithExactExpr(exact_expr);
  } else {
    return pattern_table_.GetAssignWithExactPattern(GetVarIndex(var_name),
                                                    exact_expr);
  }
}

StmtVarPairList PKB::GetAllAssignPatternPair(TokenList sub_expr) {
  return pattern_table_.GetAllAssignPatternPair(sub_expr);
}

StmtVarPairList PKB::GetAllAssignExactPatternPair(TokenList exact_expr) {
  return pattern_table_.GetAllAssignExactPatternPair(exact_expr);
}

StmtNumList PKB::GetWhileWithPattern(VarName var_name) {
  if (var_name.compare("") == 0) {
    return GetAllWhileStmt();
  } else {
    return pattern_table_.GetWhileWithPattern(GetVarIndex(var_name));
  }
}

StmtVarPairList PKB::GetAllWhilePatternPair() {
  return pattern_table_.GetAllWhilePatternPair();
}

StmtNumList PKB::GetIfWithPattern(VarName var_name) {
  if (var_name.compare("") == 0) {
    return GetAllIfStmt();
  } else {
    return pattern_table_.GetIfWithPattern(GetVarIndex(var_name));
  }
}

StmtVarPairList PKB::GetAllIfPatternPair() {
  return pattern_table_.GetAllIfPatternPair();
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
  call_table_.InsertCalls(stmt_num, GetProcIndex(callee_proc));
}

StmtNumList PKB::GetCallingStmts(ProcIndex callee_proc_id) {
  return call_table_.GetCallingStmts(callee_proc_id);
}

StmtNumProcPairList PKB::GetAllCallingStmtPairs() {
  return call_table_.GetAllCallingStmtPairs();
}

ProcIndexList PKB::GetCallee(ProcIndex caller_proc_id) {
  return call_table_.GetCallee(caller_proc_id);
}

ProcNameList PKB::GetCallee(ProcName caller_proc) {
  return call_table_.GetCallee(caller_proc);
}

ProcIndexList PKB::GetCalleeT(ProcName caller_proc) {
  return call_table_.GetCalleeT(GetProcIndex(caller_proc));
}

ProcIndexList PKB::GetCalleeT(ProcIndex caller_proc_id) {
  return call_table_.GetCalleeT(caller_proc_id);
}

ProcIndexList PKB::GetCaller(ProcName callee_proc) {
  return call_table_.GetCaller(GetProcIndex(callee_proc));
}

ProcIndexList PKB::GetCaller(ProcIndex callee_proc_id) {
  return call_table_.GetCaller(callee_proc_id);
}

ProcIndexList PKB::GetCallerT(ProcIndex callee_proc_id) {
  return call_table_.GetCallerT(callee_proc_id);
}

ProcIndexList PKB::GetCallerT(ProcName callee_proc) {
  return call_table_.GetCallerT(GetProcIndex(callee_proc));
}

ProcIndexList PKB::GetAllCaller() { return call_table_.GetAllCaller(); }

ProcNameList PKB::GetAllCallerName() {
  ProcIndexList caller_id_list = GetAllCaller();
  ProcNameList caller_name_list;
  for (auto& proc : caller_id_list) {
    caller_name_list.push_back(GetProcName(proc));
  }
  return caller_name_list;
}

ProcIndexList PKB::GetAllCallee() { return call_table_.GetAllCallee(); }

ProcNameList PKB::GetAllCalleeName() {
  ProcIndexList callee_id_list = GetAllCallee();
  ProcNameList callee_name_list;
  for (auto& proc : callee_id_list) {
    callee_name_list.push_back(GetProcName(proc));
  }
  return callee_name_list;
}

ProcIndexPairList PKB::GetAllCalleeTwin() {
  return call_table_.GetAllCalleeTwin();
}

ProcIndexPairList PKB::GetAllCallPairs() {
  return call_table_.GetAllCallPairs();
}

ProcIndexPairList PKB::GetAllCallTPairs() {
  return call_table_.GetAllCallTPairs();
}

bool PKB::IsCall(ProcName caller_proc, ProcName callee_proc) {
  return call_table_.IsCall(GetProcIndex(caller_proc),
                            GetProcIndex(callee_proc));
}

bool PKB::IsCallT(ProcName caller_proc, ProcName callee_proc) {
  return call_table_.IsCallT(GetProcIndex(caller_proc),
                             GetProcIndex(callee_proc));
}

bool PKB::IsCalledProc(ProcName callee_proc) {
  return call_table_.IsCalledProc(GetProcIndex(callee_proc));
}

bool PKB::IsCalledProc(ProcIndex callee_proc_id) {
  return call_table_.IsCalledProc(callee_proc_id);
}

bool PKB::HasCallsRelationship() { return call_table_.HasCallsRelationship(); }

bool PKB::IsNext(StmtNum previous_stmt, StmtNum next_stmt) {
  return next_table_.IsNext(previous_stmt, next_stmt);
}

bool PKB::IsNext(StmtNum stmt_num) { return next_table_.IsNext(stmt_num); }

bool PKB::IsPrevious(StmtNum stmt_num) {
  return next_table_.IsPrevious(stmt_num);
}

StmtNumList PKB::GetNext(StmtNum stmt_num) {
  return next_table_.GetNext(stmt_num);
}

StmtNumList PKB::GetPrevious(StmtNum stmt_num) {
  return next_table_.GetPrevious(stmt_num);
}

StmtNumList PKB::GetAllNext() { return next_table_.GetAllNext(); }

StmtNumList PKB::GetAllPrevious() { return next_table_.GetAllPrevious(); }

StmtNumPairList PKB::GetAllNextPairs() { return next_table_.GetAllNextPairs(); }

bool PKB::HasNextRelationship() { return next_table_.HasNextRelationship(); }

CFG* PKB::GetCFG(ProcName proc_name) { return next_table_.GetCFG(proc_name); }

CFG* PKB::GetCombinedCFG() { return next_table_.GetCombinedCFG(); }

void PKB::NotifyParseEnd() {
  DesignExtractor de = DesignExtractor(this);
  // exception will be thrown if there are cycles
  de.CheckCyclicCalls();
  de.UpdatePkb();
}

/***********************************
**** Private methods begin here ****
***********************************/
bool PKB::HandleInsertStatement(StatementData* stmt_data, StmtType stmt_type) {
  StmtNum stmt_num = stmt_data->GetStmtNum();
  ProcIndex proc_index = stmt_data->GetProcOfStmt();

  // stmt already inserted into stmt_table_, no further processing required
  if (!stmt_table_.InsertStmt(stmt_num, stmt_type, proc_index)) {
    return false;
  }
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
          GetVarIndex(assign_stmt_data->GetModifiedVariable()),
          assign_stmt_data->GetPostfixedExpr());
      break;
    }
    case StmtType::kWhile: {
      WhileStmtData* while_stmt_data = (WhileStmtData*)stmt_data;
      StmtNum stmt_num = while_stmt_data->GetStmtNum();
      VarNameSet control_variables = while_stmt_data->GetUsedVariables();
      for (auto& control_variable : control_variables) {
        pattern_table_.InsertWhilePattern(stmt_num,
                                          GetVarIndex(control_variable));
      }
      break;
    }
    case StmtType::kIf: {
      IfStmtData* if_stmt_data = (IfStmtData*)stmt_data;
      StmtNum stmt_num = if_stmt_data->GetStmtNum();
      VarNameSet control_variables = if_stmt_data->GetUsedVariables();
      for (auto& control_variable : control_variables) {
        pattern_table_.InsertIfPattern(stmt_num, GetVarIndex(control_variable));
      }
      break;
    }
  }
}
