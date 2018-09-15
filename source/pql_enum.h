#pragma once

#ifndef PQL_ENUM_H
#define PQL_ENUM_H

enum PqlDeclarationEntity {
  kStmt = 0,
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssign,
  kVariable,
  kConstant,
  kProgline,
  kProcedure,
  kInteger, // only used for validation, not an actual entity type
  kIdent, // only used for validation, not an actual entity type
  kUnderscore // only used for validation, not an actual entity type
};

enum PqlSuchthatType {
  kModifiesP = 0,
  kModifiesS,
  kUsesP,
  kUsesS,
  kCalls,
  kCallsT,
  kParent,
  kParentT,
  kFollows,
  kFollowsT,
  kNext,
  kNextT,
  kAffects,
  kAffectsT
};

#endif