#include "gui_wrapper.h"
#include "../source/parser.h"
#include "../source/pql_parser.h"
#include "../source/pql_query.h"
#include "../source/query_evaluator.h"

#include <iostream>

// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  pkb_ = PKB();
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  Parser parser_(&pkb_);
  parser_.Parse(filename);
  std::cout << "parsed " << filename << std::endl;
  // ...rest of your code...
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...
  PqlQuery* pql_query = new PqlQuery();
  PqlParser::Parse(query, pql_query);
  QueryEvaluator qe;
  std::cout << "query=  " << query << std::endl;

  //results.push_back(query + "\nQuery result: ...");
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  results = qe.GetResultFromQuery(pql_query, pkb_);
}
