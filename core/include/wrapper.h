#pragma once

#include <limits.h>
#include <unordered_map>
#include "condorcet_domain.h"
#include "utils.h"

typedef std::unordered_map<Triple, std::vector<std::string>, TripleHasher> AllowedRules;

class TRSWrapper
{
public:
    CondorcetDomain cd;
    AllowedRules allowed_rules;

    TRSWrapper(CondorcetDomain cd);
    void change_allowed_rules(const std::vector<Triple>& triples, const std::vector<std::string>& rules);
    TRS remove_rules(TRS trs, const std::vector<int>& contains);
    Triple next_unassigned_triple(const TRS& trs);
    std::vector<std::size_t> evaluate_rules_on_triple(const TRS& trs, const Triple& triple);
    Triple dynamic_triple_ordering(const TRS& trs);

    int largest_full_size(const TRS& trs);

};

