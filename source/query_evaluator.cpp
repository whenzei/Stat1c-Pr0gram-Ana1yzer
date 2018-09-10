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
  map<string, PqlDeclarationEntity> declarations = query->GetDeclarations();

  return GetResultFromSelectAllQuery(select_var_name, declarations, pkb);
}

list<string> QueryEvaluator::GetResultFromSelectAllQuery(
    string select_var_name, map<string, PqlDeclarationEntity> declarations,
    PKB pkb) {
  list<string> results;

  // Find out what the user is selecting by going through the list of
  // declarations made by the user
  for (map<string, PqlDeclarationEntity>::iterator it = declarations.begin();
       it != declarations.end(); ++it) {
    // Check for a match between the selection and declaration
    if (select_var_name.compare(it->first) == 0) {
      select_type = it->second;
      list<string> result_list;

      switch (select_type) {
        case PqlDeclarationEntity::kProcedure:
          // Get all procedures name from PKB and store into results list
          result_list = pkb.GetAllProcName();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all procedure." << endl;
          break;
        case PqlDeclarationEntity::kVariable:
          // Get all variable name from PKB and store into results list
          result_list = pkb.GetAllVarName();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all variables." << endl;
          break;
        case PqlDeclarationEntity::kAssign:
          // Get all statement number of statement which contains assignment
          // from PKB and store into results list
          result_list = pkb.GetAllAssignStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all assign statement." << endl;
          break;
        case PqlDeclarationEntity::kStmt:
          // Get all stmt number from PKB and store into results list
          result_list = pkb.GetAllStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all statement." << endl;
          break;
        case PqlDeclarationEntity::kRead:
          // Get all read stmt from PKB and store into results list
          result_list = pkb.GetAllReadStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all read statement." << endl;
          break;
        case PqlDeclarationEntity::kPrint:
          // Get all print stmt from PKB and store into results list
          result_list = pkb.GetAllPrintStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all print statement." << endl;
          break;
        case PqlDeclarationEntity::kCall:
          // Get all call stmt from PKB and store into results list
          break;
        case PqlDeclarationEntity::kWhile:
          // Get all while stmt from PKB and store into results list
          result_list = pkb.GetAllWhileStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all while statement." << endl;
          break;
        case PqlDeclarationEntity::kIf:
          // Get all if stmt from PKB and store into results list
          result_list = pkb.GetAllIfStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all if statement." << endl;
          break;
        case PqlDeclarationEntity::kConstant:
          // Get all constant from PKB and store into results list
          result_list = pkb.GetAllConstValue();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all constants." << endl;
          break;
        case PqlDeclarationEntity::kProgline:
          // Get all program line from PKB and store into results list
          result_list = pkb.GetAllStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          cout << "Select all program lines." << endl;
          break;
      }
      cout << "Result size: " << result_list.size() << endl;

	  //Set result to none if no results were found.
      if (result_list.size() == 0) {
        results.push_back("none");
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