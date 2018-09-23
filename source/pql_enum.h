#pragma once

#ifndef PQL_ENUM_H
#define PQL_ENUM_H

/* The entity types for a PQL declaration */
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
  kInteger,     // only used for validation, not an actual entity type
  kIdent,       // only used for validation, not an actual entity type
  kUnderscore,  // only used for validation, not an actual entity type
  kSynonym,     // only used for validation, not an actual entity type
  kNone,
  kAll  // used for PKB to hold all entity types
};

/* The relationship types for a such that clause */
enum class PqlSuchthatType {
  kModifiesP = 0,
  kModifiesS,
  kModifies,  // only used for parsing
  kUsesP,
  kUsesS,
  kUses,  // only used for parsing
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
  kNone  // only used for parsing
};

/* The type for pattern clause */
enum class PqlPatternType { kAssign, kWhile, kIf };

/* The type for the 2nd parameter of an assign pattern clause */
enum class PqlPatternExpressionType {
  kUnderscore,
  kUnderscoreExpressionUnderscore,
  kExpression
};

/* The arrangement types for a such that clause */
enum SuchthatParamType {
  kNoSynonym = 0,                        // e.g (1, 2)
  kNoSynonymUnderscoreLeft,              // e.g (_, 2)
  kNoSynonymUnderscoreRight,             // e.g (1, _)
  kNoSynonymUnderscoreBoth,              // e.g (_, _)
  kOneSynonymLeft,                       // e.g (a, 2)
  kOneSynonymLeftUnderscoreRight,        // e.g (a, _)
  kOneSynonymRight,                      // e.g (1, a)
  kOneSynonymRightUnderscoreLeft,        // e.g (_, a)
  kTwoSynonym,                           // e.g (a, b)
};

/* The filter type for the result */
enum PqlResultFilterType { kFilterLeft = 0, kFilterRight, kFilterBoth };

/* The conflict type for result table */
enum PqlResultTableConflict {
  kNoConflict = 0,
  kConflict,
  kOneConflictLeft,   // One conflict at the left of input pair
  kOneConflictRight,  // One conflict at the right of input pair
  kTwoConflict        // Two conflict for the input pair
};

#endif