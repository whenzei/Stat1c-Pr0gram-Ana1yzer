#include <list>
#include <string>
#include <vector>

#include "pkb.h"
#include "pql_declaration.h"
#include "pql_query.h"
#include "query_evaluator.h"

using std::list;
using std::string;
using std::vector;

DeclarationKeyword select_type;

QueryEvaluator::QueryEvaluator() {}

list<string> QueryEvaluator::GetResultFromQuery(Query query, PKB pkb) {
  string select_var_name = query.GetVarName();
  vector<Declaration> declarations = query.GetDeclarations();
  list<string> results;

  // Find out what the user is selecting by going through the list of
  // declarations made by the user
  for (auto &iter : declarations) {
    // Check if there is a match between the selection and declaration
    if (select_var_name.compare(iter.GetVarName()) == 0) {
      select_type = iter.GetKeyword();

      switch (select_type) {
        case DeclarationKeyword::kProcedure:
          // Get all procedures name from PKB and store into results list
          break;
        case DeclarationKeyword::kVariable:
          // Get all variable name from PKB and store into results list
          break;
        case DeclarationKeyword::kAssign:
          // Get all statement number of statement which contains assignment
          // from PKB and store into results list
          list<string> result_list = pkb.GetAllAssignStmt();
          std::copy(result_list.begin(), result_list.end(),
                    std::back_inserter(results));
          break;
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
      case DeclarationKeyword::kProcedure:
        // Format for procedure
        formatted_string = "procedure ";
        break;
      case DeclarationKeyword::kVariable:
        // Format for variable
        formatted_string = "variable ";
        break;
      case DeclarationKeyword::kAssign:
        index_flag = "#";
        formatted_string = "statement ";
        break;
    }

    // Loop through the results and format it into a single string
    for (auto &iter : results) {
      // Insert the results into string, uses # if result is a number
      formatted_string += index_flag + iter;

      if (iter != results.back()) {
        formatted_string += ", ";
      }
    }
  }

  return formatted_string;
}