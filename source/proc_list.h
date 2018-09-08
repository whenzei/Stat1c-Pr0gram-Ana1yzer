#pragma once

#ifndef SPA_PROC_LIST_H
#define SPA_PROC_LIST_H

#include <string>
#include <list>

using std::string;
using std::list;

using ProcName = string;
using ProcNameList = list<string>;

// The procedure list class for the PKB component
// Used to store procedure names that are passed into PKB from the parser
class ProcList {
  
  ProcNameList proc_name_list_;

 public:
  
  // inserts the given procedure name into the procedure list
  // @param proc_name the procedure name to be inserted
  // @returns true if the procedure name cannot be found in the list and is successfully inserted 
  //          false if the procedure name is empty 
  //          false if the procedure name was already inside the list
  bool InsertProcName(ProcName proc_name);

  // get all procedure names stored inside procedure list
  // @returns the list of procedure names (can be empty)
  ProcNameList GetProcNameList();

 private: 
  // checks whether the given procedure name is already in the procedure table
  // @param proc_name the procedure name to be checked
  // @returns true if proc_name can be found in the procedure table, false otherwise
  bool FindProcName(ProcName proc_name);
};

#endif  // !SPA_PROC_LIST_H
