#include "wrapper.h"


TRSWrapper::TRSWrapper(CondorcetDomain cd)
{
    this->cd = cd;
    TRS empty_trs = cd.init_empty();
    for (const TripletRule& tr: empty_trs)
    {
        allowed_rules[tr.triplet] = {"1N3", "3N1"};
    }
}

void TRSWrapper::change_allowed_rules(const std::vector<Triplet>& triplets, const std::vector<std::string>& rules)
{
    for (const Triplet& triplet : triplets)
    {
        allowed_rules[triplet] = rules;
    }
}

TRS TRSWrapper::remove_rules(TRS trs, const std::vector<int>& contains)
{
    for (TripletRule& tr : trs)
    {
        Triplet& triplet = tr.triplet;
        for (const int& c : contains)
        {
            if (triplet[0] == c || triplet[1] == c || triplet[2] == c)
                trs = cd.assign(trs, triplet, "");
        }
    }
    return trs;
}

Triplet TRSWrapper::next_unassigned_triplet(const TRS& trs)
{
    return cd.unassigned_triplets(trs)[0];
}


Triplet TRSWrapper::dynamic_triplet_ordering(const TRS& trs)
{
    unsigned int min_size = UINT_MAX;
    Triplet min_triplet;

    std::vector<Triplet> unassigned = cd.unassigned_triplets(trs);
    if (unassigned.empty())
        return Triplet{0, 0, 0};

    for (const Triplet& triplet : unassigned) {
        for (const std::string &rule: allowed_rules[triplet]) {
            TRS new_trs = cd.assign(trs, triplet, rule);
            std::size_t s = cd.condorcet_domain(new_trs).size();
            std::cout << triplet[0] << triplet[1] << triplet[2] << " : " << s << std::endl;
            if (s < min_size)
            {
                min_size = s;
                min_triplet = triplet;
            }
        }
    }

    return min_triplet;

}


int TRSWrapper::largest_full_size(const TRS& trs)
{
    return 100000;
}