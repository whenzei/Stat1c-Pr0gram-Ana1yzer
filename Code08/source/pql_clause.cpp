#include "pql_clause.h"

int PqlClause::GetPriority() { return priority_; }

void PqlClause::SetPriority(int priority) { priority_ = priority; }

pair<string, string> PqlClause::GetSynonyms() { return synonyms_; }

void PqlClause::SetSynonyms(string syn1, string syn2) {
  synonyms_ = std::make_pair(syn1, syn2);
}