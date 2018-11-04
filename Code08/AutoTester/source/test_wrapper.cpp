#include "test_wrapper.h"
#include "parser.h"
#include "pql_parser.h"
#include "pql_query.h"
#include "pql_evaluator.h"

#include <iostream>

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() { 
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program

  exception_caught_ = false;
  // PKB initialization
  pkb_ = PKB();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // attach pkb to parser
  Parser parser(&pkb_);

  try {
    parser.Parse(filename);
  } catch (const std::exception& e) {
    exception_caught_ = true;
    std::cerr << e.what() << std::endl;
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...
  // early return if exception caught in parsing
  if (exception_caught_) {
    return;
  }
  PqlQuery* pql_query = new PqlQuery();
  PqlParser pql_parser(query, pql_query);
  if (pql_parser.Parse()) {
    pql_query->Optimize();
    PqlEvaluator qe;
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    results = qe.GetResultFromQuery(pql_query, pkb_);
  }

  delete pql_query;
}
