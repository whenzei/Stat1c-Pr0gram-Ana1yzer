#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_enum.h"
#include "pql_evaluator.h"
#include "pql_query.h"

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::vector;

PqlEvaluator::PqlEvaluator() {}

list<string> PqlEvaluator::GetResultFromQuery(PqlQuery* query, PKB pkb) {
  setSelectVar(query->GetVarName());
  setDeclarations(query->GetDeclarations());
  setSuchthat(query->GetSuchThats());
  setPKB(pkb);
  // Default value should be true, unless the clause returns a false
  setClauseFlag(true);
  list<string> results;

  // Determine the declaration type of the select variable
  setSelectType(CheckSelectDeclarationType(getSelectVar()));

  // If there is no such that clause, then evaluator will use
  // GetResultFromSelectAllQuery method
  if (getSuchthat().size() == 0) {
    PqlDeclarationEntity select_type = getSelectType();
    results = GetResultFromSelectAllQuery(select_type);
  }
  // Else use GetResultFromQueryWithSuchThat method
  else {
    PqlSuchthat suchthat = getSuchthat().front();
    results = GetResultFromQueryWithSuchThat(suchthat);
  }

  cout << "Result size: " << results.size() << endl;

  return results;
}

list<string> PqlEvaluator::GetResultFromQueryWithSuchThat(
    PqlSuchthat suchthat) {
  PqlDeclarationEntity select_type = getSelectType();
  string select_var_name = getSelectVar();
  list<string> results;

  PqlArrangementOfSynonymInSuchthatParam arrangement =
      CheckArrangementOfSynonymInSuchthatParam(select_var_name,
                                               suchthat.GetParameters());

  switch (suchthat.GetType()) {
    case PqlSuchthatType::kFollows:
      return EvaluateFollows(select_type, suchthat, arrangement);
    case PqlSuchthatType::kFollowsT:
      return EvaluateFollowsT(select_type, suchthat, arrangement);
    case PqlSuchthatType::kParent:
      return EvaluateParent(select_type, suchthat, arrangement);
    case PqlSuchthatType::kParentT:
      return EvaluateParentT(select_type, suchthat, arrangement);
    case PqlSuchthatType::kUsesS:
      return EvaluateUsesS(select_type, suchthat, arrangement);
    case PqlSuchthatType::kModifiesS:
      return EvaluateModifiesS(select_type, suchthat, arrangement);
    case PqlSuchthatType::kModifiesP:
      return EvaluateModifiesP(select_type, suchthat, arrangement);

  }

  return results;
}

list<string> PqlEvaluator::GetResultFromSelectAllQuery(
    PqlDeclarationEntity select_type) {
  PKB pkb = getPKB();
  list<string> results;

  switch (select_type) {
    case PqlDeclarationEntity::kProcedure:
      // Get all procedures name from PKB and store into
      // results list
      cout << "Select all procedure." << endl;
      return pkb.GetAllProcName();
    case PqlDeclarationEntity::kVariable:
      // Get all variable name from PKB and store into
      // results list
      cout << "Select all variables." << endl;
      return pkb.GetAllVarName();
    case PqlDeclarationEntity::kAssign:
      // Get all statement number of statement which
      // contains assignment from PKB and store into results
      // list
      cout << "Select all assign statement." << endl;
      return pkb.GetAllAssignStmt();
    case PqlDeclarationEntity::kStmt:
      // Get all stmt number from PKB and store into results
      // list
      cout << "Select all statement." << endl;
      return pkb.GetAllStmt();
    case PqlDeclarationEntity::kRead:
      // Get all read stmt from PKB and store into results
      // list
      cout << "Select all read statement." << endl;
      return pkb.GetAllReadStmt();
    case PqlDeclarationEntity::kPrint:
      // Get all print stmt from PKB and store into results
      // list
      cout << "Select all print statement." << endl;
      return pkb.GetAllPrintStmt();
    case PqlDeclarationEntity::kCall:
      // Get all call stmt from PKB and store into results
      // list
      break;
    case PqlDeclarationEntity::kWhile:
      // Get all while stmt from PKB and store into results
      // list
      cout << "Select all while statement." << endl;
      return pkb.GetAllWhileStmt();
    case PqlDeclarationEntity::kIf:
      // Get all if stmt from PKB and store into results
      // list
      cout << "Select all if statement." << endl;
      return pkb.GetAllIfStmt();
    case PqlDeclarationEntity::kConstant:
      // Get all constant from PKB and store into results
      // list
      cout << "Select all constants." << endl;
      return pkb.GetAllConstValue();
    case PqlDeclarationEntity::kProgline:
      // Get all program line from PKB and store into
      // results list
      cout << "Select all program lines." << endl;
      return pkb.GetAllStmt();
  }

  // Return empty result if nothing is found
  return results;
}

list<string> PqlEvaluator::EvaluateFollows(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat,
    PqlArrangementOfSynonymInSuchthatParam arrangement) {
  list<string> results;
  PKB pkb = getPKB();

  switch (arrangement) {
    case kNoSynonym:
      break;
    case kNoSynonymUnderscoreLeft:
      break;
    case kNoSynonymUnderscoreRight:
      break;
    case kNoSynonymUnderscoreBoth:
      break;
    case kOneSynonymLeft:
      break;
    case kOneSynonymLeftUnderscoreRight:
      break;
    case kOneSynonymRight:
      break;
    case kOneSynonymRightUnderscoreLeft:
      break;
    case kOneSynonymSelectLeft:
      break;
    case kOneSynonymSelectLeftUnderscoreRight:
      break;
    case kOneSynonymSelectRight:
      break;
    case kOneSynonymSelectRightUnderscoreLeft:
      break;
    case kTwoSynonym:
      break;
    case kTwoSynonymSelectLeft:
      break;
    case kTwoSynonymSelectRight:
      break;
  }

  return results;
}

list<string> PqlEvaluator::EvaluateFollowsT(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat,
    PqlArrangementOfSynonymInSuchthatParam arrangement) {
  list<string> results;
  PKB pkb = getPKB();

  switch (arrangement) {
    case kNoSynonym:
      break;
    case kNoSynonymUnderscoreLeft:
      break;
    case kNoSynonymUnderscoreRight:
      break;
    case kNoSynonymUnderscoreBoth:
      break;
    case kOneSynonymLeft:
      break;
    case kOneSynonymLeftUnderscoreRight:
      break;
    case kOneSynonymRight:
      break;
    case kOneSynonymRightUnderscoreLeft:
      break;
    case kOneSynonymSelectLeft:
      break;
    case kOneSynonymSelectLeftUnderscoreRight:
      break;
    case kOneSynonymSelectRight:
      break;
    case kOneSynonymSelectRightUnderscoreLeft:
      break;
    case kTwoSynonym:
      break;
    case kTwoSynonymSelectLeft:
      break;
    case kTwoSynonymSelectRight:
      break;
  }

  return results;
}

list<string> PqlEvaluator::EvaluateParent(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat,
    PqlArrangementOfSynonymInSuchthatParam arrangement) {
  list<string> results;
  PKB pkb = getPKB();

  switch (arrangement) {
    case kNoSynonym:
      break;
    case kNoSynonymUnderscoreLeft:
      break;
    case kNoSynonymUnderscoreRight:
      break;
    case kNoSynonymUnderscoreBoth:
      break;
    case kOneSynonymLeft:
      break;
    case kOneSynonymLeftUnderscoreRight:
      break;
    case kOneSynonymRight:
      break;
    case kOneSynonymRightUnderscoreLeft:
      break;
    case kOneSynonymSelectLeft:
      break;
    case kOneSynonymSelectLeftUnderscoreRight:
      break;
    case kOneSynonymSelectRight:
      break;
    case kOneSynonymSelectRightUnderscoreLeft:
      break;
    case kTwoSynonym:
      break;
    case kTwoSynonymSelectLeft:
      break;
    case kTwoSynonymSelectRight:
      break;
  }

  return results;
}

list<string> PqlEvaluator::EvaluateParentT(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat,
    PqlArrangementOfSynonymInSuchthatParam arrangement) {
  list<string> results;
  PKB pkb = getPKB();

  switch (arrangement) {
    case kNoSynonym:
      break;
    case kNoSynonymUnderscoreLeft:
      break;
    case kNoSynonymUnderscoreRight:
      break;
    case kNoSynonymUnderscoreBoth:
      break;
    case kOneSynonymLeft:
      break;
    case kOneSynonymLeftUnderscoreRight:
      break;
    case kOneSynonymRight:
      break;
    case kOneSynonymRightUnderscoreLeft:
      break;
    case kOneSynonymSelectLeft:
      break;
    case kOneSynonymSelectLeftUnderscoreRight:
      break;
    case kOneSynonymSelectRight:
      break;
    case kOneSynonymSelectRightUnderscoreLeft:
      break;
    case kTwoSynonym:
      break;
    case kTwoSynonymSelectLeft:
      break;
    case kTwoSynonymSelectRight:
      break;
  }

  return results;
}

list<string> PqlEvaluator::EvaluateUsesS(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat,
    PqlArrangementOfSynonymInSuchthatParam arrangement) {
  list<string> results;
  PKB pkb = getPKB();

  switch (arrangement) {
    case kNoSynonym:
      break;
    case kNoSynonymUnderscoreRight:
      break;
    case kNoSynonymUnderscoreBoth:
      break;
    case kOneSynonymLeft:
      break;
    case kOneSynonymLeftUnderscoreRight:
      break;
    case kOneSynonymRight:
      break;
    case kOneSynonymSelectLeft:
      break;
    case kOneSynonymSelectLeftUnderscoreRight:
      break;
    case kOneSynonymSelectRight:
      break;
    case kTwoSynonym:
      break;
    case kTwoSynonymSelectLeft:
      break;
    case kTwoSynonymSelectRight:
      break;
  }

  return results;
}

list<string> PqlEvaluator::EvaluateModifiesS(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat,
    PqlArrangementOfSynonymInSuchthatParam arrangement) {
  list<string> results;
  PKB pkb = getPKB();
  // Getting parameter of such that
  pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>>
      such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;

  cout << "Evaluating ModifiesS." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If stmt (left) modify variable (right)
      if (pkb.IsModifiedByS(left_name, right_name)) {
        return GetResultFromSelectAllQuery(select_type);
      } else {
        setClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify " << right_name
             << endl;
      }
      break;
    case kNoSynonymUnderscoreRight:
      // If no variables were modified by this stmt
      if (pkb.GetModifiedVarS(left_name).empty()) {
        setClauseFlag(false);
        cout << "Stmt " << left_name << " doesn't modify any variable " << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymLeft:
      // If no stmt of left syn entity type modifies the right variable
      if (FilterResult(pkb.GetModifyingS(right_name), left_type).empty()) {
        setClauseFlag(false);
        cout << "Stmt of left type doesnt modify right variable" << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymLeftUnderscoreRight:
      // If no stmt of left syn entity type modifies any variable
      if (FilterResult(pkb.GetAllModifyingS(), left_type).empty()) {
        setClauseFlag(false);
        cout << "Stmt of left type doesnt modify any variable" << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymRight:
      if (pkb.GetModifiedVarS(left_name).empty()) {
        setClauseFlag(false);
        cout << "Stmt " << left_name << " doesnt modify any variable" << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymSelectLeft:
      if (left_type == PqlDeclarationEntity::kStmt) {
        return pkb.GetModifyingS(right_name);
      } else {
        return FilterResult(pkb.GetModifyingS(right_name), select_type);
      }
      break;
    case kOneSynonymSelectLeftUnderscoreRight:
      if (left_type == PqlDeclarationEntity::kStmt) {
        return pkb.GetAllModifyingS();
      } else {
        return FilterResult(pkb.GetAllModifyingS(), select_type);
      }
      break;
    case kOneSynonymSelectRight:
      return pkb.GetModifiedVarS(left_name);
      break;
    case kTwoSynonym:
      // Because right param only takes in variable synonym, it is exactly the
      // same as kOneSynonymLeftUnderscoreRight
      if (FilterResult(pkb.GetAllModifyingS(), left_type).empty()) {
        setClauseFlag(false);
        cout << "Stmt of left type doesnt modify any variable" << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kTwoSynonymSelectLeft:
      // Because right param only takes in variable synonym, it is exactly the
      // same as kOneSynonymSelectLeftUnderscoreRight
      if (left_type == PqlDeclarationEntity::kStmt) {
        return pkb.GetAllModifyingS();
      } else {
        return FilterResult(pkb.GetAllModifyingS(), select_type);
      }
      break;
    case kTwoSynonymSelectRight:
      // Because right param only takes in variable synonym, just need to filter
      // left of the pair. Get all right of pair as it is select right
      cout << "Two syn select right var" << endl;
      if (left_type == PqlDeclarationEntity::kStmt) {
        return pkb.GetAllModifiedVar();
      } else {
        return GetAllRightOfPair(FilterPairResult(
            kFilterLeft, pkb.GetAllModifiesPairS(), left_type, right_type));
      }

      break;
  }

  return results;
}

list<string> PqlEvaluator::EvaluateModifiesP(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat,
    PqlArrangementOfSynonymInSuchthatParam arrangement) {
  list<string> results;
  PKB pkb = getPKB();
  // Getting parameter of such that
  pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>>
      such_that_param = suchthat.GetParameters();
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  string left_name = left_param.first;
  string right_name = right_param.first;

  cout << "Evaluating ModifiesP." << endl;

  switch (arrangement) {
    case kNoSynonym:
      // If proc (left) modify variable (right)
      if (pkb.IsModifiedByP(left_name, right_name)) {
        return GetResultFromSelectAllQuery(select_type);
      } else {
        setClauseFlag(false);
        cout << "Proc " << left_name << " doesn't modify " << right_name
             << endl;
      }
      break;
    case kNoSynonymUnderscoreRight:
      // If no variables were modified by this proc
      if (pkb.GetModifiedVarP(left_name).empty()) {
        setClauseFlag(false);
        cout << "Proc " << left_name << " doesn't modify any variable " << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymLeft:
      // If no proc modifies the right variable
      if (pkb.GetModifyingP(right_name).empty()) {
        setClauseFlag(false);
        cout << "Left proc doesnt modify right variable" << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymLeftUnderscoreRight:
      // If no proc modifies any variable
      if (pkb.GetAllModifyingP().empty()) {
        setClauseFlag(false);
        cout << "None of the proc modify any variable" << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymRight:
      if (pkb.GetModifiedVarP(left_name).empty()) {
        setClauseFlag(false);
        cout << "Proc " << left_name << " doesnt modify any variable" << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kOneSynonymSelectLeft:
        return pkb.GetModifyingP(right_name);
      break;
    case kOneSynonymSelectLeftUnderscoreRight:
        return pkb.GetAllModifyingP();
      break;
    case kOneSynonymSelectRight:
      return pkb.GetModifiedVarP(left_name);
      break;
    case kTwoSynonym:
      if ((pkb.GetAllModifyingP().empty())) {
        setClauseFlag(false);
        cout << "None of the proc modify any variable " << endl;
      } else {
        return GetResultFromSelectAllQuery(select_type);
      }
      break;
    case kTwoSynonymSelectLeft:
      return pkb.GetAllModifyingP();
      break;
    case kTwoSynonymSelectRight:
      return pkb.GetAllModifiedVar();
      break;
  }

  return results;
}

list<string> PqlEvaluator::FilterResult(list<string> unfiltered_result,
                                        PqlDeclarationEntity select_type) {
  list<string> filtered_result;
  PKB pkb = getPKB();

  for (auto& iter : unfiltered_result) {
    string result = iter;

    if (pkb.GetStmtType(iter) == select_type) {
      filtered_result.push_back(result);
    }
  }

  return filtered_result;
}

list<pair<string, string>> PqlEvaluator::FilterPairResult(
    PqlResultFilterType filter_type,
    list<pair<string, string>> unfiltered_pair_result,
    PqlDeclarationEntity left_type, PqlDeclarationEntity right_type) {
  list<pair<string, string>> filtered_result;
  PKB pkb = getPKB();

  for (auto& iter : unfiltered_pair_result) {
    string left_result = iter.first;
    string right_result = iter.second;
    switch (filter_type) {
      case kFilterLeft:
        if (pkb.GetStmtType(left_result) == left_type) {
          filtered_result.push_back(iter);
        }
        break;
      case kFilterRight:
        break;
      case kFilterBoth:
        break;
    }
  }

  return filtered_result;
}

list<string> PqlEvaluator::GetAllLeftOfPair(
    list<pair<string, string>> filtered_list) {
  list<string> results;
  for (auto& iter : filtered_list) {
    results.push_back(iter.first);
  }
  return results;
}

list<string> PqlEvaluator::GetAllRightOfPair(
    list<pair<string, string>> filtered_list) {
  list<string> results;
  for (auto& iter : filtered_list) {
    results.push_back(iter.second);
  }
  return results;
}

PqlArrangementOfSynonymInSuchthatParam
PqlEvaluator::CheckArrangementOfSynonymInSuchthatParam(
    string select_var_name,
    pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>>
        such_that_param) {
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;
  PqlDeclarationEntity left_type = left_param.second;
  PqlDeclarationEntity right_type = right_param.second;

  // (_,?)
  if (left_type == PqlDeclarationEntity::kUnderscore) {
    //(_,_)
    if (right_type == PqlDeclarationEntity::kUnderscore) {
      return kNoSynonymUnderscoreBoth;
    }
    //(_,?syn)
    else if (right_type != PqlDeclarationEntity::kInteger &&
             right_type != PqlDeclarationEntity::kIdent) {
      //(_, selectsyn)
      if (select_var_name.compare(right_param.first) == 0) {
        return kOneSynonymSelectRightUnderscoreLeft;
      }
      //(_,syn)
      else {
        return kOneSynonymRightUnderscoreLeft;
      }
    }
    //(_,int/ident)
    else {
      return kNoSynonymUnderscoreLeft;
    }
  }  // end (_,?)

  //(?syn,?)
  else if (left_type != PqlDeclarationEntity::kInteger &&
           left_type != PqlDeclarationEntity::kIdent) {
    //(selectsyn, ?)
    if (select_var_name.compare(left_param.first) == 0) {
      //(selectsyn,_)
      if (right_type == PqlDeclarationEntity::kUnderscore) {
        return kOneSynonymSelectLeftUnderscoreRight;
      }
      //(selectsyn,syn)
      else if (right_type != PqlDeclarationEntity::kInteger &&
               right_type != PqlDeclarationEntity::kIdent) {
        return kTwoSynonymSelectLeft;
      }
      //(selectsyn,int/ident)
      else {
        return kOneSynonymSelectLeft;
      }
    }
    //(syn,?)
    else {
      //(syn,_)
      if (right_type == PqlDeclarationEntity::kUnderscore) {
        return kOneSynonymLeftUnderscoreRight;
      }
      //(syn,?syn)
      else if (right_type != PqlDeclarationEntity::kInteger &&
               right_type != PqlDeclarationEntity::kIdent) {
        //(syn, selectsyn)
        if (select_var_name.compare(right_param.first) == 0) {
          return kTwoSynonymSelectRight;
        }
        //(syn,syn)
        else {
          return kTwoSynonym;
        }
      }
      //(syn,int/ident)
      else {
        return kOneSynonymLeft;
      }
    }

  }  // end (?syn,?)

  //(int/ident, ?)
  else {
    //(int/ident,_)
    if (right_type == PqlDeclarationEntity::kUnderscore) {
      return kNoSynonymUnderscoreRight;
    }
    //(int/ident,?syn)
    else if (right_type != PqlDeclarationEntity::kInteger &&
             right_type != PqlDeclarationEntity::kIdent) {
      //(int/ident, selectsyn)
      if (select_var_name.compare(right_param.first) == 0) {
        return kOneSynonymSelectRight;
      }
      //(int/ident,syn)
      else {
        return kOneSynonymRight;
      }
    }
    //(int/ident,int/ident)
    else {
      return kNoSynonym;
    }
  }  // end (int/ident, ?)
}

PqlDeclarationEntity PqlEvaluator::CheckSelectDeclarationType(
    string select_var_name) {
  unordered_map<string, PqlDeclarationEntity> declarations = getDeclarations();

  // Find out what the user is selecting by going through the list of
  // declarations made by the user
  for (unordered_map<string, PqlDeclarationEntity>::iterator it =
           declarations.begin();
       it != declarations.end(); ++it) {
    // Check for a match between the selection and declaration
    if (select_var_name.compare(it->first) == 0) {
      return it->second;
    }
  }
  // Shouldn't happen since there is validation
  return PqlDeclarationEntity::kUnderscore;
}

/*
 * Getters and Setters
 */

void PqlEvaluator::setClauseFlag(bool clauseFlag) {
  this->clauseFlag_ = clauseFlag;
}

bool PqlEvaluator::getClauseFlag() { return clauseFlag_; }

void PqlEvaluator::setDeclarations(
    unordered_map<string, PqlDeclarationEntity> declarations) {
  this->declarations_ = declarations;
}

unordered_map<string, PqlDeclarationEntity> PqlEvaluator::getDeclarations() {
  return declarations_;
}

void PqlEvaluator::setSuchthat(vector<PqlSuchthat> suchthats) {
  this->suchthats_ = suchthats;
}

vector<PqlSuchthat> PqlEvaluator::getSuchthat() { return suchthats_; }

void PqlEvaluator::setSelectVar(string select_var_name) {
  this->select_var_name_ = select_var_name;
}

string PqlEvaluator::getSelectVar() { return select_var_name_; }

void PqlEvaluator::setSelectType(PqlDeclarationEntity select_type) {
  this->select_type_ = select_type;
}

PqlDeclarationEntity PqlEvaluator::getSelectType() { return select_type_; }

void PqlEvaluator::setPKB(PKB pkb) { this->pkb_ = pkb; }

PKB PqlEvaluator::getPKB() { return pkb_; }
