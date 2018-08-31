#include <string>
#include <vector>

#include "declaration.h"
#include "query.h"
#include "query_evaluator.h"

QueryEvaluator::QueryEvaluator() {}

std::string QueryEvaluator::GetResultFromQuery(Query query) {
  std::string select_var_name = query.GetVarName();
  std::vector<Declaration> declarations = query.GetDeclarations();
  std::string result = "none";

  // Find out what the user is selecting by going through the list of
  // declarations made by the user
  for (auto &iter : declarations) {
    // If there is a match between the selection and declaration
    if (select_var_name.compare(iter.GetVarName()) == 0) {
      DeclarationKeyword select_type = iter.GetKeyword();

      switch (select_type) {
        case DeclarationKeyword::kProcedure:
          // Get all procedures name from PKB and store into result string
          break;
        case DeclarationKeyword::kVariable:
          // Get all variable name from PKB and store into result string
          break;
        case DeclarationKeyword::kAssign:
          // Get all statement number of statement which contains assignment
          // from PKB and store into result string
          break;
      }
      break;
    }
  }
  return result;
}