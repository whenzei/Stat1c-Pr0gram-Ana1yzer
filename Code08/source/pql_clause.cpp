#include "pql_clause.h"

int PqlClause::GetPriority() { return priority_; }

void PqlClause::SetPriority(int priority) { priority_ = priority; }

string PqlClause::GetSynonym() { return synonym_; }

void PqlClause::SetSynonym(string syn) {
  synonym_= syn;
}

bool PqlClause::Compare(PqlClause* a, PqlClause* b) {
  return a->GetPriority() > b->GetPriority();
}