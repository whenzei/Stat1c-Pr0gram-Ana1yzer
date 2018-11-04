#pragma once

#ifndef PQL_GLOBAL_H
#define PQL_GLOBAL_H

#include <string>
using std::pair;
using std::string;

/* The entity types for a PQL declaration */
enum class PqlDeclarationEntity {
  kStmt = 0,
  kRead,
  kReadName,
  kPrint,
  kPrintName,
  kCall,
  kCallName,
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

/* The type of clause */
enum class PqlClauseType { kSuchthat, kPattern, kWith };

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
  kAffectsB,
  kAffectsBT,
  kDominates,
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
  kNoSynonym = 0,                  // e.g (1, 2)
  kNoSynonymUnderscoreLeft,        // e.g (_, 2)
  kNoSynonymUnderscoreRight,       // e.g (1, _)
  kNoSynonymUnderscoreBoth,        // e.g (_, _)
  kOneSynonymLeft,                 // e.g (a, 2)
  kOneSynonymLeftUnderscoreRight,  // e.g (a, _)
  kOneSynonymRight,                // e.g (1, a)
  kOneSynonymRightUnderscoreLeft,  // e.g (_, a)
  kTwoSynonym,                     // e.g (a, b)
};

/* The arrangement types for a with clause */
enum WithParamType {
  kWithNoSynonym = 0,    // e.g with 1 = 2
  kWithOneSynonymLeft,   // e.g with a.stmt# = 1
  kWithOneSynonymRight,  // e.g with 2 = a.stmt#
  kWithTwoSynonym,       // e.g a.stmt# = b.stmt#
};

/* The filter type for the result */
enum PqlResultFilterType { kFilterLeft = 0, kFilterRight, kFilterBoth };

/* The conflict type for result table */
enum PqlResultTableConflict {
  kNoConflict = 0,    // No conflict for single/pair list
  kConflict,          // One conflict for single list
  kOneConflictLeft,   // One conflict at the left of pair
  kOneConflictRight,  // One conflict at the right of pair
  kTwoConflict        // Two conflict for the pair
};

/* Custom shorthand structures */
using Synonym = pair<string, PqlDeclarationEntity>;
using Parameters = pair<Synonym, Synonym>;

#endif