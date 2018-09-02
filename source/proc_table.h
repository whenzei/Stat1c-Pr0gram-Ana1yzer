#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

#ifndef SPA_PROC_TABLE_H
#define SPA_PROC_TABLE_H

/**
 * The procedure table class for the PKB component
 * Used to store procedure names that are passed into PKB from the parser
 */
class ProcTable {
 private:
  vector<string> proc_vector_;

  /**
   * checks whether the given procedure name is already in the procedure table
   * @param proc_name the procedure name to be checked
   * @returns true if proc_name can be found in the procedure table, false otherwise
   */
  bool Find(string proc_name);

 public:
  /**
   * inserts the given procedure name into the procedure table
   * @param proc_name the procedure name to be inserted
   * @returns 0 if the procedure name cannot be found in the table and is successfully inserted 
   *         -1 if the procedure name is empty 
   *         -1 if the procedure name was already inside the table
   */
  int Insert(string proc_name);

  /**
   * get all procedure names stored inside procedure table
   * @returns the vector of procedure names (can be empty)
   */
  vector<string> List();
};

#endif  // !SPA_PROC_TABLE_H
