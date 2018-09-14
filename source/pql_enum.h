#pragma once

#ifndef PQL_ENUM_H
#define PQL_ENUM_H

enum class PqlDeclarationEntity {
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
  kUnderscore, // only used for validation, not an actual entity type
  kSynonym // only used for validation, not an actual entity type
};

enum class PqlSuchthatType {
  kModifiesP = 0,
  kModifiesS,
  kModifies, // only used for parsing
  kUsesP,
  kUsesS,
  kUses, // only used for parsing
  kCalls,
  kCallsT,
  kParent,
  kParentT,
  kFollows,
  kFollowsT,
  kNext,
  kNextT,
  kAffects,
  kAffectsT,
  kNone
};

#endif