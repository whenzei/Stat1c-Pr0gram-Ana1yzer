#pragma once

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

#ifndef SPA_ASSIGN_TABLE_H
#define SPA_ASSIGN_TABLE_H

/**
 * The assignment table class for the PKB component
 * Used to store assign statements that are passed into PKB from the parser
 */
class AssignTable {
 private:
  map<int, string> assign_map_;
  vector<string> assign_vector_;

 public:
  /**
   * inserts the given assign statement into the assignment table
   * @param statement_number the statement number of the assign statement
   * @param assign_statement the assign statement to be inserted
   * @returns 0 if the statement number cannot be found in the table and the
   * assign statement is successfully inserted
   * @returns -1 if the assign statement is empty
   * @returns -1 if the statement number was already in the table
   */
  int Insert(int statement_number, string assign_statement);

  /**
   * get statement numbers for all assign statements stored inside assignment
   * table
   * @returns the vector of statement numbers(can be empty)
   */
  vector<string> List();
};

#endif  // !SPA_ASSIGN_TABLE_H
