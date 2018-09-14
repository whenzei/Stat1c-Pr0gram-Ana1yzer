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
  kInteger,    // only used for validation, not an actual entity type
  kIdent,      // only used for validation, not an actual entity type
  kUnderscore  // only used for validation, not an actual entity type
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

enum PqlArrangementOfSynonymInSuchthatParam {
  kNoSynonym = 0,  // e.g (1, 2)
  kNoSynonymUnderscoreLeft, // e.g (_, 2)
  kNoSynonymUnderscoreRight, // e.g (1, _)
  kNoSynonymUnderscoreBoth, // e.g (_, _)
  kOneSynonymLeft, // e.g (a, 2)
  kOneSynonymLeftUnderscoreRight, // e.g (a, _)
  kOneSynonymRight, // e.g (1, a)
  kOneSynonymRightUnderscoreLeft, // e.g (_, a)
  kOneSynonymSelectLeft, // e.g (s, 2)
  kOneSynonymSelectLeftUnderscoreRight, // e.g (s, _)
  kOneSynonymSelectRight, // e.g (1, s)
  kOneSynonymSelectRightUnderscoreLeft, // e.g (_, s)
  kTwoSynonym, // e.g (a, b)
  kTwoSynonymSelectLeft, // e.g (s, a)
  kTwoSynonymSelectRight // e.g (a, s)
};

#endif