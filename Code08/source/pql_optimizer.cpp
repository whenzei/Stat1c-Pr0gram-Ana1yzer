#include "pql_optimizer.h"
#include "pql_validator.h"
#include <algorithm>
using std::sort;

/* Helper function to compare 2 groups based on priority. */
bool CompareGroups(PqlGroup a, PqlGroup b) {
  return a.GetPriority() > b.GetPriority();
}

PqlOptimizer::PqlOptimizer() {}

vector<PqlGroup> PqlOptimizer::Optimize() {
  vector<PqlGroup> groups;
  unordered_map<int, PqlGroup> group_map;
  for (int i = 0; i < clauses_.size(); i++) {
    if (clauses_[i]->GetSynonyms().first != "") { // if there is a synonym
      int root = union_.at(clauses_[i]->GetSynonyms().first);
      while (find_.at(root) != -1) root = find_.at(root);
      if (group_map.find(root) == group_map.end()) { // if group don't exist
        group_map.insert({ root, PqlGroup() });
        group_map.at(root).AddClause(clauses_[i]);
        if (ClauseUsesSelection(clauses_[i])) group_map.at(root).UsesSelectionTrue();
      }
      else {
        group_map.at(root).AddClause(clauses_[i]);
        if (ClauseUsesSelection(clauses_[i])) group_map.at(root).UsesSelectionTrue();
      }
    }
    else { // if there are no synonyms
      if (group_map.find(-1) == group_map.end()) { // if group don't exist
        group_map.insert({ -1, PqlGroup() });
        group_map.at(-1).AddClause(clauses_[i]);
        if (ClauseUsesSelection(clauses_[i])) group_map.at(-1).UsesSelectionTrue();
      }
      else {
        group_map.at(-1).AddClause(clauses_[i]);
        if (ClauseUsesSelection(clauses_[i])) group_map.at(-1).UsesSelectionTrue();
      }
    }
  }

  for (unordered_map<int, PqlGroup>::iterator it = group_map.begin(); it != group_map.end(); it++) {
    PqlGroup group = it->second;
    group.SortClauses(); // sort clauses within a group
    if (it->first == -1) group.SetPriority(INT_MAX); // set groups with no synonyms to max priority
    else if (!group.GetUsesSelection()) group.SetPriority(INT_MAX - 1); // set groups that do not use selected synonyms as 2nd top priority
    groups.push_back(group);
  }

  sort(groups.begin(), groups.end(), CompareGroups); // sort the groups

  return groups;
}

void PqlOptimizer::AddUnion(PqlClause* clause, string first, PqlDeclarationEntity first_type, string second, PqlDeclarationEntity second_type) {
  clauses_.push_back(clause);
  // CASE 1: Both are synoynms
  if (PqlValidator::IsSynonym(first_type) && PqlValidator::IsSynonym(second_type)) {
    clause->SetSynonyms(first, second);
    // Find if synonyms have existing groupings
    int first_group = -1;
    int second_group = -1;
    if (union_.find(first) != union_.end()) {
      first_group = union_.at(first);
    }
    if (union_.find(second) != union_.end()) {
      second_group = union_.at(second);
    }

    // CASE 1a: Both no groups
    if (first_group == -1 && second_group == -1) {
      int group = find_.size();
      union_.insert({ first, group });
      union_.insert({ second, group });
      find_.insert({ group, -1 });
    }
    // CASE 1b: Both have groups
    else if (first_group != -1 && second_group != -1) {
      // Check if both are in the same group
      if (first_group != second_group) {
        // CASE 1c.1: Both have no group ref
        if (find_.at(first_group) == -1 && find_.at(second_group) == -1) {
          find_[second_group] = first_group;
        }
        // CASE 1c.3: Both have group ref
        else if (find_.at(first_group) != -1 && find_.at(second_group) != -1) {
          int root = second_group;
          while (find_.at(root) != -1) root = find_.at(root);
          find_[root] = first_group;
        }
        // CASE 1c.2: Only 1 have group ref
        else {
          if (find_.at(first_group) != -1) find_[second_group] = first_group;
          else find_[first_group] = second_group;
        }
      }
    }
    // CASE 1c: 1 of them has group
    else {
      if (first_group != -1) union_.insert({ second, first_group });
      else union_.insert({ first, second_group });
    }
  }
  // CASE 2: Only 1 synonym
  else if (PqlValidator::IsSynonym(first_type) || PqlValidator::IsSynonym(second_type)) {
    string synonym;
    if (PqlValidator::IsSynonym(first_type)) synonym = first;
    else synonym = second;
    clause->SetSynonyms(synonym, "");

    // CASE 2a: No group yet
    if (union_.find(synonym) == union_.end()) {
      int group = find_.size();
      union_.insert({ synonym, group });
      find_.insert({ group, -1 });
    }
    // CASE 2b: Group exist, just ignore
  }
  // CASE 3: No synonyms, just ignore 
}

bool PqlOptimizer::ClauseUsesSelection(PqlClause* clause) {
  return (selections_.find(clause->GetSynonyms().first) != selections_.end() || selections_.find(clause->GetSynonyms().first) != selections_.end());
}

void PqlOptimizer::AddSelection(string selection) {
  selections_.insert(selection);
}