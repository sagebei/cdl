#pragma once

#include "condorcet_domain.h"


class TRSWrapper
{
private:
    CondorcetDomain cd;
    std::map<Triplet, std::vector<std::string>> allowed_rules;

public:
    TRSWrapper(CondorcetDomain cd);
    void change_allowed_rules(const std::vector<Triplet>& triplets, const std::vector<std::string>& rules);
    TRS remove_rules(TRS trs, const std::vector<int>& contains);
    Triplet next_unassigned_triplet(const TRS& trs);
    int largest_full_size(const TRS& trs);
};

