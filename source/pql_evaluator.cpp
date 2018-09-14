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
    results = GetResultFromSelectAllQuery();
  }
  // Else use GetResultFromQueryWithSuchThat method
  else {
    results = GetResultFromQueryWithSuchThat();
  }

  cout << "Result size: " << results.size() << endl;
  // Set result to none if no results were found.
  if (results.size() == 0) {
    results.push_back("none");
  }

  return results;
}

list<string> PqlEvaluator::GetResultFromQueryWithSuchThat() {
  PqlDeclarationEntity select_type = getSelectType();
  string select_var_name = getSelectVar();
  PqlSuchthat suchthat = getSuchthat().front();
  list<string> results;

  PqlArrangementOfSynonymInSuchthatParam arrangement =
      CheckArrangementOfSynonymInSuchthatParam(select_var_name,
                                               suchthat.GetParameters());

  switch (suchthat.GetType()) {
    case kFollows:
      return EvaluateFollows(select_type, suchthat, arrangement);
    case kFollowsT:
      return EvaluateFollowsT(select_type, suchthat, arrangement);
    case kParent:
      return EvaluateParent(select_type, suchthat, arrangement);
    case kParentT:
      return EvaluateParentT(select_type, suchthat, arrangement);
    case kUsesS:
      return EvaluateUsesS(select_type, suchthat, arrangement);
    case kModifiesS:
      return EvaluateModifiesS(select_type, suchthat, arrangement);
  }

  return results;
}

list<string> PqlEvaluator::GetResultFromSelectAllQuery() {
  PKB pkb = getPKB();
  PqlDeclarationEntity select_type = getSelectType();
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
  if (left_type == kUnderscore) {
    //(_,_)
    if (right_type == kUnderscore) {
      return kNoSynonymUnderscoreBoth;
    }
    //(_,?syn)
    else if (right_type != kInteger && right_type != kIdent) {
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
  else if (left_type != kInteger && left_type != kIdent) {
    //(selectsyn, ?)
    if (select_var_name.compare(left_param.first) == 0) {
      //(selectsyn,_)
      if (right_type == kUnderscore) {
        return kOneSynonymSelectLeftUnderscoreRight;
      }
      //(selectsyn,syn)
      else if (right_type != kInteger && right_type != kIdent) {
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
      if (right_type == kUnderscore) {
        return kOneSynonymLeftUnderscoreRight;
      }
      //(syn,?syn)
      else if (right_type != kInteger && right_type != kIdent) {
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
    if (right_type == kUnderscore) {
      return kNoSynonymUnderscoreRight;
    }
    //(int/ident,?syn)
    else if (right_type != kInteger && right_type != kIdent) {
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
  return kUnderscore;
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
