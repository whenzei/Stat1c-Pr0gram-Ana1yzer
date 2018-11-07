#include "pql_group.h"

#include <algorithm>
#include <climits>
using std::sort;

PqlGroup::PqlGroup() { 
  priority_ = 0;
  uses_selection_ = false;
}

void PqlGroup::SortClauses() {
  int max_priority = 0;
  int max_index;
  unordered_set<int> available_set;
  for(unsigned i = 0; i <  clauses_.size(); i++) {
    // Get clause with highest priority
    if (clauses_[i]->GetPriority() > max_priority) {
      max_priority = clauses_[i]->GetPriority();
      max_index = i;
    }

    // Set up synonym map
    string first = clauses_[i]->GetSynonyms().first;
    string second = clauses_[i]->GetSynonyms().second;
    if (first != "") {
      if (synonym_map_.find(first) != synonym_map_.end()) {
        synonym_map_.at(first).insert(i);
      }
      else {
        unordered_set<int> set;
        set.insert(i);
        synonym_map_.insert(std::make_pair(first, set));
      }
    }
    if (second != "") {
      if (synonym_map_.find(second) != synonym_map_.end()) {
        synonym_map_.at(second).insert(i);
      }
      else {
        unordered_set<int> set;
        set.insert(i);
        synonym_map_.insert(std::make_pair(second, set));
      }
    }

    // Add to leftovers
    available_set.insert(i);
  }

  // Create new vector for sorted clauses
  vector<PqlClause*> sorted_clauses;
  sorted_clauses.push_back(clauses_[max_index]);

  // Remove from leftover
  available_set.erase(max_index);

  // Set up the set for all possible neighbours to go next
  unordered_set<int> neighbour_set;
  string first = clauses_[max_index]->GetSynonyms().first;
  string second = clauses_[max_index]->GetSynonyms().second;
  if (first != "") {
    unordered_set<int> set = synonym_map_.at(first);
    neighbour_set.insert(set.begin(), set.end());
  }
  if (second != "") {
    unordered_set<int> set = synonym_map_.at(second);
    neighbour_set.insert(set.begin(), set.end());
  }

  // Keep finding next neighbour until none left
  while (available_set.size() != 0) {
    // Find highest priority of all neighbours that is still available
    max_priority = 0;
    max_index = -1;
    for(unordered_set<int>::iterator it = neighbour_set.begin(); it != neighbour_set.end(); it++) {
      if (available_set.find(*it) != available_set.end()) { // still available
        if (clauses_[*it]->GetPriority() > max_priority) {
          max_priority = clauses_[*it]->GetPriority();
          max_index = *it;
        }
      }
    }

    // If there are no more neighbours left
    if (max_index == -1) {
      for (unordered_set<int>::iterator it = available_set.begin(); it != available_set.end(); it++) {
        sorted_clauses.push_back(clauses_[*it]); // Add remaining clauses to sorted
      }
      break;
    }

    // Add highest priority clause to vector
    sorted_clauses.push_back(clauses_[max_index]);
    available_set.erase(max_index); // remove from available set

    // Add more neighbours
    string first = clauses_[max_index]->GetSynonyms().first;
    string second = clauses_[max_index]->GetSynonyms().second;
    if (first != "") {
      unordered_set<int> set = synonym_map_.at(first);
      neighbour_set.insert(set.begin(), set.end());
    }
    if (second != "") {
      unordered_set<int> set = synonym_map_.at(second);
      neighbour_set.insert(set.begin(), set.end());
    }
  }

  clauses_ = sorted_clauses;
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