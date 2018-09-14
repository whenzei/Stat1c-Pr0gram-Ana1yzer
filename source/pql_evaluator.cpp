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
  string select_var_name = query->GetVarName();
  unordered_map<string, PqlDeclarationEntity> declarations =
      query->GetDeclarations();
  vector<PqlSuchthat> such_that_clauses = query->GetSuchThats();
  list<string> results;
  // The select declaration type
  PqlDeclarationEntity select_type;

  // Determine the declaration type of the select variable
  select_type = CheckSelectDeclarationType(select_var_name, declarations);

  // If there is no such that clause, then evaluator will use
  // GetResultFromSelectAllQuery method
  if (such_that_clauses.size() == 0) {
    results = GetResultFromSelectAllQuery(select_type, pkb);
  }
  // Else use GetResultFromQueryWithSuchThat method
  else {
    // for (auto& iter : such_that_clauses) { //for next iteration
    results = GetResultFromQueryWithSuchThat(select_type,
                                             such_that_clauses.front(), pkb);
    //}
  }

  cout << "Result size: " << results.size() << endl;
  // Set result to none if no results were found.
  if (results.size() == 0) {
    results.push_back("none");
  }

  return results;
}

list<string> PqlEvaluator::GetResultFromQueryWithSuchThat(
    PqlDeclarationEntity select_type, PqlSuchthat suchthat, PKB pkb) {
  list<string> results;

  // Check for the such that types (modifies, uses, follows, parent)
  switch (suchthat.GetType()) {
    case PqlSuchthatType::kFollows:
      break;
  }

  return results;
}

list<string> PqlEvaluator::GetResultFromSelectAllQuery(
    PqlDeclarationEntity select_type, PKB pkb) {
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

PqlArrangementOfSynonymInSuchthatParam
PqlEvaluator::CheckArrangementOfSynonymInSuchthatParam(
    pair<pair<string, PqlDeclarationEntity>, pair<string, PqlDeclarationEntity>>
        such_that_param) {
  pair<string, PqlDeclarationEntity> left_param = such_that_param.first;
  pair<string, PqlDeclarationEntity> right_param = such_that_param.second;

  return PqlArrangementOfSynonymInSuchthatParam::kNoSynonym;

}

PqlDeclarationEntity PqlEvaluator::CheckSelectDeclarationType(
    string select_var_name,
    unordered_map<string, PqlDeclarationEntity> declarations) {
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
}
