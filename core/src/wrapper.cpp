#include "wrapper.h"
#include <limits>

TRSWrapper::TRSWrapper(CondorcetDomain cd)
{
    this->cd = cd;
    TRS empty_trs = cd.init_trs();
    for (const TripleRule& tr: empty_trs)
    {
        allowed_rules[tr.triple] = {"1N3", "3N1"};
    }
}

void TRSWrapper::change_allowed_rules(const std::vector<Triple>& triples, const std::vector<std::string>& rules)
{
    for (const Triple& triple : triples)
    {
        allowed_rules[triple] = rules;
    }
}

TRS TRSWrapper::remove_rules(TRS trs, const std::vector<int>& contains)
{
    for (TripleRule& tr : trs)
    {
        Triple& triple = tr.triple;
        for (const int& c : contains)
        {
            if (triple[0] == c || triple[1] == c || triple[2] == c)
                trs = cd.assign_id(trs, triple, 0);
        }
    }
    return trs;
}

Triple TRSWrapper::next_unassigned_triple(const TRS& trs)
{
    return cd.unassigned_triples(trs)[0];
}


std::vector<std::size_t> TRSWrapper::evaluate_rules_on_triple(const TRS& trs, const Triple& triple)
{
    std::vector<std::size_t> sizes = {};
    for (const std::string& rule: allowed_rules[triple])
    {
        TRS new_trs = cd.assign_rule(trs, triple, rule);
        std::size_t s = cd.size(new_trs);
        sizes.push_back(s);
    }
    return sizes;
}

Triple TRSWrapper::dynamic_triple_ordering(const TRS& trs)
{
    std::vector<Triple> unassigned = cd.unassigned_triples(trs);
    if (unassigned.empty())
        return Triple{0, 0, 0};

    unsigned int min_size = UINT_MAX;
    Triple min_triple;

    for (const Triple& unassigned_triple : unassigned)
    {
        auto sizes = evaluate_rules_on_triple(trs, unassigned_triple);
        auto max_size = *std::max_element(sizes.begin(), sizes.end());
        if (max_size < min_size)
        {
            min_size = max_size;
            min_triple = unassigned_triple;
        }
    }

    return min_triple;
}


int TRSWrapper::largest_full_size(const TRS& trs)
{
    return 100000;
}