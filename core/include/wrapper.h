#pragma once

#include <limits.h>
#include <unordered_map>
#include "condorcet_domain.h"

struct TripletHasher
{
    int operator()(const Triplet &triplet) const {
        int hash = 0;
        for(const int &t : triplet) {
            hash ^= t + 0x9e3779b9;
        }
        return hash;
    }
};

typedef std::unordered_map<Triplet, std::vector<std::string>, TripletHasher> AllowedRules;

class TRSWrapper
{
public:
    CondorcetDomain cd;
    AllowedRules allowed_rules;

    TRSWrapper(CondorcetDomain cd);
    void change_allowed_rules(const std::vector<Triplet>& triplets, const std::vector<std::string>& rules);
    TRS remove_rules(TRS trs, const std::vector<int>& contains);
    Triplet next_unassigned_triplet(const TRS& trs);
    std::vector<std::size_t> evaluate_rules_on_triplet(const TRS& trs, const Triplet& triplet);
    Triplet dynamic_triplet_ordering(const TRS& trs);

    int largest_full_size(const TRS& trs);
};

