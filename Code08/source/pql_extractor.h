#pragma once

#ifndef PQL_EXTRACTOR_H
#define PQL_EXTRACTOR_H

#include "affects_extractor.h"
#include "next_extractor.h"

class PqlExtractor {
  AffectsExtractor ae_;
  NextExtractor ne_;

 public:
  PqlExtractor(PKB* pkb);

  //**************** Next* *********************************

  // @returns true if Next*(previous_stmt, next_stmt) holds
  bool IsNextT(StmtNum previous_stmt, StmtNum next_stmt);

  // @returns true if Next*(_, stmt_num) holds
  bool IsNextT(StmtNum stmt_num);

  // @returns true if Next*(stmt_num, _) holds
  bool IsPreviousT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(stmt_num, n)
  StmtNumSet GetNextT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(n, stmt_num)
  StmtNumSet GetPreviousT(StmtNum stmt_num);

  // @returns a list of all pairs of <n1, n2> that satisfy Next*(n1, n2)
  StmtNumPairSet GetAllNextTPairs();

  //****************** Affects *******************************

  // @returns true if Affects(stmt_1, stmt_2) holds, else false
  bool IsAffects(StmtNum stmt_1, StmtNum stmt_2);

  // @returns true if stmt_num affects any statement
  bool IsAffects(StmtNum stmt_num);

  // @returns true if stmt_num is affected by any statement
  bool IsAffected(StmtNum stmt_num);

  // @returns a set of n that Affects(stmt_1, n) holds true
  VertexSet GetAffects(StmtNum stmt_1);

  // @returns a set of n that Affects(n, stmt_num) holds true
  VertexSet GetAffectedBy(StmtNum stmt_num);

  // @returns set of all statements stmt_num such that Affects(stmt_num, _)
  // holds true
  VertexSet GetAllAffects();

  // @returns set of all statements stmt_num such that Affects(_, stmt_num)
  // holds true
  VertexSet GetAllAffectedBy();

  // Get the AffectsMap of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affected StmtNums
  AffectsMap GetAffectsMap();

  // Get the AffectedByMap of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affecting StmtNums
  AffectsMap GetAffectedByMap();

  //**************** Affects* *********************************

  // @returns true if Affects*(stmt_1, stmt_2) holds
  bool IsAffectsT(StmtNum stmt_1, StmtNum stmt_2);

  // @returns true if Affects*(stmt, n) holds for some statement n
  bool IsAffectsT(StmtNum stmt);

  // @returns true if Affects*(n, stmt) holds for some statement n
  bool IsAffectedT(StmtNum stmt);

  // @returns a set of stmt numbers a where Affects*(stmt, a) holds
  VertexSet GetAffectsT(StmtNum stmt);

  // @returns a set of stmt numbers a where Affects*(a, stmt) holds
  VertexSet GetAffectedByT(StmtNum stmt);

  // @returns a set of stmts affecting some other statement
  VertexSet GetAllAffectsT();

  // @returns a set of stmts being affected by some other statement
  VertexSet GetAllAffectedByT();

  // @returns a hashmap of <key> stmt <value> set of all affected stmts
  AffectsMap GetAffectsTMap();

  // @returns a hashmap of <key> stmt <value> set of all affecting stmts
  AffectsMap GetAffectedByTMap();

  //****************** AffectsBip *******************************

  // @returns true if AffectsBip(stmt_1, stmt_2) holds, else false
  bool IsAffectsBip(StmtNum stmt_1, StmtNum stmt_2);

  // @returns true if there is any AffectsBip(stmt_num, _ ) relation that holds,
  // else false
  bool IsAffectsBip(StmtNum stmt_num);

  // @returns true if there is any AffectsBip(_,stmt_num ) relation that holds,
  // else false
  bool IsAffectedBip(StmtNum stmt_num);

  // @returns a set of n that AffectsBip(stmt_1, n) holds true
  VertexSet GetAffectsBip(StmtNum stmt_1);

  // @returns a set of n that Affects(n, stmt_num) holds true
  VertexSet GetAffectedByBip(StmtNum stmt_num);

  // Get the AffectsBipMap of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affectedBip StmtNums
  AffectsMap GetAffectsBipMap();

  // Get the AffectedByBipMap of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affectingBip
  // StmtNums
  AffectsMap GetAffectedByBipMap();

  //****************** AffectsBip* *******************************

  // @returns true if AffectsBip*(stmt_1, stmt_2) holds
  bool IsAffectsBipT(StmtNum stmt_1, StmtNum stmt_2);

  // @returns true if AffectsBip*(stmt, n) holds for some statement n
  bool IsAffectsBipT(StmtNum stmt);

  // @returns true if AffectsBip*(n, stmt) holds for some statement n
  bool IsAffectedBipT(StmtNum stmt);

  // @returns a set of stmt numbers a where AffectsBip*(stmt, a) holds
  VertexSet GetAffectsBipT(StmtNum stmt);

  // @returns a set of stmt numbers a where AffectsBip*(a, stmt) holds
  VertexSet GetAffectedByBipT(StmtNum stmt);

  // @returns a set of stmts affecting some other statement across procedures
  VertexSet GetAllAffectsBipT();

  // @returns a set of stmts affected by some other statement across procedures
  VertexSet GetAllAffectedByBipT();

  // @returns a hashmap of <key> StmtNum <value> set of all affectedByBipT
  // StmtNums
  AffectsMap GetAffectsBipTMap();

  // @returns a hashmap of <key> StmtNum <value> set of all affectingBipT
  // StmtNums
  AffectsMap GetAffectedByBipTMap();
};

#endif  // !PQL_EXTRACTOR_H
