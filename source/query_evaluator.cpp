#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_query.h"
#include "query_evaluator.h"

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::vector;

PqlDeclarationEntity select_type;

QueryEvaluator::QueryEvaluator() {}

list<string> QueryEvaluator::GetResultFromQuery(PqlQuery* query, PKB pkb) {
  string select_var_name = query->GetVarName();
  unordered_map<string, PqlDeclarationEntity> declarations = query->GetDeclarations();
  vector<PqlSuchthat> such_that_clauses = query->GetSuchThats();
  list<string> results;

  // Check if there is "such that" clause, if not then evaluator will use
  // GetResultFromSelectAllQuery methods
  if (such_that_clauses.size() == 0) {
    results = GetResultFromSelectAllQuery(select_var_name, declarations, pkb);
  }

  cout << "Result size: " << results.size() << endl;

  // Set result to none if no results were found.
  if (results.size() == 0) {
    results.push_back("none");
  }

  return results;
}

list<string> QueryEvaluator::GetResultFromSelectAllQuery(
    string select_var_name, unordered_map<string, PqlDeclarationEntity> declarations,
    PKB pkb) {
  list<string> results;

  // Find out what the user is selecting by going through the list of
  // declarations made by the user
  for (unordered_map<string, PqlDeclarationEntity>::iterator it = declarations.begin();
       it != declarations.end(); ++it) {
    // Check for a match between the selection and declaration
    if (select_var_name.compare(it->first) == 0) {
      select_type = it->second;

      switch (select_type) {
        case PqlDeclarationEntity::kProcedure:
          // Get all procedures name from PKB and store into results list
          cout << "Select all procedure." << endl;
          return pkb.GetAllProcName();
        case PqlDeclarationEntity::kVariable:
          // Get all variable name from PKB and store into results list
          cout << "Select all variables." << endl;
          return pkb.GetAllVarName();
        case PqlDeclarationEntity::kAssign:
          // Get all statement number of statement which contains assignment
          // from PKB and store into results list
          cout << "Select all assign statement." << endl;
          return pkb.GetAllAssignStmt();
        case PqlDeclarationEntity::kStmt:
          // Get all stmt number from PKB and store into results list
          cout << "Select all statement." << endl;
          return pkb.GetAllStmt();
        case PqlDeclarationEntity::kRead:
          // Get all read stmt from PKB and store into results list
          cout << "Select all read statement." << endl;
          return pkb.GetAllReadStmt();
        case PqlDeclarationEntity::kPrint:
          // Get all print stmt from PKB and store into results list
          cout << "Select all print statement." << endl;
          return pkb.GetAllPrintStmt();
        case PqlDeclarationEntity::kCall:
          // Get all call stmt from PKB and store into results list
          break;
        case PqlDeclarationEntity::kWhile:
          // Get all while stmt from PKB and store into results list
          cout << "Select all while statement." << endl;
          return pkb.GetAllWhileStmt();
        case PqlDeclarationEntity::kIf:
          // Get all if stmt from PKB and store into results list
          cout << "Select all if statement." << endl;
          return pkb.GetAllIfStmt();
        case PqlDeclarationEntity::kConstant:
          // Get all constant from PKB and store into results list
          cout << "Select all constants." << endl;
          return pkb.GetAllConstValue();
        case PqlDeclarationEntity::kProgline:
          // Get all program line from PKB and store into results list
          cout << "Select all program lines." << endl;
          return pkb.GetAllStmt();
      }
      break;
    }
  }

  return results;
}

string QueryEvaluator::FormatResultString(list<string> results) {
  string formatted_string;
  string index_flag = "";

  if (results.empty()) {
    // If no result, output none
    formatted_string = "none";
  } else {
    // Check the type of the result
    switch (select_type) {
      case PqlDeclarationEntity::kProcedure:
        // Format for procedure
        formatted_string = "procedure ";
        break;
      case PqlDeclarationEntity::kVariable:
        // Format for variable
        formatted_string = "variable ";
        break;
      case PqlDeclarationEntity::kAssign:
        index_flag = "#";
        formatted_string = "statement ";
        break;
    }

    // Loop through the results and format it into a single string
    for (auto& iter : results) {
      // Insert the results into string, uses # if result is a number
      formatted_string += index_flag + iter;

      if (iter != results.back()) {
        formatted_string += ", ";
      }
    }
  }

  return formatted_string;
}