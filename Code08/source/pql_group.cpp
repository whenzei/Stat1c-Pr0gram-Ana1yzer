#include "pql_group.h"

#include <algorithm>
#include <climits>
using std::sort;

/* Helper function to compare 2 clauses based on priority. */
bool CompareClauses(PqlClause* a, PqlClause* b) {
  return a->GetPriority() > b->GetPriority();
}

PqlGroup::PqlGroup() { 
  priority_ = 0;
  uses_selection_ = false;
}

void PqlGroup::SortClauses() {
  int max_priority = 0;
  int index;
  for(int i = 0; i < clauses_.size(); i++) {
    // Get clause with highest priority
    if (clauses_[i]->GetPriority() > max_priority) {
      max_priority = clauses_[i]->GetPriority();
      index = i;
    }

    // Set up synonym map
    string first = clauses_[i]->GetSynonyms().first;
    string second = clauses_[i]->GetSynonyms().second;
    if (first != "") {
      
    }
    if (second != "") {
    }
  }

  vector<PqlClause*> clauses;
  clauses.push_back(clauses_[index]);

  sort(clauses_.begin(), clauses_.end(), CompareClauses);
}

void PqlGroup::AddClause(PqlClause* clause) {
  clauses_.push_back(clause);
  priority_ += clause->GetPriority();
}

vector<PqlClause*> PqlGroup::GetClauses() { return clauses_; }

int PqlGroup::GetPriority() { return priority_; }

void PqlGroup::SetPriority(int pri) { priority_ = pri; }

void PqlGroup::UsesSelectionTrue() { uses_selection_ = true; }

bool PqlGroup::GetUsesSelection() { return uses_selection_; }