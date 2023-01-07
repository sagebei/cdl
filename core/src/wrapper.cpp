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

}

TRS TRSWrapper::remove_rules(TRS trs, const std::vector<int>& contains)
{
    return trs;
}

Triplet TRSWrapper::next_unassigned_triplet(const TRS& trs)
{

}

int TRSWrapper::largest_full_size(const TRS& trs)
{
    return 224;
}