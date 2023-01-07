#pragma once

#include "condorcet_domain.h"


class TRSWrapper
{
private:
    std::vector<std::size_t> evaluate_rules_on_triplet(const TRS& trs, Triplet triplet);


public:
    CondorcetDomain cd;
    std::map<Triplet, std::vector<std::string>> allowed_rules;

    TRSWrapper(CondorcetDomain cd);
    void change_allowed_rules(const std::vector<Triplet>& triplets, const std::vector<std::string>& rules);
    TRS remove_rules(TRS trs, const std::vector<int>& contains);
    Triplet next_unassigned_triplet(const TRS& trs);
    Triplet dynamic_triplet_ordering(const TRS& trs);

    int largest_full_size(const TRS& trs);
};

