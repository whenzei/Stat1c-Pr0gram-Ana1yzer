#include "gui_wrapper.h"
#include "../source/parser.h"
#include "../source/pql_parser.h"
#include "../source/pql_query.h"
#include "../source/pql_evaluator.h"

#include <iostream>

// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  pkb_ = PKB();
  parse_success_ = false;
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  Parser parser_(&pkb_);
  parse_success_ = parser_.Parse(filename);
  std::cout << "parsed " << filename << std::endl;
  // ...rest of your code...
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...
  if (!parse_success_) {
    results.push_back("Invalid SIMPLE syntax detected, no queries can be performed.");
    return;
  }

  PqlQuery* pql_query = new PqlQuery();
  PqlParser pql_parser(query, pql_query);
  if (pql_parser.Parse()) {
    PqlEvaluator qe;
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    results = qe.GetResultFromQuery(pql_query, pkb_);

	// Set result to none if no results were found.
    if (results.empty()) {
      results.push_back("none");
    }
  }
  else {
    std::list<std::string> error;
    error.push_back(pql_parser.GetErrorMessage());
    results = error;
  }
}